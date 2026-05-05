#include "render.h"
/*
    Incluimos render.h porque acá implementamos las funciones
    declaradas en ese archivo.

    Este archivo necesita conocer:
        - tRender
        - tConfig
        - tTablero
        - tTetromino
        - colores
        - prototipos de funciones render_*
*/

#include <stdio.h>
/*
    Incluimos stdio.h para usar sprintf.

    sprintf se usa para convertir números a texto.
*/

#include <string.h>
/*
    Incluimos string.h para funciones de manejo de strings.

    En este archivo casi no se usa directamente,
    pero puede ser útil para textos.
*/

#include "GBT/gbt.h"
/*
    Incluimos GBT para poder dibujar.

    Acá usamos funciones como:
        gbt_aplicar_paleta
        gbt_borrar_backbuffer
        gbt_dibujar_pixel
*/

static tGBT_ColorRGB PALETA_CLASICA[16] = {
    /*
        Esta es una paleta de 16 colores.

        static:
            Solo se puede usar dentro de render.c.

        tGBT_ColorRGB:
            Es una estructura de GBT para representar colores RGB.

        Cada color tiene:
            rojo
            verde
            azul

        Los valores están en hexadecimal.
    */

    {0x00,0x00,0x00}, /* 0  - negro */
    {0x00,0x00,0xAA}, /* 1  - azul */
    {0x00,0xAA,0x00}, /* 2  - verde */
    {0x00,0xAA,0xAA}, /* 3  - cian */
    {0xAA,0x00,0x00}, /* 4  - rojo */
    {0xAA,0x00,0xAA}, /* 5  - magenta */
    {0xAA,0x55,0x00}, /* 6  - marrón/naranja */
    {0xAA,0xAA,0xAA}, /* 7  - gris claro */
    {0x55,0x55,0x55}, /* 8  - gris oscuro */
    {0x55,0x55,0xFF}, /* 9  - azul brillante */
    {0x55,0xFF,0x55}, /* 10 - verde brillante */
    {0x55,0xFF,0xFF}, /* 11 - cian brillante */
    {0xFF,0x55,0x55}, /* 12 - rojo brillante */
    {0xFF,0x55,0xFF}, /* 13 - magenta brillante */
    {0xFF,0xFF,0x55}, /* 14 - amarillo */
    {0xFF,0xFF,0xFF}  /* 15 - blanco/transparente según GBT */
};

static tGBT_ColorRGB PALETA_NOCHE[16] = {
    /*
        Segunda paleta de colores.

        La idea es permitir que el usuario elija otra apariencia visual.
    */

    {0x04,0x06,0x12},
    {0x18,0x24,0x70},
    {0x17,0x84,0x44},
    {0x18,0xA0,0xA0},
    {0xA0,0x20,0x30},
    {0x80,0x30,0x90},
    {0xCC,0x7A,0x21},
    {0xB0,0xB0,0xB0},
    {0x35,0x35,0x45},
    {0x55,0x80,0xFF},
    {0x55,0xFF,0x77},
    {0x55,0xFF,0xFF},
    {0xFF,0x55,0x55},
    {0xFF,0x70,0xFF},
    {0xFF,0xE0,0x55},
    {0xF8,0xF8,0xF8}
};

