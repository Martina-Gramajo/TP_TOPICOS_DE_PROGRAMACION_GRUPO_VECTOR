#include "juego.h"
/*
    Incluimos juego.h porque acá implementamos las funciones
    declaradas ahí.

    Este archivo necesita conocer:
        - tJuego
        - eEstadoJuego
        - tConfig
        - tTablero
        - tTetromino
        - tRender
        - funciones de tablero, tetromino, render y stats
*/

#include <stdlib.h>
/*
    Incluimos stdlib.h.

    En este archivo se usa indirectamente para funciones generales.
*/

#include <string.h>
/*
    Incluimos string.h para usar:
        memset
        strcpy
*/

#include <ctype.h>
/*
    Incluimos ctype.h para usar:
        toupper

    toupper convierte una letra minúscula en mayúscula.

    Ejemplo:
        'a' → 'A'
*/

#ifdef _WIN32
/*
    Esta parte pregunta si estamos compilando en Windows.

    _WIN32 suele estar definido automáticamente
    cuando el programa se compila en Windows.
*/

#include <windows.h>
/*
    En Windows incluimos windows.h para usar GetTickCount().
*/

#else
/*
    Si no estamos en Windows, entramos por acá.
*/

#include <sys/time.h>
/*
    En otros sistemas se usa gettimeofday().
*/

#endif

#include "GBT/gbt.h"
/*
    Incluimos GBT para usar funciones de entrada y dibujo.

    En este archivo usamos:
        gbt_procesar_entrada
        gbt_obtener_tecla_presionada
        gbt_volcar_backbuffer
*/

static uint32_t ahora_ms(void)
/*
    Esta función devuelve el tiempo actual en milisegundos.

    static:
        Significa que solo puede usarse dentro de juego.c.

    uint32_t:
        Devuelve un entero sin signo de 32 bits.

    ¿Para qué sirve?
        Para saber cuándo debe caer automáticamente la pieza.
*/
{
#ifdef _WIN32
    /*
        Si estamos en Windows, usamos GetTickCount().
    */

    return (uint32_t)GetTickCount();
    /*
        GetTickCount devuelve la cantidad de milisegundos
        desde que se inició el sistema.

        Lo convertimos a uint32_t.

        Esto nos sirve como reloj para el juego.
    */

#else
    /*
        Si no estamos en Windows, usamos otra forma de obtener tiempo.
    */

    struct timeval tv;
    /*
        Estructura para guardar segundos y microsegundos.
    */

    gettimeofday(&tv, NULL);
    /*
        Obtiene el tiempo actual.
    */

    return (uint32_t)(tv.tv_sec * 1000u + tv.tv_usec / 1000u);
    /*
        Convertimos:
            segundos a milisegundos
            microsegundos a milisegundos

        y devolvemos el total.
    */
#endif
}

static void generar_nueva_pieza(tJuego *juego)
/*
    Genera una nueva pieza aleatoria y la coloca arriba del tablero.

    static:
        Solo se usa dentro de juego.c.

    Recibe:
        juego:
            puntero a la estructura principal del juego.

    Modifica:
        juego->actual
        juego->piezaX
        juego->piezaY
*/
{
    tetromino_crear_aleatorio(&juego->actual);
    /*
        Crea una pieza aleatoria y la guarda en juego->actual.

        &juego->actual:
            dirección de memoria de la pieza actual.

        Esta función elige entre:
            I, J, L, O, S, T, Z.
    */

    juego->piezaX = (TABLERO_COLUMNAS / 2) - 2;
    /*
        Posiciona la pieza horizontalmente cerca del centro.

        TABLERO_COLUMNAS vale 10.

        Entonces:
            TABLERO_COLUMNAS / 2 = 5
            5 - 2 = 3

        piezaX = 3

        ¿Por qué -2?
            Porque la pieza se guarda en una matriz 4x4.
            Restar 2 ayuda a centrarla.
    */

    juego->piezaY = 0;
    /*
        Ubica la pieza arriba del tablero.

        Recordá que hay filas ocultas arriba.
    */
}

