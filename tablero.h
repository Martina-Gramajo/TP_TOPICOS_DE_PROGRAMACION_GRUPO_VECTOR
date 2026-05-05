#ifndef TABLERO_H_INCLUDED
/*
    Include guard.
*/

#define TABLERO_H_INCLUDED
/*
    Definimos el include guard.
*/

#include <stdint.h>
/*
    Incluimos stdint.h para usar uint8_t.
*/

#include "tetromino.h"
/*
    Incluimos tetromino.h porque el tablero necesita conocer tTetromino.

    Por ejemplo, tablero_puede_ubicar recibe una pieza.
*/

#define TABLERO_COLUMNAS 10
/*
    Cantidad de columnas visibles del tablero.

    El Tetris clásico usa 10 columnas.
*/

#define TABLERO_FILAS_VISIBLES 20
/*
    Cantidad de filas visibles del tablero.

    El Tetris clásico usa 20 filas visibles.
*/

#define TABLERO_FILAS_OCULTAS 2
/*
    Cantidad de filas ocultas arriba del tablero.

    Sirven para que la pieza aparezca desde arriba,
    como en el Tetris clásico.
*/

#define TABLERO_FILAS (TABLERO_FILAS_VISIBLES + TABLERO_FILAS_OCULTAS)
/*
    Cantidad total de filas.

    20 visibles + 2 ocultas = 22 filas totales.
*/

#define TABLERO_FILA_VISIBLE_INICIO TABLERO_FILAS_OCULTAS
/*
    Indica desde qué fila empieza la parte visible.

    Como hay 2 filas ocultas:
        la parte visible empieza en la fila 2.
*/

typedef struct {
    uint8_t celda[TABLERO_FILAS][TABLERO_COLUMNAS];
    /*
        Matriz del tablero.

        Dimensiones:
            TABLERO_FILAS x TABLERO_COLUMNAS
            22 x 10

        Cada celda representa un casillero del tablero.

        Valor:
            0 → celda vacía
            distinto de 0 → hay un bloque fijo con ese color
    */
} tTablero;
/*
    tTablero representa el tablero completo del Tetris.
*/

void tablero_inicializar(tTablero *tablero);
/*
    Limpia el tablero poniendo todas las celdas en 0.
*/

uint8_t tablero_puede_ubicar(const tTablero *tablero, const tTetromino *pieza, int x, int y);
/*
    Revisa si una pieza puede estar en una posición determinada.

    Devuelve:
        1 → sí puede ubicarse
        0 → no puede ubicarse

    No modifica ni el tablero ni la pieza.
*/

void tablero_fijar_pieza(tTablero *tablero, const tTetromino *pieza, int x, int y);
/*
    Copia la pieza actual al tablero.

    Se usa cuando la pieza ya tocó el piso o chocó con otro bloque.
*/

uint8_t tablero_limpiar_lineas(tTablero *tablero);
/*
    Busca filas completas, las borra y baja las filas superiores.

    Devuelve la cantidad de líneas eliminadas.
*/

uint8_t tablero_hay_bloques_en_zona_oculta(const tTablero *tablero);
/*
    Revisa si hay bloques en las filas ocultas.

    Si hay, significa que el jugador perdió.
*/

#endif
/*
    Fin del include guard.
*/
