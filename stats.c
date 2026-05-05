#include "stats.h"
/*
    Incluimos stats.h porque acá implementamos
    las funciones declaradas ahí.

    Este archivo necesita conocer:
        - tStatsJugador
        - STATS_ARCHIVO
        - STATS_NOMBRE_MAX
*/

#include <stdio.h>
/*
    Incluimos stdio.h para trabajar con archivos.

    En este archivo usamos:
        FILE
        fopen
        fscanf
        fprintf
        fclose
*/

#include <string.h>
/*
    Incluimos string.h para trabajar con strings y memoria.

    En este archivo usamos:
        memset
        strncpy
        strcmp
*/

void stats_cargar(const char *nombre, tStatsJugador *stats)
/*
    Esta función busca las estadísticas de un jugador
    dentro de stats.txt.

    Parámetros:
        nombre:
            nombre del jugador que queremos cargar.

        stats:
            estructura donde vamos a guardar las estadísticas.

    Modifica:
        *stats

    Si encuentra al jugador:
        carga sus datos.

    Si no lo encuentra:
        deja mejorPuntaje, partidas y lineasTotales en 0.
*/
{
    FILE *pf;
    /*
        Puntero a archivo.

        Lo vamos a usar para abrir y leer stats.txt.
    */

    char nom[STATS_NOMBRE_MAX];
    /*
        Variable auxiliar para leer el nombre de cada jugador
        desde el archivo.

        Cada línea del archivo empieza con un nombre.
    */

    unsigned mejor, partidas, lineas;
    /*
        Variables auxiliares para leer los números del archivo.

        mejor:
            mejor puntaje guardado.

        partidas:
            cantidad de partidas jugadas.

        lineas:
            líneas totales acumuladas.
    */

    memset(stats, 0, sizeof(*stats));
    /*
        Limpiamos la estructura stats completa.

        memset:
            llena memoria con un valor.

        stats:
            puntero a la estructura que queremos limpiar.

        0:
            valor con el que llenamos.

        sizeof(*stats):
            tamaño completo de la estructura apuntada.

        Resultado:
            mejorPuntaje = 0
            partidas = 0
            lineasTotales = 0
            nombre queda vacío inicialmente
    */

    strncpy(stats->nombre, nombre, STATS_NOMBRE_MAX - 1);
    /*
        Copiamos el nombre recibido dentro de stats->nombre.

        strncpy:
            copia texto con un límite máximo de caracteres.

        STATS_NOMBRE_MAX - 1:
            dejamos lugar para el '\0' final.

        Esto es más seguro que strcpy cuando hay límites de tamaño.
    */

    pf = fopen(STATS_ARCHIVO, "rt");
    /*
        Abrimos stats.txt para lectura en modo texto.

        STATS_ARCHIVO:
            es "stats.txt"

        "rt":
            r = read, leer
            t = text, texto
    */

    if (!pf) {
    /*
        Si pf es NULL, significa que el archivo no se pudo abrir.

        Puede pasar porque:
            - stats.txt no existe todavía
            - no hay permisos
            - hubo un error de lectura

        Si no existe, no es grave:
        significa que todavía no hay estadísticas guardadas.
    */

        return;
        /*
            Salimos de la función.

            stats ya quedó inicializado en cero.
        */
    }

    while (fscanf(pf, "%15s %u %u %u", nom, &mejor, &partidas, &lineas) == 4) {
    /*
        Leemos el archivo línea por línea.

        Formato esperado de cada línea:

            NOMBRE MEJOR PARTIDAS LINEAS

        Ejemplo:
            JUGADOR 863 1 6

        fscanf:
            intenta leer datos desde el archivo.

        "%15s":
            lee un texto de máximo 15 caracteres.
            Esto coincide con STATS_NOMBRE_MAX - 1.

        %u:
            lee un unsigned.

        &mejor, &partidas, &lineas:
            pasamos direcciones de memoria para que fscanf escriba ahí.

        == 4:
            significa que logró leer correctamente 4 datos.

        Mientras pueda leer 4 datos, sigue recorriendo el archivo.
    */

        if (strcmp(nom, nombre) == 0) {
        /*
            Comparamos el nombre leído del archivo con el nombre buscado.

            strcmp devuelve 0 si los textos son iguales.

            Entonces:
                strcmp(nom, nombre) == 0

            significa:
                "encontramos el jugador".
        */

            stats->mejorPuntaje = mejor;
            /*
                Guardamos en la estructura el mejor puntaje leído.
            */

            stats->partidas = partidas;
            /*
                Guardamos la cantidad de partidas.
            */

            stats->lineasTotales = lineas;
            /*
                Guardamos las líneas totales.
            */

            break;
            /*
                Como ya encontramos al jugador,
                no hace falta seguir leyendo el archivo.
            */
        }
    }

    fclose(pf);
    /*
        Cerramos el archivo.

        Siempre que abrimos con fopen,
        debemos cerrar con fclose.
    */
}

