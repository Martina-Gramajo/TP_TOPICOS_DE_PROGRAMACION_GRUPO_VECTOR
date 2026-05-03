#include "render.h"
#include <stdio.h>
#include <string.h>
#include "GBT/gbt.h"

static tGBT_ColorRGB PALETA_CLASICA[16] = {
    {0x00,0x00,0x00}, {0x00,0x00,0xAA}, {0x00,0xAA,0x00}, {0x00,0xAA,0xAA},
    {0xAA,0x00,0x00}, {0xAA,0x00,0xAA}, {0xAA,0x55,0x00}, {0xAA,0xAA,0xAA},
    {0x55,0x55,0x55}, {0x55,0x55,0xFF}, {0x55,0xFF,0x55}, {0x55,0xFF,0xFF},
    {0xFF,0x55,0x55}, {0xFF,0x55,0xFF}, {0xFF,0xFF,0x55}, {0xFF,0xFF,0xFF}
};

static tGBT_ColorRGB PALETA_NOCHE[16] = {
    {0x04,0x06,0x12}, {0x18,0x24,0x70}, {0x17,0x84,0x44}, {0x18,0xA0,0xA0},
    {0xA0,0x20,0x30}, {0x80,0x30,0x90}, {0xCC,0x7A,0x21}, {0xB0,0xB0,0xB0},
    {0x35,0x35,0x45}, {0x55,0x80,0xFF}, {0x55,0xFF,0x77}, {0x55,0xFF,0xFF},
    {0xFF,0x55,0x55}, {0xFF,0x70,0xFF}, {0xFF,0xE0,0x55}, {0xF8,0xF8,0xF8}
};

static const char *glyph(char c)
{
    switch (c) {
        case 'A': return "01110""10001""10001""11111""10001""10001""10001";
        case 'B': return "11110""10001""10001""11110""10001""10001""11110";
        case 'C': return "01111""10000""10000""10000""10000""10000""01111";
        case 'D': return "11110""10001""10001""10001""10001""10001""11110";
        case 'E': return "11111""10000""10000""11110""10000""10000""11111";
        case 'F': return "11111""10000""10000""11110""10000""10000""10000";
        case 'G': return "01111""10000""10000""10011""10001""10001""01111";
        case 'H': return "10001""10001""10001""11111""10001""10001""10001";
        case 'I': return "11111""00100""00100""00100""00100""00100""11111";
        case 'J': return "00111""00010""00010""00010""10010""10010""01100";
        case 'K': return "10001""10010""10100""11000""10100""10010""10001";
        case 'L': return "10000""10000""10000""10000""10000""10000""11111";
        case 'M': return "10001""11011""10101""10101""10001""10001""10001";
        case 'N': return "10001""11001""10101""10011""10001""10001""10001";
        case 'O': return "01110""10001""10001""10001""10001""10001""01110";
        case 'P': return "11110""10001""10001""11110""10000""10000""10000";
        case 'Q': return "01110""10001""10001""10001""10101""10010""01101";
        case 'R': return "11110""10001""10001""11110""10100""10010""10001";
        case 'S': return "01111""10000""10000""01110""00001""00001""11110";
        case 'T': return "11111""00100""00100""00100""00100""00100""00100";
        case 'U': return "10001""10001""10001""10001""10001""10001""01110";
        case 'V': return "10001""10001""10001""10001""10001""01010""00100";
        case 'W': return "10001""10001""10001""10101""10101""10101""01010";
        case 'X': return "10001""10001""01010""00100""01010""10001""10001";
        case 'Y': return "10001""10001""01010""00100""00100""00100""00100";
        case 'Z': return "11111""00001""00010""00100""01000""10000""11111";
        case '0': return "01110""10001""10011""10101""11001""10001""01110";
        case '1': return "00100""01100""00100""00100""00100""00100""01110";
        case '2': return "01110""10001""00001""00010""00100""01000""11111";
        case '3': return "11110""00001""00001""01110""00001""00001""11110";
        case '4': return "00010""00110""01010""10010""11111""00010""00010";
        case '5': return "11111""10000""10000""11110""00001""00001""11110";
        case '6': return "01110""10000""10000""11110""10001""10001""01110";
        case '7': return "11111""00001""00010""00100""01000""01000""01000";
        case '8': return "01110""10001""10001""01110""10001""10001""01110";
        case '9': return "01110""10001""10001""01111""00001""00001""01110";
        case ':': return "00000""00100""00100""00000""00100""00100""00000";
        case '-': return "00000""00000""00000""11111""00000""00000""00000";
        case '.': return "00000""00000""00000""00000""00000""01100""01100";
        default: return "00000""00000""00000""00000""00000""00000""00000";
    }
}

