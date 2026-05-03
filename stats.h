#ifndef STATS_H_INCLUDED
#define STATS_H_INCLUDED

#include <stdint.h>

#define STATS_ARCHIVO "stats.txt"
#define STATS_NOMBRE_MAX 16

typedef struct {
    char nombre[STATS_NOMBRE_MAX];
    uint32_t mejorPuntaje;
    uint32_t partidas;
    uint32_t lineasTotales;
} tStatsJugador;

void stats_cargar(const char *nombre, tStatsJugador *stats);
void stats_guardar_resultado(const char *nombre, uint32_t puntaje, uint32_t lineas);

#endif