static const char *glyph(char c)
/*
    Esta función devuelve el dibujo de una letra o número.

    static:
        Solo se puede usar dentro de render.c.

    const char *:
        Devuelve un puntero a texto constante.

    La fuente está hecha como una matriz de 5 columnas x 7 filas.

    Cada carácter se representa con una cadena de 35 caracteres.

    Ejemplo:
        "01110"
        "10001"
        "10001"
        "11111"
        "10001"
        "10001"
        "10001"

    En el string aparece todo junto:
        "01110""10001""..."

    Cada posición puede ser:
        '1' → pintar píxel
        '0' → no pintar
*/
{
    switch (c) {
    /*
        Según el carácter recibido, devolvemos su forma.
    */

        case 'A': return "01110""10001""10001""11111""10001""10001""10001";
        case 'B': return "11110""10001""10001""11110""10001""10001""11110";
        case 'C': return "01111""10000""10000""10000""10000""10000""01111";
        case 'D': return "11110""10001""10001""10001""10001""10001""11110";
        case 'E': return "11111""10000""10000""11110""10000""10000""11111";
        case 'F': return "11111""10000""10000""11110""10000""10000""10000";
        case 'G': return "01111""10000""10000""10011""10001""10001""01111";
        case 'H': return "10001""10001""10001""11111""10001""10001""10001";
        case 'I': return "11111""00100""00100""00100""00100""00100""11111";
        case 'J': return "00111""00010""00010""00010""10010""10010""01100";
        case 'K': return "10001""10010""10100""11000""10100""10010""10001";
        case 'L': return "10000""10000""10000""10000""10000""10000""11111";
        case 'M': return "10001""11011""10101""10101""10001""10001""10001";
        case 'N': return "10001""11001""10101""10011""10001""10001""10001";
        case 'O': return "01110""10001""10001""10001""10001""10001""01110";
        case 'P': return "11110""10001""10001""11110""10000""10000""10000";
        case 'Q': return "01110""10001""10001""10001""10101""10010""01101";
        case 'R': return "11110""10001""10001""11110""10100""10010""10001";
        case 'S': return "01111""10000""10000""01110""00001""00001""11110";
        case 'T': return "11111""00100""00100""00100""00100""00100""00100";
        case 'U': return "10001""10001""10001""10001""10001""10001""01110";
        case 'V': return "10001""10001""10001""10001""10001""01010""00100";
        case 'W': return "10001""10001""10001""10101""10101""10101""01010";
        case 'X': return "10001""10001""01010""00100""01010""10001""10001";
        case 'Y': return "10001""10001""01010""00100""00100""00100""00100";
        case 'Z': return "11111""00001""00010""00100""01000""10000""11111";

        case '0': return "01110""10001""10011""10101""11001""10001""01110";
        case '1': return "00100""01100""00100""00100""00100""00100""01110";
        case '2': return "01110""10001""00001""00010""00100""01000""11111";
        case '3': return "11110""00001""00001""01110""00001""00001""11110";
        case '4': return "00010""00110""01010""10010""11111""00010""00010";
        case '5': return "11111""10000""10000""11110""00001""00001""11110";
        case '6': return "01110""10000""10000""11110""10001""10001""01110";
        case '7': return "11111""00001""00010""00100""01000""01000""01000";
        case '8': return "01110""10001""10001""01110""10001""10001""01110";
        case '9': return "01110""10001""10001""01111""00001""00001""01110";

        case ':': return "00000""00100""00100""00000""00100""00100""00000";
        case '-': return "00000""00000""00000""11111""00000""00000""00000";
        case '.': return "00000""00000""00000""00000""00000""01100""01100";

        default: return "00000""00000""00000""00000""00000""00000""00000";
        /*
            Si el carácter no está definido,
            devolvemos una matriz vacía.

            Eso significa que no se dibuja nada.
        */
    }
}

