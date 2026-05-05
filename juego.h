#ifndef JUEGO_H_INCLUDED
/*
    Include guard.

    Evita que juego.h se incluya más de una vez
    durante la compilación.
*/

#define JUEGO_H_INCLUDED
/*
    Definimos el identificador del include guard.
*/

#include <stdint.h>
/*
    Incluimos stdint.h para usar tipos como:

        uint8_t
        uint32_t

    Estos tipos son enteros con tamaño fijo.
*/

#include "config.h"
/*
    Incluimos config.h porque tJuego contiene una estructura tConfig.

    tConfig guarda:
        - resolución
        - ancho
        - alto
        - escala
        - velocidad inicial
        - paleta
*/

#include "tablero.h"
/*
    Incluimos tablero.h porque tJuego contiene un tTablero.

    tTablero guarda la matriz del tablero.
*/

#include "tetromino.h"
/*
    Incluimos tetromino.h porque tJuego contiene un tTetromino.

    tTetromino representa la pieza actual que está cayendo.
*/

#include "render.h"
/*
    Incluimos render.h porque tJuego contiene un tRender.

    tRender guarda datos para dibujar:
        - ancho
        - alto
        - tamaño de bloque
        - posición del tablero
        - posición del panel
*/

#include "stats.h"
/*
    Incluimos stats.h porque tJuego usa datos de estadísticas.

    Por ejemplo:
        STATS_NOMBRE_MAX

    Ese valor define la longitud máxima del nombre del jugador.
*/

typedef enum {
    EST_PRESENTACION = 0,
    /*
        Estado inicial del juego.

        Se muestra la pantalla de presentación:
            TETRIS
            ENTER PARA JUGAR
            ESC PARA SALIR
    */

    EST_NOMBRE,
    /*
        Estado donde se pide el nombre del jugador.

        El usuario escribe su nombre usando el teclado.
    */

    EST_JUGANDO,
    /*
        Estado de partida activa.

        Acá se mueve la pieza, cae por gravedad,
        se rotan piezas, se limpian líneas, etc.
    */

    EST_PAUSA,
    /*
        Estado de pausa.

        El juego queda congelado hasta que el usuario vuelva.
    */

    EST_GAME_OVER,
    /*
        Estado cuando el jugador pierde.

        Se muestra puntaje, récord y opción de jugar de nuevo.
    */

    EST_SALIR
    /*
        Estado usado para cerrar el programa.

        Cuando el juego pasa a EST_SALIR,
        el while principal del main termina.
    */
} eEstadoJuego;
/*
    eEstadoJuego es un enum que representa en qué pantalla
    o situación está el juego.

    Esto permite que juego_actualizar y juego_dibujar
    hagan cosas diferentes según el estado.
*/

typedef struct {
    tConfig cfg;
    /*
        Guarda la configuración del juego.

        Incluye:
            - resolución
            - ancho
            - alto
            - escala
            - velocidad inicial
            - paleta
    */

    tRender render;
    /*
        Guarda datos para dibujar.

        Por ejemplo:
            - tamaño de bloque
            - dónde empieza el tablero
            - dónde está el panel lateral
    */

    tTablero tablero;
    /*
        Guarda la matriz del tablero.

        El tablero tiene:
            - 20 filas visibles
            - 2 filas ocultas
            - 10 columnas
    */

    tTetromino actual;
    /*
        Guarda la pieza actual que está cayendo.

        Contiene:
            - tipo de pieza
            - color
            - matriz 4x4
    */

    int piezaX;
    /*
        Columna actual de la pieza dentro del tablero.

        Ejemplo:
            piezaX = 3

        Significa que la pieza empieza en la columna 3.
    */

    int piezaY;
    /*
        Fila actual de la pieza dentro del tablero.

        Ejemplo:
            piezaY = 0

        Significa que la pieza está arriba.
    */

    eEstadoJuego estado;
    /*
        Guarda el estado actual del juego.

        Puede ser:
            EST_PRESENTACION
            EST_NOMBRE
            EST_JUGANDO
            EST_PAUSA
            EST_GAME_OVER
            EST_SALIR
    */

    char nombre[STATS_NOMBRE_MAX];
    /*
        Guarda el nombre del jugador.

        Es un arreglo de caracteres.

        STATS_NOMBRE_MAX vale 16,
        entonces el nombre puede tener hasta 15 caracteres útiles
        más el '\0' final.
    */

    uint8_t nombreLen;
    /*
        Guarda cuántos caracteres tiene el nombre actualmente.

        Sirve para saber dónde agregar o borrar letras.
    */

    uint32_t puntos;
    /*
        Puntaje actual de la partida.
    */

    uint32_t lineas;
    /*
        Cantidad de líneas limpiadas en la partida actual.
    */

    uint32_t piezasFijadas;
    /*
        Cantidad de piezas que ya cayeron y quedaron fijas
        en el tablero.

        Se usa para aumentar la velocidad cada 10 piezas.
    */

    uint32_t velocidadCaidaMs;
    /*
        Velocidad actual de caída de la pieza, en milisegundos.

        Ejemplo:
            1000 → baja cada 1 segundo
            970  → baja más rápido
    */

    uint32_t ultimoTickCaida;
    /*
        Guarda el último momento en que la pieza bajó automáticamente.

        Se usa para calcular si ya pasó el tiempo suficiente
        para que vuelva a bajar.
    */

    uint32_t recordJugador;
    /*
        Guarda el mejor puntaje registrado del jugador actual.
    */
} tJuego;
/*
    tJuego es la estructura principal del proyecto.

    Contiene casi todo el estado de la partida.

    Es como la "caja principal" del juego:
        configuración
        render
        tablero
        pieza
        posición
        estado
        nombre
        puntos
        líneas
        velocidad
        récord
*/

void juego_inicializar(tJuego *juego, const tConfig *cfg);
/*
    Inicializa la estructura tJuego.

    Recibe:
        juego:
            estructura que se va a modificar.

        cfg:
            configuración ya cargada.

    Deja el juego en estado de presentación.
*/

void juego_reiniciar_partida(tJuego *juego);
/*
    Reinicia una partida.

    Limpia tablero, puntos, líneas, velocidad,
    genera nueva pieza y pasa a EST_JUGANDO.
*/

void juego_actualizar(tJuego *juego);
/*
    Actualiza la lógica del juego.

    Procesa:
        - teclado
        - estados
        - movimiento
        - rotación
        - caída automática
        - pausa
        - game over
*/

void juego_dibujar(const tJuego *juego);
/*
    Dibuja el juego según el estado actual.

    Recibe const porque no debería modificar el juego,
    solo leerlo para dibujar.
*/

uint8_t juego_esta_corriendo(const tJuego *juego);
/*
    Devuelve:
        1 → si el juego debe seguir ejecutándose.
        0 → si el juego debe terminar.

    Se usa en el while principal del main.
*/

#endif
/*
    Fin del include guard.
*/
