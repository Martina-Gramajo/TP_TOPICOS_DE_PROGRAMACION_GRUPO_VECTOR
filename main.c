/*
Apellido(s), nombre(s): Gamba, Agustin
DNI: 44824752
Usuario: agusgamba
Entrega: Si

Apellido(s), nombre(s): Saldutti, Gastón
DNI:
Usuario: GastonSaldutti
Entrega: Si

Apellido(s), nombre(s): Gramajo, Martina Dana
DNI: 45689186
Usuario: Martina-Gramajo
Entrega: Si
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "GBT/gbt.h"
#include "config.h"
#include "render.h"
#include "juego.h"

int main(int argc, char *argv[])
/*
    Esta es la función principal del programa.
    Todo programa en C empieza ejecutando main.

    int:
        Significa que main devuelve un número entero.
        return 0 significa "terminó bien".
        return -1 significa "terminó con error".

    argc:
        Es la cantidad de argumentos que recibió el programa al ejecutarse.

    argv:
        Es un arreglo/lista de textos con esos argumentos.

    Ejemplo:
        Si ejecutás:
            tetris.exe VGA ESCALA=2 VEL=800

        Entonces:
            argc = 4
            argv[0] = "tetris.exe"
            argv[1] = "VGA"
            argv[2] = "ESCALA=2"
            argv[3] = "VEL=800"
*/
{
    tConfig cfg;
    /*
        Declara una variable llamada cfg de tipo tConfig.

        tConfig es una estructura que creamos en config.h.
        Guarda la configuración del juego, por ejemplo:
            - resolución: CGA o VGA
            - ancho de ventana
            - alto de ventana
            - escala
            - velocidad inicial de caída
            - paleta de colores

        Todavía acá no tiene valores útiles cargados.
        Solo reservamos la variable en memoria.
    */

    tJuego juego;
    /*
        Declara una variable llamada juego de tipo tJuego.

        tJuego es la estructura principal del juego.
        Guarda casi todo:
            - configuración
            - datos de renderizado
            - tablero
            - pieza actual
            - posición de la pieza
            - estado actual: presentación, jugando, pausa, game over, salir
            - nombre del jugador
            - puntos
            - líneas
            - velocidad actual

        Es como "la caja grande" donde vive la partida.
    */

    char titulo[64];
    /*
        Declara un arreglo de 64 caracteres.

        En C, los textos se guardan como arreglos de char.
        Este arreglo se usa para armar el título de la ventana.

        Ejemplo:
            "Tetris GBT 320x200"

        El 64 significa que puede guardar hasta 63 caracteres útiles
        más el carácter final '\0', que marca el fin del texto.
    */

    srand((unsigned)time(NULL));
    /*
        Inicializa la semilla de los números aleatorios.

        srand:
            Configura desde dónde arranca la secuencia de rand().

        time(NULL):
            Devuelve el tiempo actual.

        (unsigned):
            Convierte ese valor a unsigned, que es un entero sin signo.

        ¿Para qué sirve?
            Para que rand() no dé siempre la misma secuencia.

        En este proyecto se usa rand() para elegir piezas aleatorias.
        Si no ponés srand(time(NULL)), cada vez que abras el juego
        podrían aparecer las mismas piezas en el mismo orden.
    */

    config_cargar_default(&cfg);
    /*
        Carga valores por defecto en la configuración.

        &cfg:
            Significa "la dirección de memoria de cfg".
            Se pasa así para que la función pueda modificar la variable original.

        Esta función carga algo parecido a:
            resolución = CGA
            escala = 3
            velocidad inicial = 1000 ms
            paleta = 0

        También calcula ancho y alto según la resolución.
    */

    config_leer_archivo(&cfg);
    /*
        Intenta leer la configuración desde config.txt.

        Si existe config.txt, toma los valores guardados ahí.
        Por ejemplo:
            resolucion=0
            escala=3
            velocidad=1000
            paleta=0

        Si el archivo no existe, no pasa nada:
            se siguen usando los valores por defecto cargados antes.

        Se pasa &cfg porque la función puede modificar cfg.
    */

    config_aplicar_argumentos(&cfg, argc, argv);
    /*
        Revisa si el programa fue ejecutado con argumentos.

        Ejemplo:
            tetris.exe VGA ESCALA=1 VEL=800

        Esta función mira argc y argv.
        Si encuentra "VGA", cambia la resolución.
        Si encuentra "ESCALA=1", cambia la escala.
        Si encuentra "VEL=800", cambia la velocidad inicial.

        Orden de prioridad:
            1. Primero se cargan valores por defecto.
            2. Después se lee config.txt.
            3. Por último, los argumentos pisan lo anterior.

        Se pasa &cfg porque puede modificar la configuración.
    */

    config_guardar_archivo(&cfg);
    /*
        Guarda la configuración actual en config.txt.

        ¿Por qué después de aplicar argumentos?
            Porque si abriste el programa con:
                VGA ESCALA=1

            queda guardado para la próxima vez.

        Recibe &cfg, pero en este caso solo lee los valores
        para escribirlos en el archivo.
    */

    if (gbt_iniciar() != 0) {
    /*
        gbt_iniciar():
            Inicializa la biblioteca GBT.

        GBT es la biblioteca gráfica que usamos para:
            - crear ventana
            - dibujar píxeles
            - leer teclado
            - manejar backbuffer
            - esperar tiempos

        != 0:
            Significa "si el resultado es distinto de cero".

        Convención:
            0 significa que salió bien.
            distinto de 0 significa que hubo un error.

        Entonces esta línea significa:
            "Si GBT no pudo iniciar, entrá al if".
    */

        fprintf(stderr, "Error al iniciar GBT: %s\n", gbt_obtener_log());
        /*
            Imprime un mensaje de error.

            fprintf:
                Es parecido a printf, pero permite elegir dónde imprimir.

            stderr:
                Es la salida estándar de errores.

            "Error al iniciar GBT: %s\n":
                Es el texto que se imprime.
                %s es un espacio donde se pone un texto.
                \n hace un salto de línea.

            gbt_obtener_log():
                Devuelve el último mensaje de error generado por GBT.

            Entonces imprime algo como:
                Error al iniciar GBT: no se pudo inicializar...
        */

        return -1;
        /*
            Termina el programa con error.

            Como GBT no inició, no tiene sentido seguir,
            porque no se podría crear ventana ni dibujar.
        */
    }

    sprintf(titulo, "Tetris GBT %ux%u", cfg.ancho, cfg.alto);
    /*
        Arma el título de la ventana y lo guarda en la variable titulo.

        sprintf:
            Es como printf, pero en vez de imprimir en pantalla,
            escribe el texto dentro de un arreglo de char.

        titulo:
            Es el destino donde se guarda el texto.

        "Tetris GBT %ux%u":
            Es el formato del texto.

        %u:
            Significa "número entero sin signo".

        cfg.ancho:
            Ancho lógico de la ventana.

        cfg.alto:
            Alto lógico de la ventana.

        Ejemplo:
            Si cfg.ancho = 320 y cfg.alto = 200,
            titulo queda:
                "Tetris GBT 320x200"
    */

    if (gbt_crear_ventana(titulo, cfg.ancho, cfg.alto, cfg.escala) != 0) {
    /*
        Crea la ventana gráfica usando GBT.

        Parámetros:
            titulo:
                Texto que aparece como título de la ventana.

            cfg.ancho:
                Ancho lógico de la ventana.

            cfg.alto:
                Alto lógico de la ventana.

            cfg.escala:
                Factor de escala visual.

        Ejemplo:
            ancho lógico = 320
            alto lógico = 200
            escala = 3

            La ventana se ve más grande, pero internamente dibujamos
            en coordenadas 320x200.

        != 0:
            Si devuelve distinto de cero, hubo error creando la ventana.
    */

        fprintf(stderr, "Error al crear ventana: %s\n", gbt_obtener_log());
        /*
            Si falló la creación de la ventana, imprime el error.

            gbt_obtener_log() trae el detalle del error desde GBT.
        */

        gbt_cerrar();
        /*
            Como GBT ya había sido iniciado con gbt_iniciar(),
            hay que cerrarlo antes de terminar el programa.

            Esto libera recursos internos de la biblioteca.
        */

        return -1;
        /*
            Termina el programa con error porque no se pudo crear la ventana.
        */
    }

    render_aplicar_paleta(cfg.paleta);
    /*
        Aplica la paleta de colores que se va a usar en el juego.

        cfg.paleta:
            Es el número de paleta elegido.
            En tu proyecto puede ser:
                0 = paleta clásica
                1 = paleta noche

        Esta función está en render.c.

        ¿Por qué está en render y no en main?
            Porque la paleta tiene que ver con el dibujo,
            entonces pertenece al módulo de renderizado.
    */

    juego_inicializar(&juego, &cfg);
    /*
        Inicializa la estructura principal del juego.

        &juego:
            Pasamos la dirección de la variable juego
            para que la función pueda modificarla.

        &cfg:
            Pasamos la configuración ya cargada.

        Dentro de juego_inicializar se hace algo parecido a:
            - limpiar la estructura juego
            - copiar la configuración
            - configurar datos de renderizado
            - poner nombre por defecto "JUGADOR"
            - poner el estado inicial en EST_PRESENTACION
            - cargar velocidad inicial

        Después de esta línea, el juego ya está listo para arrancar.
    */

    while (juego_esta_corriendo(&juego)) {
    /*
        Este es el bucle principal del juego.

        while:
            Repite el bloque mientras la condición sea verdadera.

        juego_esta_corriendo(&juego):
            Devuelve 1 si el juego debe seguir.
            Devuelve 0 si el juego debe terminar.

        El juego deja de correr cuando el estado pasa a EST_SALIR,
        por ejemplo al presionar ESC.

        Este while es el "game loop".
        Casi todos los videojuegos tienen una estructura parecida:
            1. Actualizar lógica.
            2. Dibujar pantalla.
            3. Esperar un poco.
            4. Repetir.
    */

        juego_actualizar(&juego);
        /*
            Actualiza la lógica del juego.

            Acá se procesan cosas como:
                - leer teclado
                - pasar de presentación a nombre
                - escribir nombre
                - mover pieza izquierda/derecha
                - bajar pieza
                - rotar pieza
                - pausar
                - detectar caída automática por tiempo
                - fijar pieza si toca el piso
                - limpiar líneas
                - detectar game over
                - salir con ESC

            Esta función modifica la estructura juego.
        */

        juego_dibujar(&juego);
        /*
            Dibuja la pantalla actual.

            Dependiendo del estado, dibuja:
                - presentación
                - pantalla para ingresar nombre
                - partida
                - pausa
                - game over

            Esta función no debería cambiar la lógica del juego.
            Solo mira el estado actual y lo dibuja.

            Adentro termina llamando a gbt_volcar_backbuffer(),
            que muestra en la ventana todo lo dibujado.
        */

        gbt_esperar(16);
        /*
            Hace una pequeña espera para controlar la velocidad del loop.

            16 milisegundos equivale aproximadamente a 60 FPS.

            Cálculo:
                1000 ms / 60 ≈ 16.6 ms

            ¿Para qué sirve?
                - Evita que el programa use 100% de CPU.
                - Hace que el juego tenga una velocidad estable.
                - Da tiempo entre un frame y otro.

            Esto no es la velocidad de caída de la pieza.
            La velocidad de caída se maneja aparte en juego_actualizar.
        */
    }

    gbt_destruir_ventana();
    /*
        Cuando salimos del while, destruimos la ventana gráfica.

        Esto libera recursos usados por la ventana.
    */

    gbt_cerrar();
    /*
        Cierra la biblioteca GBT.

        Siempre que se llama a gbt_iniciar(),
        al final corresponde llamar a gbt_cerrar().
    */

    return 0;
    /*
        Termina el programa correctamente.

        0 significa:
            "El programa finalizó sin errores".
    */
}
