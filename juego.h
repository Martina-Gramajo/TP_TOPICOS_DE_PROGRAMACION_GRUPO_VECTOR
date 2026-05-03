#ifndef JUEGO_H_INCLUDED
#define JUEGO_H_INCLUDED

#include <stdint.h>
#include "config.h"
#include "tablero.h"
#include "tetromino.h"
#include "render.h"
#include "stats.h"

typedef enum {
    EST_PRESENTACION = 0,
    EST_NOMBRE,
    EST_JUGANDO,
    EST_PAUSA,
    EST_GAME_OVER,
    EST_SALIR
} eEstadoJuego;

typedef struct {
    tConfig cfg;
    tRender render;
    tTablero tablero;
    tTetromino actual;
    int piezaX;
    int piezaY;
    eEstadoJuego estado;
    char nombre[STATS_NOMBRE_MAX];
    uint8_t nombreLen;
    uint32_t puntos;
    uint32_t lineas;
    uint32_t piezasFijadas;
    uint32_t velocidadCaidaMs;
    uint32_t ultimoTickCaida;
    uint32_t recordJugador;
} tJuego;

void juego_inicializar(tJuego *juego, const tConfig *cfg);
void juego_reiniciar_partida(tJuego *juego);
void juego_actualizar(tJuego *juego);
void juego_dibujar(const tJuego *juego);
uint8_t juego_esta_corriendo(const tJuego *juego);

#endif
