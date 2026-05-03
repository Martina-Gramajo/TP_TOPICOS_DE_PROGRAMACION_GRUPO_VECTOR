#include "juego.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif
#include "GBT/gbt.h"

static uint32_t ahora_ms(void)
{
#ifdef _WIN32
    return (uint32_t)GetTickCount();
#else
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (uint32_t)(tv.tv_sec * 1000u + tv.tv_usec / 1000u);
#endif
}

static void generar_nueva_pieza(tJuego *juego)
{
    tetromino_crear_aleatorio(&juego->actual);
    juego->piezaX = (TABLERO_COLUMNAS / 2) - 2;
    juego->piezaY = 0;
}

static void actualizar_record(tJuego *juego)
{
    tStatsJugador st;
    stats_cargar(juego->nombre, &st);
    juego->recordJugador = st.mejorPuntaje;
}

void juego_inicializar(tJuego *juego, const tConfig *cfg)
{
    memset(juego, 0, sizeof(*juego));
    juego->cfg = *cfg;
    render_configurar(&juego->render, cfg);
    strcpy(juego->nombre, "JUGADOR");
    juego->nombreLen = 7;
    juego->estado = EST_PRESENTACION;
    juego->velocidadCaidaMs = cfg->velocidadInicialMs;
}

void juego_reiniciar_partida(tJuego *juego)
{
    tablero_inicializar(&juego->tablero);
    juego->puntos = 0;
    juego->lineas = 0;
    juego->piezasFijadas = 0;
    juego->velocidadCaidaMs = juego->cfg.velocidadInicialMs;
    juego->ultimoTickCaida = ahora_ms();
    generar_nueva_pieza(juego);
    actualizar_record(juego);
    juego->estado = EST_JUGANDO;
}

static void sumar_puntos_por_lineas(tJuego *juego, uint8_t lineas)
{
    uint32_t base = 0;
    uint32_t multiplicadorVelocidad;

    if (lineas == 1) base = 100;
    else if (lineas == 2) base = 300;
    else if (lineas == 3) base = 500;
    else if (lineas >= 4) base = 800;

    multiplicadorVelocidad = juego->cfg.velocidadInicialMs / juego->velocidadCaidaMs;
    if (multiplicadorVelocidad < 1) {
        multiplicadorVelocidad = 1;
    }

    juego->puntos += base * multiplicadorVelocidad;
}

static void fijar_actual_y_continuar(tJuego *juego)
{
    uint8_t lineasBorradas;

    tablero_fijar_pieza(&juego->tablero, &juego->actual, juego->piezaX, juego->piezaY);
    juego->piezasFijadas++;

    lineasBorradas = tablero_limpiar_lineas(&juego->tablero);
    if (lineasBorradas > 0) {
        juego->lineas += lineasBorradas;
        sumar_puntos_por_lineas(juego, lineasBorradas);
    }

    if (juego->piezasFijadas % 10 == 0) {
        juego->velocidadCaidaMs = (uint32_t)(juego->velocidadCaidaMs * 97 / 100);
        if (juego->velocidadCaidaMs < 80) {
            juego->velocidadCaidaMs = 80;
        }
    }

    generar_nueva_pieza(juego);

    if (!tablero_puede_ubicar(&juego->tablero, &juego->actual, juego->piezaX, juego->piezaY) ||
        tablero_hay_bloques_en_zona_oculta(&juego->tablero)) {
        stats_guardar_resultado(juego->nombre, juego->puntos, juego->lineas);
        actualizar_record(juego);
        if (juego->puntos > juego->recordJugador) {
            juego->recordJugador = juego->puntos;
        }
        juego->estado = EST_GAME_OVER;
    }
}

static void intentar_mover(tJuego *juego, int dx, int dy, uint8_t sumaPuntoManual)
{
    if (tablero_puede_ubicar(&juego->tablero, &juego->actual, juego->piezaX + dx, juego->piezaY + dy)) {
        juego->piezaX += dx;
        juego->piezaY += dy;
        if (sumaPuntoManual && dy > 0) {
            juego->puntos += 1;
        }
    } else if (dy > 0) {
        fijar_actual_y_continuar(juego);
    }
}

static void intentar_rotar_derecha(tJuego *juego)
{
    tTetromino rotada;
    tetromino_rotar_derecha(&juego->actual, &rotada);
    if (tablero_puede_ubicar(&juego->tablero, &rotada, juego->piezaX, juego->piezaY)) {
        tetromino_copiar(&juego->actual, &rotada);
    }
}

