#ifndef STATS_H_INCLUDED
/*
    Include guard.

    Sirve para evitar que stats.h se incluya más de una vez
    durante la compilación.
*/

#define STATS_H_INCLUDED
/*
    Definimos el identificador del include guard.
*/

#include <stdint.h>
/*
    Incluimos stdint.h para poder usar tipos enteros
    con tamaño fijo.

    En este archivo usamos:
        uint32_t

    uint32_t:
        entero sin signo de 32 bits.
*/

#define STATS_ARCHIVO "stats.txt"
/*
    Nombre del archivo donde se guardan las estadísticas.

    En este proyecto, las estadísticas se guardan en:
        stats.txt

    Como está en un #define, si mañana queremos cambiar
    el nombre del archivo, lo cambiamos acá y no en todo el código.
*/

#define STATS_NOMBRE_MAX 16
/*
    Cantidad máxima de caracteres para el nombre del jugador.

    Ojo:
        En C, los strings necesitan un carácter final '\0'.

    Entonces, si STATS_NOMBRE_MAX vale 16,
    el nombre puede tener hasta 15 caracteres útiles
    más el '\0' final.

    Ejemplo:
        "JUGADOR" ocupa 7 letras + '\0'.
*/

typedef struct {
    char nombre[STATS_NOMBRE_MAX];
    /*
        Guarda el nombre del jugador.

        Es un arreglo de caracteres.

        Ejemplo:
            "AGUS"
            "JUGADOR"
            "MARTINA"
    */

    uint32_t mejorPuntaje;
    /*
        Guarda el mejor puntaje histórico de ese jugador.

        Ejemplo:
            12500
    */

    uint32_t partidas;
    /*
        Guarda cuántas partidas jugó ese jugador.
    */

    uint32_t lineasTotales;
    /*
        Guarda cuántas líneas limpió ese jugador en total,
        sumando todas sus partidas.
    */

} tStatsJugador;
/*
    tStatsJugador representa las estadísticas de un jugador.

    Agrupa:
        - nombre
        - mejor puntaje
        - partidas jugadas
        - líneas totales

    Se usa para leer y escribir datos en stats.txt.
*/

void stats_cargar(const char *nombre, tStatsJugador *stats);
/*
    Carga las estadísticas de un jugador desde stats.txt.

    Parámetros:
        nombre:
            nombre del jugador que queremos buscar.

        stats:
            estructura donde vamos a guardar el resultado.

    Si encuentra al jugador:
        carga su mejor puntaje, partidas y líneas.

    Si no lo encuentra:
        deja las estadísticas en cero.
*/

void stats_guardar_resultado(const char *nombre, uint32_t puntaje, uint32_t lineas);
/*
    Guarda el resultado de una partida.

    Parámetros:
        nombre:
            jugador que terminó la partida.

        puntaje:
            puntaje conseguido en esa partida.

        lineas:
            líneas limpiadas en esa partida.

    Esta función:
        - lee el archivo stats.txt
        - busca al jugador
        - si no existe, lo agrega
        - actualiza récord si corresponde
        - suma una partida
        - suma líneas
        - vuelve a escribir stats.txt
*/

#endif
/*
    Fin del include guard.
*/
