#include "config.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void config_aplicar_resolucion(tConfig *cfg)
{
    if (cfg->resolucion == RES_VGA) {
        cfg->ancho = 640;
        cfg->alto = 480;
        if (cfg->escala == 0) {
            cfg->escala = 1;
        }
    } else {
        cfg->ancho = 320;
        cfg->alto = 200;
        if (cfg->escala == 0) {
            cfg->escala = 3;
        }
    }
}

void config_cargar_default(tConfig *cfg)
{
    cfg->resolucion = RES_CGA;
    cfg->escala = 3;
    cfg->velocidadInicialMs = 1000;
    cfg->paleta = 0;
    config_aplicar_resolucion(cfg);
}

void config_leer_archivo(tConfig *cfg)
{
    FILE *pf = fopen(CONFIG_ARCHIVO, "rt");
    if (!pf) {
        return;
    }

    int resolucion = 0;
    unsigned escala = 0;
    unsigned velocidad = 1000;
    unsigned paleta = 0;

    if (fscanf(pf, "resolucion=%d\n", &resolucion) == 1 &&
        fscanf(pf, "escala=%u\n", &escala) == 1 &&
        fscanf(pf, "velocidad=%u\n", &velocidad) == 1 &&
        fscanf(pf, "paleta=%u\n", &paleta) == 1) {

        cfg->resolucion = resolucion == 1 ? RES_VGA : RES_CGA;
        cfg->escala = (uint8_t)escala;
        cfg->velocidadInicialMs = velocidad < 100 ? 100 : velocidad;
        cfg->paleta = (uint8_t)paleta;
        config_aplicar_resolucion(cfg);
    }

    fclose(pf);
}

void config_guardar_archivo(const tConfig *cfg)
{
    FILE *pf = fopen(CONFIG_ARCHIVO, "wt");
    if (!pf) {
        return;
    }

    fprintf(pf, "resolucion=%d\n", cfg->resolucion == RES_VGA ? 1 : 0);
    fprintf(pf, "escala=%u\n", (unsigned)cfg->escala);
    fprintf(pf, "velocidad=%u\n", (unsigned)cfg->velocidadInicialMs);
    fprintf(pf, "paleta=%u\n", (unsigned)cfg->paleta);

    fclose(pf);
}

void config_aplicar_argumentos(tConfig *cfg, int argc, char *argv[])
{
    int i;
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "CGA") == 0 || strcmp(argv[i], "cga") == 0) {
            cfg->resolucion = RES_CGA;
        } else if (strcmp(argv[i], "VGA") == 0 || strcmp(argv[i], "vga") == 0) {
            cfg->resolucion = RES_VGA;
        } else if (strncmp(argv[i], "ESCALA=", 7) == 0) {
            int escala = atoi(argv[i] + 7);
            if (escala >= 1 && escala <= 8) {
                cfg->escala = (uint8_t)escala;
            }
        } else if (strncmp(argv[i], "VEL=", 4) == 0) {
            int vel = atoi(argv[i] + 4);
            if (vel >= 100 && vel <= 3000) {
                cfg->velocidadInicialMs = (uint32_t)vel;
            }
        }
    }

    config_aplicar_resolucion(cfg);
}