static void actualizar_record(tJuego *juego)
/*
    Carga el récord del jugador actual desde stats.txt.

    Modifica:
        juego->recordJugador
*/
{
    tStatsJugador st;
    /*
        Creamos una variable auxiliar de estadísticas.
    */

    stats_cargar(juego->nombre, &st);
    /*
        Cargamos las estadísticas del jugador cuyo nombre está
        en juego->nombre.

        El resultado se guarda en st.
    */

    juego->recordJugador = st.mejorPuntaje;
    /*
        Guardamos el mejor puntaje del jugador en la estructura juego.
    */
}

void juego_inicializar(tJuego *juego, const tConfig *cfg)
/*
    Inicializa la estructura principal del juego.

    Se llama una vez desde main, después de crear la ventana.

    juego:
        estructura que se va a modificar.

    cfg:
        configuración ya cargada.
        Es const porque no la vamos a modificar.
*/
{
    memset(juego, 0, sizeof(*juego));
    /*
        Limpia toda la estructura juego dejándola en cero.

        sizeof(*juego):
            tamaño completo de la estructura tJuego.

        Esto evita que queden valores basura.
    */

    juego->cfg = *cfg;
    /*
        Copiamos la configuración dentro de juego.

        Como cfg es un puntero, *cfg significa:
            "el contenido apuntado por cfg".

        Después de esto, juego tiene su propia copia
        de la configuración.
    */

    render_configurar(&juego->render, cfg);
    /*
        Configuramos los datos de dibujo.

        Le pasamos:
            &juego->render:
                estructura de render que se va a modificar.

            cfg:
                configuración con resolución, ancho, alto, etc.

        Esto calcula:
            tamaño del bloque
            posición del tablero
            posición del panel lateral
    */

    strcpy(juego->nombre, "JUGADOR");
    /*
        Copia el texto "JUGADOR" en juego->nombre.

        Es el nombre por defecto.
    */

    juego->nombreLen = 7;
    /*
        "JUGADOR" tiene 7 letras.

        Guardamos la longitud para saber dónde agregar o borrar caracteres.
    */

    juego->estado = EST_PRESENTACION;
    /*
        El juego arranca en la pantalla de presentación.
    */

    juego->velocidadCaidaMs = cfg->velocidadInicialMs;
    /*
        La velocidad actual de caída empieza siendo la velocidad inicial
        de la configuración.

        Normalmente:
            1000 ms.
    */
}

void juego_reiniciar_partida(tJuego *juego)
/*
    Reinicia la partida desde cero.

    Se llama cuando:
        - el jugador confirma el nombre
        - el jugador toca ENTER en game over

    Modifica muchas partes de tJuego.
*/
{
    tablero_inicializar(&juego->tablero);
    /*
        Limpia la matriz del tablero.

        Todas las celdas quedan en 0.
    */

    juego->puntos = 0;
    /*
        Reinicia el puntaje.
    */

    juego->lineas = 0;
    /*
        Reinicia la cantidad de líneas limpiadas.
    */

    juego->piezasFijadas = 0;
    /*
        Reinicia la cantidad de piezas que cayeron.
    */

    juego->velocidadCaidaMs = juego->cfg.velocidadInicialMs;
    /*
        Vuelve la velocidad a la inicial.

        Ejemplo:
            1000 ms.
    */

    juego->ultimoTickCaida = ahora_ms();
    /*
        Guarda el tiempo actual como último momento de caída.

        Esto evita que la pieza baje instantáneamente
        apenas empieza la partida.
    */

    generar_nueva_pieza(juego);
    /*
        Genera la primera pieza de la partida.
    */

    actualizar_record(juego);
    /*
        Carga el récord del jugador actual.
    */

    juego->estado = EST_JUGANDO;
    /*
        Cambia el estado del juego a partida activa.
    */
}