void render_configurar(tRender *r, const tConfig *cfg)
/*
    Configura datos de renderizado según la resolución.

    r:
        estructura de render que se va a modificar.

    cfg:
        configuración del juego.

    Esta función calcula:
        - ancho
        - alto
        - tamaño de bloque
        - posición del tablero
        - posición del panel
*/
{
    r->ancho = cfg->ancho;
    /*
        Copiamos el ancho lógico de la configuración.
    */

    r->alto = cfg->alto;
    /*
        Copiamos el alto lógico de la configuración.
    */

    r->block = cfg->alto >= 480 ? 18 : 8;
    /*
        Elegimos el tamaño de cada bloque.

        Operador ternario:
            condicion ? valor_si_verdadero : valor_si_falso

        Si el alto es 480 o más:
            block = 18

        Si no:
            block = 8

        En VGA hay más espacio, entonces los bloques pueden ser más grandes.
        En CGA hay menos espacio, entonces usamos bloques más chicos.
    */

    r->tableroX = cfg->alto >= 480 ? 150 : 35;
    /*
        Posición X donde empieza el tablero.

        En VGA:
            tableroX = 150

        En CGA:
            tableroX = 35
    */

    r->tableroY = cfg->alto >= 480 ? 60 : 20;
    /*
        Posición Y donde empieza el tablero.

        En VGA:
            tableroY = 60

        En CGA:
            tableroY = 20
    */

    r->panelX = r->tableroX + TABLERO_COLUMNAS * r->block + (cfg->alto >= 480 ? 35 : 20);
    /*
        Calculamos dónde empieza el panel lateral.

        tableroX:
            posición inicial del tablero.

        TABLERO_COLUMNAS * block:
            ancho total del tablero en píxeles.

        espacio extra:
            35 en VGA
            20 en CGA

        Entonces el panel aparece a la derecha del tablero.
    */

    r->panelY = r->tableroY;
    /*
        El panel empieza a la misma altura que el tablero.
    */
}

void render_aplicar_paleta(uint8_t paleta)
/*
    Aplica una de las paletas del juego.

    paleta:
        0 → clásica
        1 → noche
*/
{
    if (paleta == 1) {
    /*
        Si la paleta elegida es 1...
    */

        gbt_aplicar_paleta(PALETA_NOCHE, 16, GBT_FORMATO_888);
        /*
            Aplicamos la paleta noche.

            Parámetros:
                PALETA_NOCHE:
                    array de colores.

                16:
                    cantidad de colores.

                GBT_FORMATO_888:
                    indica que cada canal RGB usa valores de 0 a 255.
        */

    } else {
    /*
        Si no es 1, usamos la clásica.
    */

        gbt_aplicar_paleta(PALETA_CLASICA, 16, GBT_FORMATO_888);
        /*
            Aplicamos la paleta clásica.
        */
    }
}

void render_limpiar(void)
/*
    Limpia la pantalla de dibujo.

    En realidad limpia el backbuffer.
*/
{
    gbt_borrar_backbuffer(COLOR_FONDO);
    /*
        Borra el backbuffer con el color de fondo.

        COLOR_FONDO vale 0, que es negro.
    */
}

void render_pixel_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t color)
/*
    Dibuja un rectángulo de píxeles.

    x, y:
        posición inicial.

    w:
        ancho.

    h:
        alto.

    color:
        índice de color de la paleta.
*/
{
    uint16_t px, py;
    /*
        Variables para recorrer los píxeles del rectángulo.

        px:
            posición horizontal.

        py:
            posición vertical.
    */

    for (py = y; py < y + h; py++) {
    /*
        Recorremos desde la fila y hasta y + h.
    */

        for (px = x; px < x + w; px++) {
        /*
            Recorremos desde la columna x hasta x + w.
        */

            gbt_dibujar_pixel(px, py, color);
            /*
                Dibujamos un píxel en la posición px, py
                con el color indicado.
            */
        }
    }
}