void render_configurar(tRender *r, const tConfig *cfg)
{
    r->ancho = cfg->ancho;
    r->alto = cfg->alto;
    r->block = cfg->alto >= 480 ? 18 : 8;
    r->tableroX = cfg->alto >= 480 ? 150 : 35;
    r->tableroY = cfg->alto >= 480 ? 60 : 20;
    r->panelX = r->tableroX + TABLERO_COLUMNAS * r->block + (cfg->alto >= 480 ? 35 : 20);
    r->panelY = r->tableroY;
}

void render_aplicar_paleta(uint8_t paleta)
{
    if (paleta == 1) {
        gbt_aplicar_paleta(PALETA_NOCHE, 16, GBT_FORMATO_888);
    } else {
        gbt_aplicar_paleta(PALETA_CLASICA, 16, GBT_FORMATO_888);
    }
}

void render_limpiar(void)
{
    gbt_borrar_backbuffer(COLOR_FONDO);
}

void render_pixel_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t color)
{
    uint16_t px, py;
    for (py = y; py < y + h; py++) {
        for (px = x; px < x + w; px++) {
            gbt_dibujar_pixel(px, py, color);
        }
    }
}

void render_texto(uint16_t x, uint16_t y, const char *texto, uint8_t color, uint8_t escala)
{
    uint16_t cursorX = x;
    int i;

    for (i = 0; texto[i] != '\0'; i++) {
        char c = texto[i];
        if (c == ' ') {
            cursorX += 4 * escala;
            continue;
        }

        const char *g = glyph(c);
        int f, col;
        for (f = 0; f < 7; f++) {
            for (col = 0; col < 5; col++) {
                if (g[f * 5 + col] == '1') {
                    render_pixel_rect(cursorX + col * escala, y + f * escala, escala, escala, color);
                }
            }
        }
        cursorX += 6 * escala;
    }
}

void render_numero(uint16_t x, uint16_t y, int numero, uint8_t color, uint8_t escala)
{
    char buffer[32];
    sprintf(buffer, "%d", numero);
    render_texto(x, y, buffer, color, escala);
}

static void dibujar_bloque(const tRender *r, int col, int filaVisible, uint8_t color)
{
    uint16_t x = r->tableroX + col * r->block;
    uint16_t y = r->tableroY + filaVisible * r->block;

    render_pixel_rect(x, y, r->block, r->block, color);
    render_pixel_rect(x, y, r->block, 1, COLOR_BLANCO);
    render_pixel_rect(x, y, 1, r->block, COLOR_BLANCO);
    if (r->block > 3) {
        render_pixel_rect(x + 1, y + 1, r->block - 2, r->block - 2, color);
        render_pixel_rect(x + r->block - 1, y, 1, r->block, COLOR_GRIS_OSCURO);
        render_pixel_rect(x, y + r->block - 1, r->block, 1, COLOR_GRIS_OSCURO);
    }
}

static void dibujar_marco_tablero(const tRender *r)
{
    uint16_t w = TABLERO_COLUMNAS * r->block;
    uint16_t h = TABLERO_FILAS_VISIBLES * r->block;
    render_pixel_rect(r->tableroX - 2, r->tableroY - 2, w + 4, 2, COLOR_GRIS);
    render_pixel_rect(r->tableroX - 2, r->tableroY + h, w + 4, 2, COLOR_GRIS);
    render_pixel_rect(r->tableroX - 2, r->tableroY - 2, 2, h + 4, COLOR_GRIS);
    render_pixel_rect(r->tableroX + w, r->tableroY - 2, 2, h + 4, COLOR_GRIS);
}

void render_presentacion(const tRender *r)
{
    render_limpiar();
    render_texto(r->ancho / 2 - 54, r->alto / 2 - 45, "TETRIS", 14, 3);
    render_texto(r->ancho / 2 - 82, r->alto / 2, "ENTER PARA JUGAR", COLOR_TEXTO, 1);
    render_texto(r->ancho / 2 - 70, r->alto / 2 + 14, "P PARA PAUSAR", COLOR_GRIS, 1);
    render_texto(r->ancho / 2 - 72, r->alto / 2 + 28, "ESC PARA SALIR", COLOR_GRIS, 1);
}

