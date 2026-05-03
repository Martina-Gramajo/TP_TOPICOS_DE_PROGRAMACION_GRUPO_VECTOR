/*
Apellido(s), nombre(s): COMPLETAR, COMPLETAR
DNI: COMPLETAR
Usuario: COMPLETAR
Entrega: Si

Apellido(s), nombre(s): COMPLETAR, COMPLETAR
DNI: COMPLETAR
Usuario: COMPLETAR
Entrega: Si

Apellido(s), nombre(s): COMPLETAR, COMPLETAR
DNI: COMPLETAR
Usuario: COMPLETAR
Entrega: Si
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "GBT/gbt.h"
#include "config.h"
#include "render.h"
#include "juego.h"

int main(int argc, char *argv[])
{
    tConfig cfg;
    tJuego juego;
    char titulo[64];

    srand((unsigned)time(NULL));

    config_cargar_default(&cfg);
    config_leer_archivo(&cfg);
    config_aplicar_argumentos(&cfg, argc, argv);
    config_guardar_archivo(&cfg);

    if (gbt_iniciar() != 0) {
        fprintf(stderr, "Error al iniciar GBT: %s\n", gbt_obtener_log());
        return -1;
    }

    sprintf(titulo, "Tetris GBT %ux%u", cfg.ancho, cfg.alto);
    if (gbt_crear_ventana(titulo, cfg.ancho, cfg.alto, cfg.escala) != 0) {
        fprintf(stderr, "Error al crear ventana: %s\n", gbt_obtener_log());
        gbt_cerrar();
        return -1;
    }

    render_aplicar_paleta(cfg.paleta);
    juego_inicializar(&juego, &cfg);

    while (juego_esta_corriendo(&juego)) {
        juego_actualizar(&juego);
        juego_dibujar(&juego);
        gbt_esperar(16);
    }

    gbt_destruir_ventana();
    gbt_cerrar();
    return 0;
}