void render_texto(uint16_t x, uint16_t y, const char *texto, uint8_t color, uint8_t escala)
/*
    Dibuja texto usando la función glyph.

    x, y:
        posición inicial del texto.

    texto:
        string a dibujar.

    color:
        color del texto.

    escala:
        tamaño del texto.
        Si escala = 1, cada punto de la letra mide 1x1.
        Si escala = 2, cada punto mide 2x2.
*/
{
    uint16_t cursorX = x;
    /*
        cursorX indica dónde se va dibujando cada letra.

        Empieza en x y va avanzando con cada carácter.
    */

    int i;
    /*
        Índice para recorrer el texto.
    */

    for (i = 0; texto[i] != '\0'; i++) {
    /*
        Recorremos el string carácter por carácter.

        En C, los strings terminan con '\0'.

        Mientras no lleguemos a '\0', seguimos dibujando.
    */

        char c = texto[i];
        /*
            Guardamos el carácter actual.
        */

        if (c == ' ') {
        /*
            Si el carácter es un espacio...
        */

            cursorX += 4 * escala;
            /*
                Avanzamos el cursor sin dibujar nada.

                Esto deja un espacio visual.
            */

            continue;
            /*
                Saltamos al próximo carácter.
            */
        }

        const char *g = glyph(c);
        /*
            Obtenemos la matriz 5x7 del carácter.

            g apunta a una cadena de 35 caracteres,
            formada por 0 y 1.
        */

        int f, col;
        /*
            Variables para recorrer filas y columnas del glyph.
        */

        for (f = 0; f < 7; f++) {
        /*
            La fuente tiene 7 filas.
        */

            for (col = 0; col < 5; col++) {
            /*
                La fuente tiene 5 columnas.
            */

                if (g[f * 5 + col] == '1') {
                /*
                    Calculamos la posición dentro del string del glyph.

                    f * 5 + col:
                        fila actual por 5 columnas + columna actual.

                    Si ahí hay '1', hay que pintar.
                */

                    render_pixel_rect(cursorX + col * escala,
                                      y + f * escala,
                                      escala,
                                      escala,
                                      color);
                    /*
                        Dibujamos un cuadradito.

                        Si escala = 1:
                            pinta 1x1.

                        Si escala = 2:
                            pinta 2x2.

                        Esto agranda la letra.
                    */
                }
            }
        }

        cursorX += 6 * escala;
        /*
            Después de dibujar una letra,
            avanzamos el cursor.

            La letra ocupa 5 columnas.
            Dejamos 1 columna de separación.

            Total:
                6 * escala
        */
    }
}

void render_numero(uint16_t x, uint16_t y, int numero, uint8_t color, uint8_t escala)
/*
    Dibuja un número en pantalla.

    Internamente convierte el número a texto
    y llama a render_texto.
*/
{
    char buffer[32];
    /*
        Buffer para guardar el número convertido a texto.
    */

    sprintf(buffer, "%d", numero);
    /*
        Convertimos el número a string.

        Ejemplo:
            numero = 1250
            buffer = "1250"
    */

    render_texto(x, y, buffer, color, escala);
    /*
        Dibujamos ese texto.
    */
}

static void dibujar_bloque(const tRender *r, int col, int filaVisible, uint8_t color)
/*
    Dibuja un bloque del Tetris en el tablero.

    static:
        Solo se usa dentro de render.c.

    r:
        datos de renderizado.

    col:
        columna del tablero.

    filaVisible:
        fila visible del tablero.

    color:
        color del bloque.
*/
{
    uint16_t x = r->tableroX + col * r->block;
    /*
        Convertimos columna del tablero a coordenada X de pantalla.

        tableroX:
            donde empieza el tablero.

        col * block:
            desplazamiento según la columna.
    */

    uint16_t y = r->tableroY + filaVisible * r->block;
    /*
        Convertimos fila visible del tablero a coordenada Y de pantalla.
    */

    render_pixel_rect(x, y, r->block, r->block, color);
    /*
        Dibujamos el bloque completo como un cuadrado.
    */

    render_pixel_rect(x, y, r->block, 1, COLOR_BLANCO);
    /*
        Dibujamos una línea superior clara.
        Esto da efecto de borde/relieve.
    */

    render_pixel_rect(x, y, 1, r->block, COLOR_BLANCO);
    /*
        Dibujamos una línea izquierda clara.
    */

    if (r->block > 3) {
    /*
        Si el bloque es suficientemente grande,
        dibujamos detalles internos y sombra.
    */

        render_pixel_rect(x + 1, y + 1, r->block - 2, r->block - 2, color);
        /*
            Rellenamos el interior del bloque.
        */

        render_pixel_rect(x + r->block - 1, y, 1, r->block, COLOR_GRIS_OSCURO);
        /*
            Borde derecho oscuro.
        */

        render_pixel_rect(x, y + r->block - 1, r->block, 1, COLOR_GRIS_OSCURO);
        /*
            Borde inferior oscuro.
        */
    }
}