static void sumar_puntos_por_lineas(tJuego *juego, uint8_t lineas)
/*
    Suma puntos según la cantidad de líneas borradas.

    static:
        Solo se usa dentro de juego.c.

    Modifica:
        juego->puntos
*/
{
    uint32_t base = 0;
    /*
        Puntaje base según cuántas líneas se borraron.
    */

    uint32_t multiplicadorVelocidad;
    /*
        Multiplicador que intenta dar más puntos
        cuando la velocidad es mayor.
    */

    if (lineas == 1) base = 100;
    /*
        1 línea limpia suma 100 puntos base.
    */

    else if (lineas == 2) base = 300;
    /*
        2 líneas juntas suman 300 puntos base.
    */

    else if (lineas == 3) base = 500;
    /*
        3 líneas juntas suman 500 puntos base.
    */

    else if (lineas >= 4) base = 800;
    /*
        4 líneas juntas, que sería un Tetris,
        suman 800 puntos base.
    */

    multiplicadorVelocidad = juego->cfg.velocidadInicialMs / juego->velocidadCaidaMs;
    /*
        Calcula un multiplicador según la velocidad.

        Si la velocidad actual es menor que la inicial,
        quiere decir que el juego va más rápido.

        Ejemplo:
            inicial = 1000
            actual = 500

            1000 / 500 = 2

        Entonces daría el doble de puntos.

        Ojo:
            Como es división entera, muchas veces puede dar 1.
    */

    if (multiplicadorVelocidad < 1) {
    /*
        Por seguridad, si el multiplicador queda menor a 1,
        lo forzamos a 1.
    */

        multiplicadorVelocidad = 1;
    }

    juego->puntos += base * multiplicadorVelocidad;
    /*
        Sumamos al puntaje:

            puntos = puntos + base * multiplicador

        Ejemplo:
            base = 300
            multiplicador = 1

            suma 300 puntos.
    */
}

static void fijar_actual_y_continuar(tJuego *juego)
/*
    Esta función se llama cuando la pieza actual
    ya no puede seguir bajando.

    Es una de las funciones más importantes del juego.

    Hace:
        1. Fija la pieza en el tablero.
        2. Limpia líneas.
        3. Suma puntos.
        4. Aumenta velocidad cada 10 piezas.
        5. Genera una nueva pieza.
        6. Revisa si hubo game over.
*/
{
    uint8_t lineasBorradas;
    /*
        Variable para guardar cuántas líneas se borraron
        después de fijar la pieza.
    */

    tablero_fijar_pieza(&juego->tablero, &juego->actual, juego->piezaX, juego->piezaY);
    /*
        Copia la pieza actual al tablero fijo.

        Le pasamos:
            tablero
            pieza actual
            posición X
            posición Y

        Después de esto, la pieza queda guardada en la matriz del tablero.
    */

    juego->piezasFijadas++;
    /*
        Sumamos 1 a la cantidad de piezas que ya cayeron.
    */

    lineasBorradas = tablero_limpiar_lineas(&juego->tablero);
    /*
        Revisa el tablero, borra líneas completas
        y devuelve cuántas borró.
    */

    if (lineasBorradas > 0) {
    /*
        Si se borró al menos una línea...
    */

        juego->lineas += lineasBorradas;
        /*
            Sumamos esas líneas al contador total de líneas.
        */

        sumar_puntos_por_lineas(juego, lineasBorradas);
        /*
            Sumamos puntos según cuántas líneas se borraron.
        */
    }

    if (juego->piezasFijadas % 10 == 0) {
    /*
        Cada 10 piezas fijadas, aumentamos la velocidad.

        % es el módulo, devuelve el resto de una división.

        Si piezasFijadas % 10 == 0,
        significa que piezasFijadas es múltiplo de 10.
    */

        juego->velocidadCaidaMs = (uint32_t)(juego->velocidadCaidaMs * 97 / 100);
        /*
            Reducimos el tiempo de caída al 97%.

            Ejemplo:
                1000 * 97 / 100 = 970

            Si el tiempo baja, la pieza cae más rápido.

            Esto cumple la idea de aumentar la velocidad un 3%.
        */

        if (juego->velocidadCaidaMs < 80) {
        /*
            Ponemos un límite mínimo.

            No queremos que la velocidad baje infinitamente
            y el juego se vuelva imposible o inestable.
        */

            juego->velocidadCaidaMs = 80;
            /*
                Mínimo 80 ms.
            */
        }
    }

    generar_nueva_pieza(juego);
    /*
        Después de fijar la pieza, generamos otra nueva.
    */

    if (!tablero_puede_ubicar(&juego->tablero, &juego->actual, juego->piezaX, juego->piezaY) ||
        tablero_hay_bloques_en_zona_oculta(&juego->tablero)) {
    /*
        Revisamos si el jugador perdió.

        Primera condición:
            !tablero_puede_ubicar(...)

            Si la nueva pieza no puede aparecer en su posición inicial,
            es game over.

        Segunda condición:
            tablero_hay_bloques_en_zona_oculta(...)

            Si hay bloques en la zona oculta de arriba,
            también es game over.

        || significa "o".
        Si cualquiera de las dos condiciones es verdadera,
        el jugador pierde.
    */

        stats_guardar_resultado(juego->nombre, juego->puntos, juego->lineas);
        /*
            Guardamos el resultado de la partida en stats.txt.

            Guarda:
                nombre
                puntaje
                líneas
        */

        actualizar_record(juego);
        /*
            Cargamos el récord actualizado desde el archivo.
        */

        if (juego->puntos > juego->recordJugador) {
        /*
            Si el puntaje actual es mayor que el récord cargado...
        */

            juego->recordJugador = juego->puntos;
            /*
                Actualizamos el récord en memoria.
            */
        }

        juego->estado = EST_GAME_OVER;
        /*
            Cambiamos a pantalla de game over.
        */
    }
}

