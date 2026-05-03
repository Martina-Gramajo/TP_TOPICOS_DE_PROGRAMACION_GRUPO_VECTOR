#include "tablero.h"
#include <string.h>

void tablero_inicializar(tTablero *tablero)
{
    memset(tablero->celda, 0, sizeof(tablero->celda));
}

uint8_t tablero_puede_ubicar(const tTablero *tablero, const tTetromino *pieza, int x, int y)
{
    int pf, pc;

    for (pf = 0; pf < TETRO_TAM; pf++) {
        for (pc = 0; pc < TETRO_TAM; pc++) {
            if (pieza->m[pf][pc]) {
                int tf = y + pf;
                int tc = x + pc;

                if (tc < 0 || tc >= TABLERO_COLUMNAS) {
                    return 0;
                }

                if (tf >= TABLERO_FILAS) {
                    return 0;
                }

                if (tf >= 0 && tablero->celda[tf][tc] != 0) {
                    return 0;
                }
            }
        }
    }

    return 1;
}

void tablero_fijar_pieza(tTablero *tablero, const tTetromino *pieza, int x, int y)
{
    int pf, pc;

    for (pf = 0; pf < TETRO_TAM; pf++) {
        for (pc = 0; pc < TETRO_TAM; pc++) {
            if (pieza->m[pf][pc]) {
                int tf = y + pf;
                int tc = x + pc;

                if (tf >= 0 && tf < TABLERO_FILAS && tc >= 0 && tc < TABLERO_COLUMNAS) {
                    tablero->celda[tf][tc] = pieza->color;
                }
            }
        }
    }
}

static uint8_t fila_completa(const tTablero *tablero, int fila)
{
    int c;
    for (c = 0; c < TABLERO_COLUMNAS; c++) {
        if (tablero->celda[fila][c] == 0) {
            return 0;
        }
    }
    return 1;
}

static void bajar_filas(tTablero *tablero, int filaEliminada)
{
    int f;
    for (f = filaEliminada; f > 0; f--) {
        memcpy(tablero->celda[f], tablero->celda[f - 1], sizeof(tablero->celda[f]));
    }
    memset(tablero->celda[0], 0, sizeof(tablero->celda[0]));
}

uint8_t tablero_limpiar_lineas(tTablero *tablero)
{
    int f;
    uint8_t lineas = 0;

    for (f = TABLERO_FILAS - 1; f >= 0; f--) {
        if (fila_completa(tablero, f)) {
            bajar_filas(tablero, f);
            lineas++;
            f++; /* Se revisa la misma fila otra vez porque bajaron nuevas celdas. */
        }
    }

    return lineas;
}

uint8_t tablero_hay_bloques_en_zona_oculta(const tTablero *tablero)
{
    int f, c;
    for (f = 0; f < TABLERO_FILAS_OCULTAS; f++) {
        for (c = 0; c < TABLERO_COLUMNAS; c++) {
            if (tablero->celda[f][c] != 0) {
                return 1;
            }
        }
    }
    return 0;
}