static void intentar_rotar_izquierda(tJuego *juego)
{
    tTetromino rotada;
    tetromino_rotar_izquierda(&juego->actual, &rotada);
    if (tablero_puede_ubicar(&juego->tablero, &rotada, juego->piezaX, juego->piezaY)) {
        tetromino_copiar(&juego->actual, &rotada);
    }
}

static void actualizar_nombre(tJuego *juego, eGBT_Tecla tecla)
{
    if (tecla == GBTK_ENTER) {
        if (juego->nombreLen == 0) {
            strcpy(juego->nombre, "JUGADOR");
            juego->nombreLen = 7;
        }
        juego_reiniciar_partida(juego);
        return;
    }

    if (tecla == GBTK_RETROCESO) {
        if (juego->nombreLen > 0) {
            juego->nombreLen--;
            juego->nombre[juego->nombreLen] = '\0';
        }
        return;
    }

    if (juego->nombreLen < STATS_NOMBRE_MAX - 1) {
        char c = 0;
        if (tecla >= GBTK_a && tecla <= GBTK_z) {
            c = (char)toupper((int)tecla);
        } else if (tecla >= GBTK_0 && tecla <= GBTK_9) {
            c = (char)tecla;
        }

        if (c) {
            juego->nombre[juego->nombreLen++] = c;
            juego->nombre[juego->nombreLen] = '\0';
        }
    }
}

void juego_actualizar(tJuego *juego)
{
    eGBT_Tecla tecla;
    uint32_t t;

    gbt_procesar_entrada();
    tecla = gbt_obtener_tecla_presionada();

    if (tecla == GBTK_ESCAPE) {
        juego->estado = EST_SALIR;
        return;
    }

    switch (juego->estado) {
        case EST_PRESENTACION:
            if (tecla == GBTK_ENTER) {
                juego->estado = EST_NOMBRE;
                juego->nombre[0] = '\0';
                juego->nombreLen = 0;
            }
            break;

        case EST_NOMBRE:
            actualizar_nombre(juego, tecla);
            break;

        case EST_JUGANDO:
            if (tecla == GBTK_p) {
                juego->estado = EST_PAUSA;
                break;
            }
            if (tecla == GBTK_IZQUIERDA) {
                intentar_mover(juego, -1, 0, 0);
            } else if (tecla == GBTK_DERECHA) {
                intentar_mover(juego, 1, 0, 0);
            } else if (tecla == GBTK_ABAJO) {
                intentar_mover(juego, 0, 1, 1);
            } else if (tecla == GBTK_x) {
                intentar_rotar_derecha(juego);
            } else if (tecla == GBTK_z) {
                intentar_rotar_izquierda(juego);
            }

            t = ahora_ms();
            if (t - juego->ultimoTickCaida >= juego->velocidadCaidaMs) {
                intentar_mover(juego, 0, 1, 0);
                juego->ultimoTickCaida = t;
            }
            break;

        case EST_PAUSA:
            if (tecla == GBTK_p || tecla == GBTK_ENTER) {
                juego->estado = EST_JUGANDO;
                juego->ultimoTickCaida = ahora_ms();
            }
            break;

        case EST_GAME_OVER:
            if (tecla == GBTK_ENTER) {
                juego_reiniciar_partida(juego);
            }
            break;

        case EST_SALIR:
        default:
            break;
    }
}

void juego_dibujar(const tJuego *juego)
{
    switch (juego->estado) {
        case EST_PRESENTACION:
            render_presentacion(&juego->render);
            break;
        case EST_NOMBRE:
            render_pedir_nombre(&juego->render, juego->nombre);
            break;
        case EST_JUGANDO:
            render_juego(&juego->render, &juego->tablero, &juego->actual,
                         juego->piezaX, juego->piezaY, juego->puntos, juego->lineas,
                         juego->piezasFijadas, juego->velocidadCaidaMs, juego->nombre);
            break;
        case EST_PAUSA:
            render_juego(&juego->render, &juego->tablero, &juego->actual,
                         juego->piezaX, juego->piezaY, juego->puntos, juego->lineas,
                         juego->piezasFijadas, juego->velocidadCaidaMs, juego->nombre);
            render_pausa(&juego->render);
            break;
        case EST_GAME_OVER:
            render_game_over(&juego->render, juego->puntos, juego->recordJugador);
            break;
        default:
            render_limpiar();
            break;
    }

    gbt_volcar_backbuffer();
}

uint8_t juego_esta_corriendo(const tJuego *juego)
{
    return juego->estado != EST_SALIR;
}
