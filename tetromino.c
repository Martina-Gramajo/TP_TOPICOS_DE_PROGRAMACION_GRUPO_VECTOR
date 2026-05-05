#include "tetromino.h"
/*
    Incluimos tetromino.h porque acá implementamos sus funciones.

    Este archivo necesita conocer:
        - tTetromino
        - eTetrominoTipo
        - TETRO_TAM
        - TETRO_CANT
*/

#include <stdlib.h>
/*
    Incluimos stdlib.h para usar rand().
*/

#include <string.h>
/*
    Incluimos string.h para usar memcpy().
*/

static const uint8_t FORMAS[TETRO_CANT][TETRO_TAM][TETRO_TAM] = {
/*
    FORMAS es un arreglo con todas las formas de las piezas.

    static:
        Significa que esta variable solo existe dentro de tetromino.c.
        No se puede usar directamente desde otros archivos.

    const:
        Significa que no vamos a modificar estas formas.
        Son datos fijos.

    Dimensiones:
        TETRO_CANT = 7 piezas.
        TETRO_TAM = 4 filas.
        TETRO_TAM = 4 columnas.

    Entonces:
        FORMAS[pieza][fila][columna]

    Cada pieza es una matriz de 4x4.

    Valores:
        0 → no hay bloque
        1 → hay bloque
*/
    /* I */
    {
        {0,0,0,0},
        {1,1,1,1},
        {0,0,0,0},
        {0,0,0,0}
    },
    /*
        Pieza I.

        Es una línea horizontal de 4 bloques.
    */

    /* J */
    {
        {1,0,0,0},
        {1,1,1,0},
        {0,0,0,0},
        {0,0,0,0}
    },
    /*
        Pieza J.
    */

    /* L */
    {
        {0,0,1,0},
        {1,1,1,0},
        {0,0,0,0},
        {0,0,0,0}
    },
    /*
        Pieza L.
    */

    /* O */
    {
        {0,1,1,0},
        {0,1,1,0},
        {0,0,0,0},
        {0,0,0,0}
    },
    /*
        Pieza O.

        Es un cuadrado de 2x2.
    */

    /* S */
    {
        {0,1,1,0},
        {1,1,0,0},
        {0,0,0,0},
        {0,0,0,0}
    },
    /*
        Pieza S.
    */

    /* T */
    {
        {0,1,0,0},
        {1,1,1,0},
        {0,0,0,0},
        {0,0,0,0}
    },
    /*
        Pieza T.
    */

    /* Z */
    {
        {1,1,0,0},
        {0,1,1,0},
        {0,0,0,0},
        {0,0,0,0}
    }
    /*
        Pieza Z.
    */
};

static const uint8_t COLORES[TETRO_CANT] = {
/*
    COLORES guarda el color de cada pieza.

    El índice de este arreglo coincide con el enum eTetrominoTipo.

    Por ejemplo:
        COLORES[TETRO_I] devuelve el color de la pieza I.
*/
    11, /* I - cian brillante */
    9,  /* J - azul brillante */
    6,  /* L - marron/naranja */
    14, /* O - amarillo */
    10, /* S - verde brillante */
    13, /* T - magenta */
    12  /* Z - rojo brillante */
};

void tetromino_crear(tTetromino *t, eTetrominoTipo tipo)
/*
    Crea una pieza específica.

    t:
        puntero a la estructura que vamos a llenar.

    tipo:
        qué pieza queremos crear.
*/
{
    int f, c;
    /*
        Variables para recorrer filas y columnas.
    */

    t->tipo = tipo;
    /*
        Guardamos el tipo de pieza.

        Ejemplo:
            TETRO_T
    */

    t->color = COLORES[tipo];
    /*
        Buscamos el color correspondiente a ese tipo de pieza.

        Si tipo es TETRO_I, usa COLORES[TETRO_I].
    */

    for (f = 0; f < TETRO_TAM; f++) {
    /*
        Recorremos las filas de la matriz 4x4.
    */

        for (c = 0; c < TETRO_TAM; c++) {
        /*
            Recorremos las columnas de la matriz 4x4.
        */

            t->m[f][c] = FORMAS[tipo][f][c];
            /*
                Copiamos la forma correspondiente.

                FORMAS[tipo][f][c]:
                    valor original de la pieza.

                t->m[f][c]:
                    matriz de la pieza que estamos creando.

                Si hay un 1, hay bloque.
                Si hay un 0, está vacío.
            */
        }
    }
}