static void dibujar_marco_tablero(const tRender *r)
/*
    Dibuja el marco/borde del tablero.
*/
{
    uint16_t w = TABLERO_COLUMNAS * r->block;
    /*
        Ancho total del tablero en píxeles.

        columnas * tamaño de bloque.
    */

    uint16_t h = TABLERO_FILAS_VISIBLES * r->block;
    /*
        Alto visible del tablero en píxeles.

        filas visibles * tamaño de bloque.
    */

    render_pixel_rect(r->tableroX - 2, r->tableroY - 2, w + 4, 2, COLOR_GRIS);
    /*
        Borde superior.
    */

    render_pixel_rect(r->tableroX - 2, r->tableroY + h, w + 4, 2, COLOR_GRIS);
    /*
        Borde inferior.
    */

    render_pixel_rect(r->tableroX - 2, r->tableroY - 2, 2, h + 4, COLOR_GRIS);
    /*
        Borde izquierdo.
    */

    render_pixel_rect(r->tableroX + w, r->tableroY - 2, 2, h + 4, COLOR_GRIS);
    /*
        Borde derecho.
    */
}

void render_presentacion(const tRender *r)
/*
    Dibuja la pantalla inicial del juego.
*/
{
    render_limpiar();
    /*
        Limpiamos la pantalla.
    */

    render_texto(r->ancho / 2 - 54, r->alto / 2 - 45, "TETRIS", 14, 3);
    /*
        Dibujamos el título TETRIS.

        r->ancho / 2:
            centro horizontal.

        r->alto / 2:
            centro vertical.

        Los valores restados son para centrar visualmente el texto.
    */

    render_texto(r->ancho / 2 - 82, r->alto / 2, "ENTER PARA JUGAR", COLOR_TEXTO, 1);
    /*
        Instrucción para empezar.
    */

    render_texto(r->ancho / 2 - 70, r->alto / 2 + 14, "P PARA PAUSAR", COLOR_GRIS, 1);
    /*
        Texto informativo sobre pausa.
    */

    render_texto(r->ancho / 2 - 72, r->alto / 2 + 28, "ESC PARA SALIR", COLOR_GRIS, 1);
    /*
        Texto informativo sobre salida.
    */
}

void render_pedir_nombre(const tRender *r, const char *nombre)
/*
    Dibuja la pantalla para ingresar nombre.

    r:
        datos de render.

    nombre:
        texto escrito hasta el momento.
*/
{
    char linea[64];
    /*
        Buffer auxiliar para armar el texto del nombre
        más el guion cursor.
    */

    render_limpiar();
    /*
        Limpiamos pantalla.
    */

    render_texto(r->ancho / 2 - 72, r->alto / 2 - 35, "NOMBRE JUGADOR", 14, 1);
    /*
        Título de la pantalla.
    */

    if (nombre[0] == '\0') {
    /*
        Si el primer carácter es '\0',
        significa que el nombre está vacío.
    */

        sprintf(linea, "-");
        /*
            Mostramos solo un guion como cursor.
        */

    } else {
    /*
        Si ya hay algo escrito...
    */

        sprintf(linea, "%s-", nombre);
        /*
            Armamos:
                nombre + "-"

            Ejemplo:
                AGUS-
        */
    }

    render_texto(r->ancho / 2 - 50, r->alto / 2 - 5, linea, COLOR_TEXTO, 2);
    /*
        Dibujamos el nombre con escala 2 para que se vea más grande.
    */

    render_texto(r->ancho / 2 - 84, r->alto / 2 + 35, "ENTER CONFIRMAR", COLOR_GRIS, 1);
    /*
        Instrucción para confirmar.
    */

    render_texto(r->ancho / 2 - 82, r->alto / 2 + 48, "BORRAR BACKSPACE", COLOR_GRIS, 1);
    /*
        Instrucción para borrar.
    */
}

