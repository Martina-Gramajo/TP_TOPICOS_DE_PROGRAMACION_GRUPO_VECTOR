#include "stats.h"
#include <stdio.h>
#include <string.h>

void stats_cargar(const char *nombre, tStatsJugador *stats)
{
    FILE *pf;
    char nom[STATS_NOMBRE_MAX];
    unsigned mejor, partidas, lineas;

    memset(stats, 0, sizeof(*stats));
    strncpy(stats->nombre, nombre, STATS_NOMBRE_MAX - 1);

    pf = fopen(STATS_ARCHIVO, "rt");
    if (!pf) {
        return;
    }

    while (fscanf(pf, "%15s %u %u %u", nom, &mejor, &partidas, &lineas) == 4) {
        if (strcmp(nom, nombre) == 0) {
            stats->mejorPuntaje = mejor;
            stats->partidas = partidas;
            stats->lineasTotales = lineas;
            break;
        }
    }

    fclose(pf);
}

void stats_guardar_resultado(const char *nombre, uint32_t puntaje, uint32_t lineas)
{
    tStatsJugador lista[64];
    int cant = 0;
    int i;
    int pos = -1;
    FILE *pf;
    char nom[STATS_NOMBRE_MAX];
    unsigned mejor, partidas, lineasArchivo;

    pf = fopen(STATS_ARCHIVO, "rt");
    if (pf) {
        while (cant < 64 && fscanf(pf, "%15s %u %u %u", nom, &mejor, &partidas, &lineasArchivo) == 4) {
            memset(&lista[cant], 0, sizeof(lista[cant]));
            strncpy(lista[cant].nombre, nom, STATS_NOMBRE_MAX - 1);
            lista[cant].mejorPuntaje = mejor;
            lista[cant].partidas = partidas;
            lista[cant].lineasTotales = lineasArchivo;
            if (strcmp(nom, nombre) == 0) {
                pos = cant;
            }
            cant++;
        }
        fclose(pf);
    }

    if (pos == -1 && cant < 64) {
        pos = cant;
        memset(&lista[cant], 0, sizeof(lista[cant]));
        strncpy(lista[cant].nombre, nombre, STATS_NOMBRE_MAX - 1);
        cant++;
    }

    if (pos != -1) {
        if (puntaje > lista[pos].mejorPuntaje) {
            lista[pos].mejorPuntaje = puntaje;
        }
        lista[pos].partidas++;
        lista[pos].lineasTotales += lineas;
    }

    pf = fopen(STATS_ARCHIVO, "wt");
    if (!pf) {
        return;
    }

    for (i = 0; i < cant; i++) {
        fprintf(pf, "%s %u %u %u\n",
                lista[i].nombre,
                (unsigned)lista[i].mejorPuntaje,
                (unsigned)lista[i].partidas,
                (unsigned)lista[i].lineasTotales);
    }

    fclose(pf);
}
