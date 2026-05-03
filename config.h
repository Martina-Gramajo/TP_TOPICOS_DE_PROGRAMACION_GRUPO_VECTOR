#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

#include <stdint.h>

#define CONFIG_ARCHIVO "config.txt"

typedef enum {
    RES_CGA = 0,
    RES_VGA = 1
} eResolucion;

typedef struct {
    eResolucion resolucion;
    uint16_t ancho;
    uint16_t alto;
    uint8_t escala;
    uint32_t velocidadInicialMs;
    uint8_t paleta;
} tConfig;

void config_cargar_default(tConfig *cfg);
void config_aplicar_resolucion(tConfig *cfg);
void config_leer_archivo(tConfig *cfg);
void config_guardar_archivo(const tConfig *cfg);
void config_aplicar_argumentos(tConfig *cfg, int argc, char *argv[]);

#endif