void render_juego(const tRender *r,
                  const tTablero *tablero,
                  const tTetromino *pieza,
                  int piezaX,
                  int piezaY,
                  uint32_t puntos,
                  uint32_t lineas,
                  uint32_t piezas,
                  uint32_t velocidadMs,
                  const char *nombre)
/*
    Dibuja la pantalla completa de juego.

    Recibe todos los datos necesarios:
        - render
        - tablero fijo
        - pieza actual
        - posición de la pieza
        - puntaje
        - líneas
        - piezas fijadas
        - velocidad
        - nombre
*/
{
    int f, c, pf, pc;
    /*
        f, c:
            filas y columnas del tablero.

        pf, pc:
            filas y columnas de la pieza.
    */

    char buffer[64];
    /*
        Buffer auxiliar para textos.
    */

    render_limpiar();
    /*
        Limpiamos la pantalla antes de dibujar el frame.
    */

    dibujar_marco_tablero(r);
    /*
        Dibujamos el borde del tablero.
    */

    for (f = TABLERO_FILA_VISIBLE_INICIO; f < TABLERO_FILAS; f++) {
    /*
        Recorremos solo las filas visibles.

        TABLERO_FILA_VISIBLE_INICIO vale 2,
        porque hay 2 filas ocultas arriba.
    */

        for (c = 0; c < TABLERO_COLUMNAS; c++) {
        /*
            Recorremos columnas del tablero.
        */

            uint8_t color = tablero->celda[f][c];
            /*
                Leemos el color guardado en la celda.

                Si vale 0, está vacía.
                Si es distinto de 0, hay un bloque.
            */

            if (color != 0) {
            /*
                Si hay un bloque fijo...
            */

                dibujar_bloque(r, c, f - TABLERO_FILA_VISIBLE_INICIO, color);
                /*
                    Dibujamos el bloque.

                    f - TABLERO_FILA_VISIBLE_INICIO:
                        convierte fila real del tablero en fila visible.

                    Ejemplo:
                        fila real 2 → fila visible 0
                */
            }
        }
    }

    for (pf = 0; pf < TETRO_TAM; pf++) {
    /*
        Recorremos las filas de la pieza actual.
    */

        for (pc = 0; pc < TETRO_TAM; pc++) {
        /*
            Recorremos las columnas de la pieza actual.
        */

            if (pieza->m[pf][pc]) {
            /*
                Si en esa posición de la matriz de la pieza hay un bloque...
            */

                int tf = piezaY + pf;
                /*
                    Fila real del tablero donde se dibujaría ese bloque.
                */

                int tc = piezaX + pc;
                /*
                    Columna real del tablero donde se dibujaría ese bloque.
                */

                if (tf >= TABLERO_FILA_VISIBLE_INICIO &&
                    tf < TABLERO_FILAS &&
                    tc >= 0 &&
                    tc < TABLERO_COLUMNAS) {
                /*
                    Solo dibujamos si está dentro de la zona visible
                    y dentro de los límites horizontales.

                    Esto evita dibujar bloques en filas ocultas
                    o fuera del tablero.
                */

                    dibujar_bloque(r, tc, tf - TABLERO_FILA_VISIBLE_INICIO, pieza->color);
                    /*
                        Dibujamos el bloque de la pieza actual.
                    */
                }
            }
        }
    }

    render_texto(r->panelX, r->panelY, "JUGADOR", 14, 1);
    /*
        Título del nombre del jugador.
    */

    render_texto(r->panelX, r->panelY + 10, nombre, COLOR_TEXTO, 1);
    /*
        Nombre del jugador.
    */

    render_texto(r->panelX, r->panelY + 28, "SCORE", 14, 1);
    /*
        Título del puntaje.
    */

    render_numero(r->panelX, r->panelY + 38, (int)puntos, COLOR_TEXTO, 1);
    /*
        Dibuja el puntaje actual.
    */

    render_texto(r->panelX, r->panelY + 56, "LINES", 14, 1);
    /*
        Título de líneas.
    */

    render_numero(r->panelX, r->panelY + 66, (int)lineas, COLOR_TEXTO, 1);
    /*
        Cantidad de líneas limpiadas.
    */

    render_texto(r->panelX, r->panelY + 84, "PIEZAS", 14, 1);
    /*
        Título de piezas fijadas.
    */

    render_numero(r->panelX, r->panelY + 94, (int)piezas, COLOR_TEXTO, 1);
    /*
        Cantidad de piezas que ya cayeron.
    */

    render_texto(r->panelX, r->panelY + 112, "MS", 14, 1);
    /*
        Título de velocidad.

        MS significa milisegundos.
    */

    render_numero(r->panelX, r->panelY + 122, (int)velocidadMs, COLOR_TEXTO, 1);
    /*
        Muestra cada cuántos milisegundos cae la pieza.

        Mientras menor sea el número,
        más rápido cae la pieza.
    */

    sprintf(buffer, "Z X ROTAR");
    /*
        Armamos texto de ayuda para rotación.

        En este caso no era necesario sprintf,
        se podría haber usado directamente "Z X ROTAR".
    */

    render_texto(r->panelX, r->panelY + 145, buffer, COLOR_GRIS, 1);
    /*
        Dibujamos ayuda de controles.
    */
}