static void intentar_mover(tJuego *juego, int dx, int dy, uint8_t sumaPuntoManual)
/*
    Intenta mover la pieza actual.

    Parámetros:
        dx:
            movimiento horizontal.

            -1 → izquierda
             1 → derecha
             0 → no cambia horizontalmente

        dy:
            movimiento vertical.

             1 → bajar
             0 → no baja

        sumaPuntoManual:
            indica si debe sumar punto por bajada manual.

    Modifica:
        piezaX
        piezaY
        puntos
        o incluso puede fijar la pieza si no puede bajar.
*/
{
    if (tablero_puede_ubicar(&juego->tablero, &juego->actual, juego->piezaX + dx, juego->piezaY + dy)) {
    /*
        Antes de mover la pieza, preguntamos si se puede ubicar
        en la nueva posición.

        Nueva posición:
            X = piezaX + dx
            Y = piezaY + dy

        Si se puede ubicar, el movimiento es válido.
    */

        juego->piezaX += dx;
        /*
            Actualizamos la posición horizontal.
        */

        juego->piezaY += dy;
        /*
            Actualizamos la posición vertical.
        */

        if (sumaPuntoManual && dy > 0) {
        /*
            Si el movimiento fue una bajada manual
            y realmente bajó...
        */

            juego->puntos += 1;
            /*
                Sumamos 1 punto.

                Esto cumple el requisito de sumar puntos
                por bajar manualmente la pieza.
            */
        }

    } else if (dy > 0) {
    /*
        Si no se puede mover y el movimiento era hacia abajo,
        significa que la pieza tocó el piso o chocó con otra pieza.
    */

        fijar_actual_y_continuar(juego);
        /*
            Entonces fijamos la pieza en el tablero
            y seguimos con la próxima.
        */
    }
}

static void intentar_rotar_derecha(tJuego *juego)
/*
    Intenta rotar la pieza actual hacia la derecha.

    No aplica la rotación directamente.
    Primero prueba si la pieza rotada entra en el tablero.
*/
{
    tTetromino rotada;
    /*
        Creamos una pieza temporal.

        Acá vamos a guardar la versión rotada.
    */

    tetromino_rotar_derecha(&juego->actual, &rotada);
    /*
        Generamos la pieza rotada hacia la derecha.

        No se modifica juego->actual todavía.
    */

    if (tablero_puede_ubicar(&juego->tablero, &rotada, juego->piezaX, juego->piezaY)) {
    /*
        Preguntamos si la pieza rotada puede estar
        en la posición actual.

        Esto evita que rote atravesando paredes o bloques.
    */

        tetromino_copiar(&juego->actual, &rotada);
        /*
            Si la rotación es válida, copiamos la rotada
            como pieza actual.
        */
    }
}