void tetromino_crear_aleatorio(tTetromino *t)
/*
    Crea una pieza aleatoria.
*/
{
    tetromino_crear(t, (eTetrominoTipo)(rand() % TETRO_CANT));
    /*
        rand():
            genera un número pseudoaleatorio.

        rand() % TETRO_CANT:
            como TETRO_CANT vale 7, devuelve un número entre 0 y 6.

        Ese número corresponde a:
            0 → I
            1 → J
            2 → L
            3 → O
            4 → S
            5 → T
            6 → Z

        (eTetrominoTipo):
            convierte ese número al tipo enum.

        Después llamamos a tetromino_crear para cargar la pieza real.
    */
}

void tetromino_copiar(tTetromino *dest, const tTetromino *orig)
/*
    Copia una pieza completa.

    dest:
        destino, se modifica.

    orig:
        origen, no se modifica.
*/
{
    memcpy(dest, orig, sizeof(tTetromino));
    /*
        memcpy copia memoria.

        Copia desde orig hacia dest la cantidad de bytes que ocupa tTetromino.

        Esto copia:
            - tipo
            - color
            - matriz 4x4
    */
}

void tetromino_rotar_derecha(const tTetromino *orig, tTetromino *rotado)
/*
    Rota una pieza 90 grados hacia la derecha.

    No cambia la pieza original.
    Guarda el resultado en rotado.
*/
{
    int f, c;
    /*
        Variables para recorrer filas y columnas.
    */

    rotado->tipo = orig->tipo;
    /*
        La pieza rotada mantiene el mismo tipo.

        Si era una T, sigue siendo una T.
    */

    rotado->color = orig->color;
    /*
        La pieza rotada mantiene el mismo color.
    */

    if (orig->tipo == TETRO_O) {
    /*
        Si la pieza es O, no hace falta rotarla.

        La pieza O es un cuadrado.
        Al rotarla, se ve igual.
    */

        tetromino_copiar(rotado, orig);
        /*
            Copiamos la pieza original tal cual.
        */

        return;
        /*
            Salimos de la función.
        */
    }

    for (f = 0; f < TETRO_TAM; f++) {
    /*
        Recorremos filas de la matriz rotada.
    */

        for (c = 0; c < TETRO_TAM; c++) {
        /*
            Recorremos columnas de la matriz rotada.
        */

            rotado->m[f][c] = orig->m[TETRO_TAM - 1 - c][f];
            /*
                Fórmula para rotar a la derecha:

                    nueva[fila][columna] =
                        original[tamaño - 1 - columna][fila]

                Como TETRO_TAM vale 4:

                    rotado->m[f][c] = orig->m[3 - c][f]

                Esto equivale a transponer e invertir filas/columnas.
            */
        }
    }
}

void tetromino_rotar_izquierda(const tTetromino *orig, tTetromino *rotado)
/*
    Rota una pieza 90 grados hacia la izquierda.

    No modifica orig.
    Guarda el resultado en rotado.
*/
{
    int f, c;
    /*
        Variables para recorrer matriz.
    */

    rotado->tipo = orig->tipo;
    /*
        Conserva el mismo tipo.
    */

    rotado->color = orig->color;
    /*
        Conserva el mismo color.
    */

    if (orig->tipo == TETRO_O) {
    /*
        Si la pieza es O, no se rota porque se ve igual.
    */

        tetromino_copiar(rotado, orig);
        /*
            Copiamos la pieza original.
        */

        return;
        /*
            Salimos.
        */
    }

    for (f = 0; f < TETRO_TAM; f++) {
    /*
        Recorremos filas de la matriz rotada.
    */

        for (c = 0; c < TETRO_TAM; c++) {
        /*
            Recorremos columnas.
        */

            rotado->m[f][c] = orig->m[c][TETRO_TAM - 1 - f];
            /*
                Fórmula para rotar a la izquierda:

                    nueva[fila][columna] =
                        original[columna][tamaño - 1 - fila]

                Como TETRO_TAM vale 4:

                    rotado->m[f][c] = orig->m[c][3 - f]
            */
        }
    }
}