void render_pausa(const tRender *r)
/*
    Dibuja el cartel de pausa encima del juego.
*/
{
    render_texto(r->ancho / 2 - 42, r->alto / 2 - 10, "PAUSA", 14, 2);
    /*
        Dibuja la palabra PAUSA grande.
    */

    render_texto(r->ancho / 2 - 68, r->alto / 2 + 15, "P PARA VOLVER", COLOR_TEXTO, 1);
    /*
        Instrucción para volver al juego.
    */
}

void render_game_over(const tRender *r, uint32_t puntos, uint32_t record)
/*
    Dibuja la pantalla de game over.

    puntos:
        puntaje de la partida que acaba de terminar.

    record:
        mejor puntaje del jugador.
*/
{
    render_limpiar();
    /*
        Limpiamos pantalla.
    */

    render_texto(r->ancho / 2 - 72, r->alto / 2 - 45, "GAME OVER", 12, 2);
    /*
        Dibujamos GAME OVER.
    */

    render_texto(r->ancho / 2 - 50, r->alto / 2 - 10, "SCORE", 14, 1);
    /*
        Título del puntaje.
    */

    render_numero(r->ancho / 2 - 10, r->alto / 2 - 10, (int)puntos, COLOR_TEXTO, 1);
    /*
        Puntaje conseguido.
    */

    render_texto(r->ancho / 2 - 50, r->alto / 2 + 5, "RECORD", 14, 1);
    /*
        Título del récord.
    */

    render_numero(r->ancho / 2 - 4, r->alto / 2 + 5, (int)record, COLOR_TEXTO, 1);
    /*
        Mejor puntaje del jugador.
    */

    render_texto(r->ancho / 2 - 82, r->alto / 2 + 35, "ENTER JUGAR DE NUEVO", COLOR_TEXTO, 1);
    /*
        Instrucción para reiniciar partida.
    */

    render_texto(r->ancho / 2 - 58, r->alto / 2 + 50, "ESC SALIR", COLOR_GRIS, 1);
    /*
        Instrucción para salir.
    */
}