static void intentar_rotar_izquierda(tJuego *juego)
/*
    Intenta rotar la pieza actual hacia la izquierda.
*/
{
    tTetromino rotada;
    /*
        Pieza temporal donde se guarda la rotación.
    */

    tetromino_rotar_izquierda(&juego->actual, &rotada);
    /*
        Generamos la versión rotada hacia la izquierda.
    */

    if (tablero_puede_ubicar(&juego->tablero, &rotada, juego->piezaX, juego->piezaY)) {
    /*
        Solo aceptamos la rotación si no choca.
    */

        tetromino_copiar(&juego->actual, &rotada);
        /*
            Aplicamos la rotación.
        */
    }
}

static void actualizar_nombre(tJuego *juego, eGBT_Tecla tecla)
/*
    Maneja el ingreso del nombre del jugador.

    Se llama cuando el estado es EST_NOMBRE.

    Recibe:
        juego:
            estructura principal.

        tecla:
            tecla presionada según GBT.

    Puede modificar:
        nombre
        nombreLen
        estado
        y también reiniciar la partida al tocar ENTER.
*/
{
    if (tecla == GBTK_ENTER) {
    /*
        Si el jugador toca ENTER,
        confirma el nombre.
    */

        if (juego->nombreLen == 0) {
        /*
            Si no escribió nada...
        */

            strcpy(juego->nombre, "JUGADOR");
            /*
                Usamos "JUGADOR" como nombre por defecto.
            */

            juego->nombreLen = 7;
            /*
                Actualizamos la longitud del nombre.
            */
        }

        juego_reiniciar_partida(juego);
        /*
            Después de confirmar el nombre,
            arrancamos la partida.
        */

        return;
        /*
            Salimos de la función.
        */
    }

    if (tecla == GBTK_RETROCESO) {
    /*
        Si el jugador toca Backspace...
    */

        if (juego->nombreLen > 0) {
        /*
            Solo podemos borrar si hay al menos una letra.
        */

            juego->nombreLen--;
            /*
                Reducimos la longitud en 1.
            */

            juego->nombre[juego->nombreLen] = '\0';
            /*
                Ponemos el fin de string en la nueva posición.

                Esto borra visualmente la última letra.
            */
        }

        return;
        /*
            Salimos porque ya procesamos la tecla.
        */
    }

    if (juego->nombreLen < STATS_NOMBRE_MAX - 1) {
    /*
        Si todavía hay espacio para agregar caracteres.

        Restamos 1 porque en C los strings necesitan
        un carácter final '\0'.
    */

        char c = 0;
        /*
            Variable auxiliar para guardar el carácter a agregar.

            Si queda en 0, no se agrega nada.
        */

        if (tecla >= GBTK_a && tecla <= GBTK_z) {
        /*
            Si la tecla es una letra minúscula entre a y z...
        */

            c = (char)toupper((int)tecla);
            /*
                La convertimos a mayúscula.

                Ejemplo:
                    a → A
                    g → G
            */

        } else if (tecla >= GBTK_0 && tecla <= GBTK_9) {
        /*
            Si la tecla es un número entre 0 y 9...
        */

            c = (char)tecla;
            /*
                Lo guardamos tal cual.
            */
        }

        if (c) {
        /*
            Si c tiene algún carácter válido...
        */

            juego->nombre[juego->nombreLen++] = c;
            /*
                Guardamos el carácter al final del nombre
                y después aumentamos nombreLen.
            */

            juego->nombre[juego->nombreLen] = '\0';
            /*
                Cerramos el string con '\0'.

                Esto es obligatorio en C para que el texto termine bien.
            */
        }
    }
}

