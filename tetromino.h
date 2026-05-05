#ifndef TETROMINO_H_INCLUDED
/*
    Include guard para evitar inclusiones repetidas.
*/

#define TETROMINO_H_INCLUDED
/*
    Definimos el identificador del include guard.
*/

#include <stdint.h>
/*
    Incluimos stdint.h para poder usar uint8_t.
*/

#define TETRO_TAM 4
/*
    Define el tamaño de la matriz de cada tetromino.

    Cada pieza se guarda en una matriz de 4x4.

    ¿Por qué 4x4?
        Porque todas las piezas del Tetris entran cómodamente
        en una grilla de 4 por 4.
*/

#define TETRO_CANT 7
/*
    Cantidad de tetrominos originales del Tetris.

    Son:
        I, J, L, O, S, T, Z
*/

typedef enum {
    TETRO_I = 0,
    /*
        Pieza I.
    */

    TETRO_J,
    /*
        Pieza J.
    */

    TETRO_L,
    /*
        Pieza L.
    */

    TETRO_O,
    /*
        Pieza O, el cuadrado.
    */

    TETRO_S,
    /*
        Pieza S.
    */

    TETRO_T,
    /*
        Pieza T.
    */

    TETRO_Z
    /*
        Pieza Z.
    */
} eTetrominoTipo;
/*
    Este enum representa el tipo de pieza.

    Internamente:
        TETRO_I vale 0
        TETRO_J vale 1
        TETRO_L vale 2
        ...
*/

typedef struct {
    eTetrominoTipo tipo;
    /*
        Guarda qué tipo de pieza es:
            I, J, L, O, S, T o Z.
    */

    uint8_t color;
    /*
        Guarda el color con el que se dibuja la pieza.

        Es un índice de la paleta de colores.
    */

    uint8_t m[TETRO_TAM][TETRO_TAM];
    /*
        Matriz 4x4 que representa la forma de la pieza.

        Cada posición puede tener:
            0 → no hay bloque
            1 → hay bloque

        Ejemplo de una T:

            0 1 0 0
            1 1 1 0
            0 0 0 0
            0 0 0 0
    */
} tTetromino;
/*
    tTetromino representa una pieza del Tetris.

    Tiene:
        - tipo
        - color
        - matriz 4x4
*/

void tetromino_crear(tTetromino *t, eTetrominoTipo tipo);
/*
    Crea una pieza específica.

    Recibe:
        t:
            puntero a la pieza que queremos cargar.

        tipo:
            qué pieza queremos crear.
*/

void tetromino_crear_aleatorio(tTetromino *t);
/*
    Crea una pieza aleatoria.

    Usa rand() para elegir entre las 7 piezas.
*/

void tetromino_copiar(tTetromino *dest, const tTetromino *orig);
/*
    Copia una pieza completa desde orig hacia dest.

    dest:
        pieza destino, se modifica.

    orig:
        pieza origen, no se modifica porque es const.
*/

void tetromino_rotar_derecha(const tTetromino *orig, tTetromino *rotado);
/*
    Crea una versión rotada hacia la derecha.

    No modifica orig.
    Guarda el resultado en rotado.
*/

void tetromino_rotar_izquierda(const tTetromino *orig, tTetromino *rotado);
/*
    Crea una versión rotada hacia la izquierda.

    No modifica orig.
    Guarda el resultado en rotado.
*/

#endif
/*
    Fin del include guard.
*/
