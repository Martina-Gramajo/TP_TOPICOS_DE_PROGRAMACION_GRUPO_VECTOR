#ifndef TABLERO_H_INCLUDED
#define TABLERO_H_INCLUDED

#include <stdint.h>
#include "tetromino.h"

#define TABLERO_COLUMNAS 10
#define TABLERO_FILAS_VISIBLES 20
#define TABLERO_FILAS_OCULTAS 2
#define TABLERO_FILAS (TABLERO_FILAS_VISIBLES + TABLERO_FILAS_OCULTAS)
#define TABLERO_FILA_VISIBLE_INICIO TABLERO_FILAS_OCULTAS

typedef struct {
    uint8_t celda[TABLERO_FILAS][TABLERO_COLUMNAS];
} tTablero;

void tablero_inicializar(tTablero *tablero);
uint8_t tablero_puede_ubicar(const tTablero *tablero, const tTetromino *pieza, int x, int y);
void tablero_fijar_pieza(tTablero *tablero, const tTetromino *pieza, int x, int y);
uint8_t tablero_limpiar_lineas(tTablero *tablero);
uint8_t tablero_hay_bloques_en_zona_oculta(const tTablero *tablero);

#endif