void juego_actualizar(tJuego *juego)
/*
    Esta es la función principal de lógica del juego.

    Se llama en cada vuelta del while principal del main.

    Hace:
        - leer teclado
        - cambiar estados
        - mover piezas
        - rotar piezas
        - bajar automáticamente
        - pausar
        - game over
*/
{
    eGBT_Tecla tecla;
    /*
        Variable donde guardamos la tecla presionada.
    */

    uint32_t t;
    /*
        Variable para guardar el tiempo actual en milisegundos.
    */

    gbt_procesar_entrada();
    /*
        Le pedimos a GBT que procese los eventos de teclado.

        Es necesario hacerlo antes de consultar qué tecla se presionó.
    */

    tecla = gbt_obtener_tecla_presionada();
    /*
        Obtenemos la tecla que fue presionada.

        Si no se presionó nada útil, puede devolver GBTK_DESCONOCIDA.
    */

    if (tecla == GBTK_ESCAPE) {
    /*
        Si el usuario toca ESC...
    */

        juego->estado = EST_SALIR;
        /*
            Cambiamos el estado a salir.
        */

        return;
        /*
            Salimos de la función.

            Luego el main va a salir del while porque
            juego_esta_corriendo devolverá 0.
        */
    }

    switch (juego->estado) {
    /*
        Según el estado actual del juego,
        hacemos una cosa distinta.

        Esto ordena mucho el código.

        No es lo mismo estar:
            - en presentación
            - escribiendo nombre
            - jugando
            - pausado
            - en game over
    */

        case EST_PRESENTACION:
        /*
            Pantalla inicial.
        */

            if (tecla == GBTK_ENTER) {
            /*
                Si el jugador toca ENTER...
            */

                juego->estado = EST_NOMBRE;
                /*
                    Pasamos a la pantalla de ingresar nombre.
                */

                juego->nombre[0] = '\0';
                /*
                    Vaciamos el nombre.

                    '\0' al principio significa string vacío.
                */

                juego->nombreLen = 0;
                /*
                    La longitud del nombre pasa a ser 0.
                */
            }

            break;
            /*
                Fin del caso EST_PRESENTACION.
            */

        case EST_NOMBRE:
        /*
            Estado donde el usuario escribe su nombre.
        */

            actualizar_nombre(juego, tecla);
            /*
                Procesamos la tecla para agregar letra,
                borrar o confirmar con ENTER.
            */

            break;
            /*
                Fin del caso EST_NOMBRE.
            */

        case EST_JUGANDO:
        /*
            Estado de partida activa.
        */

            if (tecla == GBTK_p) {
            /*
                Si toca P...
            */

                juego->estado = EST_PAUSA;
                /*
                    Pasamos a pausa.
                */

                break;
                /*
                    Salimos del caso para no seguir procesando movimiento.
                */
            }

            if (tecla == GBTK_IZQUIERDA) {
            /*
                Si toca flecha izquierda...
            */

                intentar_mover(juego, -1, 0, 0);
                /*
                    Intentamos mover la pieza una columna a la izquierda.

                    dx = -1
                    dy = 0
                    sumaPuntoManual = 0
                */

            } else if (tecla == GBTK_DERECHA) {
            /*
                Si toca flecha derecha...
            */

                intentar_mover(juego, 1, 0, 0);
                /*
                    Intentamos mover una columna a la derecha.

                    dx = 1
                    dy = 0
                */

            } else if (tecla == GBTK_ABAJO) {
            /*
                Si toca flecha abajo...
            */

                intentar_mover(juego, 0, 1, 1);
                /*
                    Intentamos bajar manualmente una fila.

                    dx = 0
                    dy = 1
                    sumaPuntoManual = 1

                    Si baja, suma 1 punto.
                */

            } else if (tecla == GBTK_x) {
            /*
                Si toca X...
            */

                intentar_rotar_derecha(juego);
                /*
                    Intenta rotar la pieza hacia la derecha.
                */

            } else if (tecla == GBTK_z) {
            /*
                Si toca Z...
            */

                intentar_rotar_izquierda(juego);
                /*
                    Intenta rotar la pieza hacia la izquierda.
                */
            }

            t = ahora_ms();
            /*
                Guardamos el tiempo actual en milisegundos.
            */

            if (t - juego->ultimoTickCaida >= juego->velocidadCaidaMs) {
            /*
                Revisamos si ya pasó suficiente tiempo
                desde la última caída automática.

                t - ultimoTickCaida:
                    tiempo transcurrido desde que bajó por última vez.

                velocidadCaidaMs:
                    tiempo que debe pasar para que baje otra vez.

                Si el tiempo transcurrido es mayor o igual,
                la pieza debe bajar automáticamente.
            */

                intentar_mover(juego, 0, 1, 0);
                /*
                    Bajamos la pieza una fila automáticamente.

                    sumaPuntoManual = 0
                    porque no fue bajada por el jugador.
                */

                juego->ultimoTickCaida = t;
                /*
                    Actualizamos el último momento de caída.
                */
            }

            break;
            /*
                Fin del caso EST_JUGANDO.
            */

        case EST_PAUSA:
        /*
            Estado de pausa.
        */

            if (tecla == GBTK_p || tecla == GBTK_ENTER) {
            /*
                Si toca P o ENTER...
            */

                juego->estado = EST_JUGANDO;
                /*
                    Volvemos a jugar.
                */

                juego->ultimoTickCaida = ahora_ms();
                /*
                    Reiniciamos el tiempo de caída.

                    Esto evita que al salir de pausa la pieza baje
                    instantáneamente por el tiempo acumulado.
                */
            }

            break;
            /*
                Fin de pausa.
            */

        case EST_GAME_OVER:
        /*
            Estado de fin de partida.
        */

            if (tecla == GBTK_ENTER) {
            /*
                Si toca ENTER...
            */

                juego_reiniciar_partida(juego);
                /*
                    Empieza una nueva partida con el mismo jugador.
                */
            }

            break;
            /*
                Fin de game over.
            */

        case EST_SALIR:
        /*
            Estado de salida.
        */

        default:
        /*
            default se ejecuta si el estado no coincide
            con ningún caso anterior.

            En este proyecto no hace nada.
        */

            break;
    }
}

