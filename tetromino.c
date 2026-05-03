#include "tetromino.h"
#include <stdlib.h>
#include <string.h>

static const uint8_t FORMAS[TETRO_CANT][TETRO_TAM][TETRO_TAM] = {
    /* I */
    {
        {0,0,0,0},
        {1,1,1,1},
        {0,0,0,0},
        {0,0,0,0}
    },
    /* J */
    {
        {1,0,0,0},
        {1,1,1,0},
        {0,0,0,0},
        {0,0,0,0}
    },
    /* L */
    {
        {0,0,1,0},
        {1,1,1,0},
        {0,0,0,0},
        {0,0,0,0}
    },
    /* O */
    {
        {0,1,1,0},
        {0,1,1,0},
        {0,0,0,0},
        {0,0,0,0}
    },
    /* S */
    {
        {0,1,1,0},
        {1,1,0,0},
        {0,0,0,0},
        {0,0,0,0}
    },
    /* T */
    {
        {0,1,0,0},
        {1,1,1,0},
        {0,0,0,0},
        {0,0,0,0}
    },
    /* Z */
    {
        {1,1,0,0},
        {0,1,1,0},
        {0,0,0,0},
        {0,0,0,0}
    }
};

static const uint8_t COLORES[TETRO_CANT] = {
    11, /* I - cian brillante */
    9,  /* J - azul brillante */
    6,  /* L - marron/naranja */
    14, /* O - amarillo */
    10, /* S - verde brillante */
    13, /* T - magenta */
    12  /* Z - rojo brillante */
};

void tetromino_crear(tTetromino *t, eTetrominoTipo tipo)
{
    int f, c;
    t->tipo = tipo;
    t->color = COLORES[tipo];

    for (f = 0; f < TETRO_TAM; f++) {
        for (c = 0; c < TETRO_TAM; c++) {
            t->m[f][c] = FORMAS[tipo][f][c];
        }
    }
}

void tetromino_crear_aleatorio(tTetromino *t)
{
    tetromino_crear(t, (eTetrominoTipo)(rand() % TETRO_CANT));
}

void tetromino_copiar(tTetromino *dest, const tTetromino *orig)
{
    memcpy(dest, orig, sizeof(tTetromino));
}

void tetromino_rotar_derecha(const tTetromino *orig, tTetromino *rotado)
{
    int f, c;
    rotado->tipo = orig->tipo;
    rotado->color = orig->color;

    if (orig->tipo == TETRO_O) {
        tetromino_copiar(rotado, orig);
        return;
    }

    for (f = 0; f < TETRO_TAM; f++) {
        for (c = 0; c < TETRO_TAM; c++) {
            rotado->m[f][c] = orig->m[TETRO_TAM - 1 - c][f];
        }
    }
}

void tetromino_rotar_izquierda(const tTetromino *orig, tTetromino *rotado)
{
    int f, c;
    rotado->tipo = orig->tipo;
    rotado->color = orig->color;

    if (orig->tipo == TETRO_O) {
        tetromino_copiar(rotado, orig);
        return;
    }

    for (f = 0; f < TETRO_TAM; f++) {
        for (c = 0; c < TETRO_TAM; c++) {
            rotado->m[f][c] = orig->m[c][TETRO_TAM - 1 - f];
        }
    }
}
