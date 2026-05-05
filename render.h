#ifndef RENDER_H_INCLUDED
/*
    Include guard.

    Evita que render.h se incluya más de una vez
    durante la compilación.
*/

#define RENDER_H_INCLUDED
/*
    Definimos el identificador del include guard.
*/

#include <stdint.h>
/*
    Incluimos stdint.h para poder usar tipos como:

        uint8_t
        uint16_t
        uint32_t

    Estos tipos tienen tamaño fijo.
*/

#include "config.h"
/*
    Incluimos config.h porque render_configurar recibe una tConfig.

    La configuración le dice al render:
        - ancho de ventana
        - alto de ventana
        - resolución
        - escala
        - paleta
*/

#include "tablero.h"
/*
    Incluimos tablero.h porque render_juego necesita dibujar el tablero.

    El tablero tiene la matriz con los bloques ya fijados.
*/

#include "tetromino.h"
/*
    Incluimos tetromino.h porque render_juego también dibuja
    la pieza actual que está cayendo.
*/

#define COLOR_NEGRO 0
/*
    Índice de color negro en la paleta.
*/

#define COLOR_GRIS 7
/*
    Índice de color gris claro.

    Se usa para bordes, textos secundarios, etc.
*/

#define COLOR_GRIS_OSCURO 8
/*
    Índice de color gris oscuro.

    Se usa para sombras de bloques.
*/

#define COLOR_BLANCO 7
/*
    Color usado como blanco visual.

    En este proyecto quedó apuntando al gris claro.

    Importante:
        No usamos 15 como blanco porque en GBT el índice 15
        puede comportarse como transparente.
*/

#define COLOR_TEXTO 11
/*
    Color principal del texto.

    Antes estaba en 15 y el texto no se veía,
    porque 15 estaba funcionando como transparente.

    Por eso se cambió a 11.
*/

#define COLOR_FONDO 0
/*
    Color de fondo de la pantalla.

    0 = negro.
*/

#define COLOR_TRANSPARENTE 15
/*
    Color reservado como transparente.

    En GBT, el índice 15 suele ser transparente.
*/

typedef struct {
    uint16_t ancho;
    /*
        Ancho lógico de la ventana.

        Ejemplo:
            CGA → 320
            VGA → 640
    */

    uint16_t alto;
    /*
        Alto lógico de la ventana.

        Ejemplo:
            CGA → 200
            VGA → 480
    */

    uint8_t block;
    /*
        Tamaño en píxeles de cada bloque del Tetris.

        Ejemplo:
            En CGA puede valer 8.
            En VGA puede valer 18.

        Si block = 8:
            cada celda del tablero se dibuja como un cuadrado de 8x8 píxeles.
    */

    uint16_t tableroX;
    /*
        Posición X donde empieza a dibujarse el tablero en pantalla.
    */

    uint16_t tableroY;
    /*
        Posición Y donde empieza a dibujarse el tablero en pantalla.
    */

    uint16_t panelX;
    /*
        Posición X donde empieza el panel lateral.

        En el panel se muestran:
            - jugador
            - score
            - líneas
            - piezas
            - velocidad
    */

    uint16_t panelY;
    /*
        Posición Y donde empieza el panel lateral.
    */
} tRender;
/*
    tRender guarda datos necesarios para dibujar.

    No guarda reglas del juego.
    No sabe si una pieza puede moverse o no.

    Solo guarda medidas y posiciones para representar el juego en pantalla.
*/

void render_configurar(tRender *r, const tConfig *cfg);
/*
    Configura la estructura tRender usando la configuración.

    Calcula:
        - ancho
        - alto
        - tamaño de bloque
        - posición del tablero
        - posición del panel
*/

void render_aplicar_paleta(uint8_t paleta);
/*
    Aplica la paleta de colores elegida.

    paleta = 0 → paleta clásica
    paleta = 1 → paleta noche
*/

void render_limpiar(void);
/*
    Limpia el backbuffer usando el color de fondo.

    Es decir, borra lo dibujado antes.
*/

void render_pixel_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t color);
/*
    Dibuja un rectángulo de píxeles.

    Parámetros:
        x, y:
            posición inicial.

        w:
            ancho del rectángulo.

        h:
            alto del rectángulo.

        color:
            índice de color.
*/

void render_texto(uint16_t x, uint16_t y, const char *texto, uint8_t color, uint8_t escala);
/*
    Dibuja texto en pantalla usando una fuente propia.

    Parámetros:
        x, y:
            posición donde empieza el texto.

        texto:
            texto a dibujar.

        color:
            color del texto.

        escala:
            tamaño del texto.
*/

void render_numero(uint16_t x, uint16_t y, int numero, uint8_t color, uint8_t escala);
/*
    Dibuja un número.

    Internamente convierte el número a texto
    y llama a render_texto.
*/

void render_presentacion(const tRender *r);
/*
    Dibuja la pantalla inicial.
*/

void render_pedir_nombre(const tRender *r, const char *nombre);
/*
    Dibuja la pantalla donde se pide el nombre del jugador.
*/

void render_juego(const tRender *r,
                  const tTablero *tablero,
                  const tTetromino *pieza,
                  int piezaX,
                  int piezaY,
                  uint32_t puntos,
                  uint32_t lineas,
                  uint32_t piezas,
                  uint32_t velocidadMs,
                  const char *nombre);
/*
    Dibuja la partida completa.

    Recibe:
        render
        tablero
        pieza actual
        posición de la pieza
        puntos
        líneas
        piezas fijadas
        velocidad
        nombre del jugador
*/

void render_pausa(const tRender *r);
/*
    Dibuja el cartel de pausa.
*/

void render_game_over(const tRender *r, uint32_t puntos, uint32_t record);
/*
    Dibuja la pantalla de game over.

    Muestra:
        - puntaje
        - récord
        - opción para jugar de nuevo
*/

#endif
/*
    Fin del include guard.
*/