void stats_guardar_resultado(const char *nombre, uint32_t puntaje, uint32_t lineas)
/*
    Esta función guarda el resultado de una partida.

    Parámetros:
        nombre:
            jugador que terminó la partida.

        puntaje:
            puntaje obtenido en esta partida.

        lineas:
            líneas limpiadas en esta partida.

    Objetivo:
        actualizar stats.txt.
*/
{
    tStatsJugador lista[64];
    /*
        Creamos un arreglo de estadísticas para hasta 64 jugadores.

        Cada posición de lista guarda un tStatsJugador.

        Ejemplo:
            lista[0] → estadísticas de JUGADOR
            lista[1] → estadísticas de AGUS
            lista[2] → estadísticas de MARTINA

        Es una solución simple:
            leemos todo el archivo en memoria,
            modificamos lo necesario,
            y después reescribimos el archivo completo.
    */

    int cant = 0;
    /*
        Cantidad de jugadores cargados en la lista.

        Arranca en 0.
    */

    int i;
    /*
        Variable auxiliar para recorrer el arreglo.
    */

    int pos = -1;
    /*
        Posición donde está el jugador actual dentro de lista.

        Arranca en -1 para indicar:
            "todavía no lo encontré".

        Si después encontramos al jugador, pos tendrá su índice.

        Ejemplo:
            pos = 3
            significa que el jugador está en lista[3].
    */

    FILE *pf;
    /*
        Puntero a archivo.
    */

    char nom[STATS_NOMBRE_MAX];
    /*
        Variable auxiliar para leer nombres desde el archivo.
    */

    unsigned mejor, partidas, lineasArchivo;
    /*
        Variables auxiliares para leer datos numéricos del archivo.

        mejor:
            mejor puntaje leído.

        partidas:
            cantidad de partidas leídas.

        lineasArchivo:
            líneas totales leídas.
    */

    pf = fopen(STATS_ARCHIVO, "rt");
    /*
        Intentamos abrir stats.txt para lectura.

        Si existe, vamos a cargar todos los jugadores existentes.
    */

    if (pf) {
    /*
        Si el archivo se pudo abrir...
    */

        while (cant < 64 && fscanf(pf, "%15s %u %u %u", nom, &mejor, &partidas, &lineasArchivo) == 4) {
        /*
            Leemos jugadores mientras:
                - no superemos el límite de 64 jugadores
                - fscanf pueda leer 4 datos por línea

            Formato esperado:
                nombre mejorPuntaje partidas lineasTotales

            Ejemplo:
                JUGADOR 863 1 6
        */

            memset(&lista[cant], 0, sizeof(lista[cant]));
            /*
                Limpiamos la posición actual de la lista.

                &lista[cant]:
                    dirección de la estructura actual.

                sizeof(lista[cant]):
                    tamaño de un tStatsJugador.
            */

            strncpy(lista[cant].nombre, nom, STATS_NOMBRE_MAX - 1);
            /*
                Copiamos el nombre leído dentro de la estructura.
            */

            lista[cant].mejorPuntaje = mejor;
            /*
                Guardamos el mejor puntaje leído.
            */

            lista[cant].partidas = partidas;
            /*
                Guardamos la cantidad de partidas leída.
            */

            lista[cant].lineasTotales = lineasArchivo;
            /*
                Guardamos las líneas totales leídas.
            */

            if (strcmp(nom, nombre) == 0) {
            /*
                Comparamos el nombre leído con el jugador actual.

                Si son iguales, encontramos la posición del jugador
                dentro del arreglo lista.
            */

                pos = cant;
                /*
                    Guardamos la posición donde está el jugador.
                */
            }

            cant++;
            /*
                Aumentamos la cantidad de jugadores cargados.

                Pasamos a la siguiente posición del arreglo.
            */
        }

        fclose(pf);
        /*
            Cerramos el archivo después de leerlo.
        */
    }

    if (pos == -1 && cant < 64) {
    /*
        Si pos sigue en -1, significa que el jugador no existía
        en stats.txt.

        Además verificamos cant < 64 para asegurarnos de que
        todavía hay espacio en el arreglo.
    */

        pos = cant;
        /*
            La posición del nuevo jugador será la próxima libre.
        */

        memset(&lista[cant], 0, sizeof(lista[cant]));
        /*
            Limpiamos esa posición nueva.
        */

        strncpy(lista[cant].nombre, nombre, STATS_NOMBRE_MAX - 1);
        /*
            Copiamos el nombre del jugador nuevo.
        */

        cant++;
        /*
            Ahora hay un jugador más en la lista.
        */
    }

    if (pos != -1) {
    /*
        Si pos es distinto de -1, significa que tenemos
        una posición válida para actualizar.

        Puede ser:
            - un jugador que ya existía
            - un jugador nuevo que acabamos de agregar
    */

        if (puntaje > lista[pos].mejorPuntaje) {
        /*
            Si el puntaje de esta partida supera el mejor puntaje guardado...
        */

            lista[pos].mejorPuntaje = puntaje;
            /*
                Actualizamos el récord.
            */
        }

        lista[pos].partidas++;
        /*
            Sumamos una partida jugada.
        */

        lista[pos].lineasTotales += lineas;
        /*
            Sumamos las líneas de esta partida
            al total histórico del jugador.
        */
    }

    pf = fopen(STATS_ARCHIVO, "wt");
    /*
        Abrimos stats.txt para escritura en modo texto.

        "wt":
            w = write, escribir
            t = text, texto

        Esto pisa el archivo anterior.

        ¿Por qué lo pisamos?
            Porque ya cargamos todos los datos en lista,
            actualizamos lo necesario,
            y ahora reescribimos todo ordenado.
    */

    if (!pf) {
    /*
        Si no se pudo abrir el archivo para escribir,
        salimos.

        No podemos guardar las estadísticas.
    */

        return;
    }

    for (i = 0; i < cant; i++) {
    /*
        Recorremos todos los jugadores cargados en lista.
    */

        fprintf(pf, "%s %u %u %u\n",
                lista[i].nombre,
                (unsigned)lista[i].mejorPuntaje,
                (unsigned)lista[i].partidas,
                (unsigned)lista[i].lineasTotales);
        /*
            Escribimos una línea por jugador.

            Formato:
                nombre mejorPuntaje partidas lineasTotales

            Ejemplo:
                JUGADOR 863 1 6

            %s:
                string

            %u:
                entero sin signo

            \n:
                salto de línea
        */
    }

    fclose(pf);
    /*
        Cerramos el archivo después de escribir.
    */
}
