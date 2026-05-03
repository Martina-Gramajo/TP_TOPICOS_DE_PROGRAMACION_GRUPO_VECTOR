#ifndef RENDER_H_INCLUDED
#define RENDER_H_INCLUDED

#include <stdint.h>
#include "config.h"
#include "tablero.h"
#include "tetromino.h"

#define COLOR_NEGRO 0
#define COLOR_GRIS 7
#define COLOR_GRIS_OSCURO 8
#define COLOR_BLANCO 7
#define COLOR_TEXTO 11
#define COLOR_FONDO 0
#define COLOR_TRANSPARENTE 15

typedef struct {
    uint16_t ancho;
    uint16_t alto;
    uint8_t block;
    uint16_t tableroX;
    uint16_t tableroY;
    uint16_t panelX;
    uint16_t panelY;
} tRender;

void render_configurar(tRender *r, const tConfig *cfg);
void render_aplicar_paleta(uint8_t paleta);
void render_limpiar(void);
void render_pixel_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t color);
void render_texto(uint16_t x, uint16_t y, const char *texto, uint8_t color, uint8_t escala);
void render_numero(uint16_t x, uint16_t y, int numero, uint8_t color, uint8_t escala);
void render_presentacion(const tRender *r);
void render_pedir_nombre(const tRender *r, const char *nombre);
void render_juego(const tRender *r, const tTablero *tablero, const tTetromino *pieza, int piezaX, int piezaY, uint32_t puntos, uint32_t lineas, uint32_t piezas, uint32_t velocidadMs, const char *nombre);
void render_pausa(const tRender *r);
void render_game_over(const tRender *r, uint32_t puntos, uint32_t record);

#endif