void juego_dibujar(const tJuego *juego)
/*
    Dibuja la pantalla correspondiente al estado actual.

    Recibe const tJuego *juego:
        significa que esta función no debería modificar el juego.

    Solo lee datos y llama funciones de render.
*/
{
    switch (juego->estado) {
    /*
        Según el estado, dibujamos una pantalla distinta.
    */

        case EST_PRESENTACION:
        /*
            Si estamos en presentación...
        */

            render_presentacion(&juego->render);
            /*
                Dibuja la pantalla inicial.
            */

            break;

        case EST_NOMBRE:
        /*
            Si estamos pidiendo nombre...
        */

            render_pedir_nombre(&juego->render, juego->nombre);
            /*
                Dibuja la pantalla de nombre,
                mostrando lo que se escribió hasta ahora.
            */

            break;

        case EST_JUGANDO:
        /*
            Si estamos jugando...
        */

            render_juego(&juego->render, &juego->tablero, &juego->actual,
                         juego->piezaX, juego->piezaY, juego->puntos, juego->lineas,
                         juego->piezasFijadas, juego->velocidadCaidaMs, juego->nombre);
            /*
                Dibuja la partida completa:

                    tablero
                    pieza actual
                    puntaje
                    líneas
                    piezas
                    velocidad
                    nombre del jugador
            */

            break;

        case EST_PAUSA:
        /*
            Si está pausado...
        */

            render_juego(&juego->render, &juego->tablero, &juego->actual,
                         juego->piezaX, juego->piezaY, juego->puntos, juego->lineas,
                         juego->piezasFijadas, juego->velocidadCaidaMs, juego->nombre);
            /*
                Primero dibuja el juego como estaba.
            */

            render_pausa(&juego->render);
            /*
                Después dibuja el cartel de pausa encima.
            */

            break;

        case EST_GAME_OVER:
        /*
            Si perdió...
        */

            render_game_over(&juego->render, juego->puntos, juego->recordJugador);
            /*
                Dibuja la pantalla de game over,
                mostrando puntaje y récord.
            */

            break;

        default:
        /*
            Para cualquier otro estado, limpiamos la pantalla.
        */

            render_limpiar();
            /*
                Borra el backbuffer con color de fondo.
            */

            break;
    }

    gbt_volcar_backbuffer();
    /*
        Muestra en pantalla todo lo que se dibujó.

        GBT dibuja primero en un backbuffer,
        que es como una pantalla invisible.

        Cuando llamamos a gbt_volcar_backbuffer(),
        ese contenido se copia a la ventana visible.
    */
}

uint8_t juego_esta_corriendo(const tJuego *juego)
/*
    Indica si el programa debe seguir ejecutándose.

    Devuelve:
        1 → seguir corriendo
        0 → terminar
*/
{
    return juego->estado != EST_SALIR;
    /*
        Si el estado NO es EST_SALIR, devuelve 1.

        Si el estado es EST_SALIR, devuelve 0.

        Esta función se usa en main:

            while (juego_esta_corriendo(&juego)) {
                ...
            }
    */
}