void render_pedir_nombre(const tRender *r, const char *nombre)
{
    char linea[64];

    render_limpiar();

    render_texto(r->ancho / 2 - 72, r->alto / 2 - 35, "NOMBRE JUGADOR", 14, 1);

    if (nombre[0] == '\0') {
        sprintf(linea, "-");
    } else {
        sprintf(linea, "%s-", nombre);
    }

    render_texto(r->ancho / 2 - 50, r->alto / 2 - 5, linea, COLOR_TEXTO, 2);

    render_texto(r->ancho / 2 - 84, r->alto / 2 + 35, "ENTER CONFIRMAR", COLOR_GRIS, 1);
    render_texto(r->ancho / 2 - 82, r->alto / 2 + 48, "BORRAR BACKSPACE", COLOR_GRIS, 1);
}

void render_juego(const tRender *r, const tTablero *tablero, const tTetromino *pieza, int piezaX, int piezaY, uint32_t puntos, uint32_t lineas, uint32_t piezas, uint32_t velocidadMs, const char *nombre)
{
    int f, c, pf, pc;
    char buffer[64];

    render_limpiar();
    dibujar_marco_tablero(r);

    for (f = TABLERO_FILA_VISIBLE_INICIO; f < TABLERO_FILAS; f++) {
        for (c = 0; c < TABLERO_COLUMNAS; c++) {
            uint8_t color = tablero->celda[f][c];
            if (color != 0) {
                dibujar_bloque(r, c, f - TABLERO_FILA_VISIBLE_INICIO, color);
            }
        }
    }

    for (pf = 0; pf < TETRO_TAM; pf++) {
        for (pc = 0; pc < TETRO_TAM; pc++) {
            if (pieza->m[pf][pc]) {
                int tf = piezaY + pf;
                int tc = piezaX + pc;
                if (tf >= TABLERO_FILA_VISIBLE_INICIO && tf < TABLERO_FILAS && tc >= 0 && tc < TABLERO_COLUMNAS) {
                    dibujar_bloque(r, tc, tf - TABLERO_FILA_VISIBLE_INICIO, pieza->color);
                }
            }
        }
    }

    render_texto(r->panelX, r->panelY, "JUGADOR", 14, 1);
    render_texto(r->panelX, r->panelY + 10, nombre, COLOR_TEXTO, 1);

    render_texto(r->panelX, r->panelY + 28, "SCORE", 14, 1);
    render_numero(r->panelX, r->panelY + 38, (int)puntos, COLOR_TEXTO, 1);

    render_texto(r->panelX, r->panelY + 56, "LINES", 14, 1);
    render_numero(r->panelX, r->panelY + 66, (int)lineas, COLOR_TEXTO, 1);

    render_texto(r->panelX, r->panelY + 84, "PIEZAS", 14, 1);
    render_numero(r->panelX, r->panelY + 94, (int)piezas, COLOR_TEXTO, 1);

    render_texto(r->panelX, r->panelY + 112, "MS", 14, 1);
    render_numero(r->panelX, r->panelY + 122, (int)velocidadMs, COLOR_TEXTO, 1);

    sprintf(buffer, "Z X ROTAR");
    render_texto(r->panelX, r->panelY + 145, buffer, COLOR_GRIS, 1);
}

void render_pausa(const tRender *r)
{
    render_texto(r->ancho / 2 - 42, r->alto / 2 - 10, "PAUSA", 14, 2);
    render_texto(r->ancho / 2 - 68, r->alto / 2 + 15, "P PARA VOLVER", COLOR_TEXTO, 1);
}

void render_game_over(const tRender *r, uint32_t puntos, uint32_t record)
{
    render_limpiar();
    render_texto(r->ancho / 2 - 72, r->alto / 2 - 45, "GAME OVER", 12, 2);
    render_texto(r->ancho / 2 - 50, r->alto / 2 - 10, "SCORE", 14, 1);
    render_numero(r->ancho / 2 - 10, r->alto / 2 - 10, (int)puntos, COLOR_TEXTO, 1);
    render_texto(r->ancho / 2 - 50, r->alto / 2 + 5, "RECORD", 14, 1);
    render_numero(r->ancho / 2 - 4, r->alto / 2 + 5, (int)record, COLOR_TEXTO, 1);
    render_texto(r->ancho / 2 - 82, r->alto / 2 + 35, "ENTER JUGAR DE NUEVO", COLOR_TEXTO, 1);
    render_texto(r->ancho / 2 - 58, r->alto / 2 + 50, "ESC SALIR", COLOR_GRIS, 1);
}
