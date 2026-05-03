#ifndef TETROMINO_H_INCLUDED
#define TETROMINO_H_INCLUDED

#include <stdint.h>

#define TETRO_TAM 4
#define TETRO_CANT 7

typedef enum {
    TETRO_I = 0,
    TETRO_J,
    TETRO_L,
    TETRO_O,
    TETRO_S,
    TETRO_T,
    TETRO_Z
} eTetrominoTipo;

typedef struct {
    eTetrominoTipo tipo;
    uint8_t color;
    uint8_t m[TETRO_TAM][TETRO_TAM];
} tTetromino;

void tetromino_crear(tTetromino *t, eTetrominoTipo tipo);
void tetromino_crear_aleatorio(tTetromino *t);
void tetromino_copiar(tTetromino *dest, const tTetromino *orig);
void tetromino_rotar_derecha(const tTetromino *orig, tTetromino *rotado);
void tetromino_rotar_izquierda(const tTetromino *orig, tTetromino *rotado);

#endif
