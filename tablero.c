#include "tablero.h"
/*
    Incluimos tablero.h porque acá implementamos sus funciones.

    Este archivo necesita conocer:
        - tTablero
        - tTetromino
        - constantes del tablero
*/

#include <string.h>
/*
    Incluimos string.h para usar:
        memset
        memcpy
*/

void tablero_inicializar(tTablero *tablero)
/*
    Inicializa el tablero.

    Recibe un puntero a tTablero porque va a modificarlo.
*/
{
    memset(tablero->celda, 0, sizeof(tablero->celda));
    /*
        memset llena un bloque de memoria con un valor.

        tablero->celda:
            matriz completa del tablero.

        0:
            valor con el que llenamos la matriz.

        sizeof(tablero->celda):
            tamaño total de la matriz.

        Resultado:
            todas las celdas quedan en 0.

        En este juego:
            0 = vacío.
    */
}

uint8_t tablero_puede_ubicar(const tTablero *tablero, const tTetromino *pieza, int x, int y)
/*
    Esta función responde la pregunta:

        ¿La pieza puede estar ubicada en la posición x, y?

    tablero:
        tablero actual.

    pieza:
        pieza que queremos probar.

    x:
        columna donde queremos poner la pieza.

    y:
        fila donde queremos poner la pieza.

    Devuelve:
        1 → se puede ubicar.
        0 → no se puede ubicar.
*/
{
    int pf, pc;
    /*
        pf:
            fila dentro de la matriz de la pieza.

        pc:
            columna dentro de la matriz de la pieza.

        La pieza es 4x4.
    */

    for (pf = 0; pf < TETRO_TAM; pf++) {
    /*
        Recorremos las filas de la pieza.
    */

        for (pc = 0; pc < TETRO_TAM; pc++) {
        /*
            Recorremos las columnas de la pieza.
        */

            if (pieza->m[pf][pc]) {
            /*
                Si en la matriz de la pieza hay un 1,
                significa que en esa posición hay un bloque.

                Si hay 0, no hay nada y no hace falta validar.
            */

                int tf = y + pf;
                /*
                    tf significa "tablero fila".

                    Es la fila real del tablero donde caería
                    este bloque de la pieza.
                */

                int tc = x + pc;
                /*
                    tc significa "tablero columna".

                    Es la columna real del tablero donde caería
                    este bloque de la pieza.
                */

                if (tc < 0 || tc >= TABLERO_COLUMNAS) {
                /*
                    Validamos límites horizontales.

                    tc < 0:
                        se fue por la izquierda.

                    tc >= TABLERO_COLUMNAS:
                        se fue por la derecha.
                */

                    return 0;
                    /*
                        No se puede ubicar.
                    */
                }

                if (tf >= TABLERO_FILAS) {
                /*
                    Validamos el piso.

                    Si la fila real es mayor o igual a la cantidad
                    total de filas, la pieza se fue por debajo del tablero.
                */

                    return 0;
                    /*
                        No se puede ubicar.
                    */
                }

                if (tf >= 0 && tablero->celda[tf][tc] != 0) {
                /*
                    Validamos choque contra bloques fijos.

                    tf >= 0:
                        Solo revisamos si la fila existe dentro del tablero.
                        Esto permite que una pieza pueda estar parcialmente
                        arriba del tablero.

                    tablero->celda[tf][tc] != 0:
                        significa que esa celda ya está ocupada.
                */

                    return 0;
                    /*
                        No se puede ubicar porque choca.
                    */
                }
            }
        }
    }

    return 1;
    /*
        Si recorrimos toda la pieza y no encontramos problemas,
        significa que la ubicación es válida.
    */
}

void tablero_fijar_pieza(tTablero *tablero, const tTetromino *pieza, int x, int y)
/*
    Esta función copia una pieza al tablero.

    Se usa cuando la pieza ya no puede bajar más.

    A partir de este momento, la pieza deja de ser "móvil"
    y pasa a formar parte del tablero fijo.
*/
{
    int pf, pc;
    /*
        Variables para recorrer la matriz 4x4 de la pieza.
    */

    for (pf = 0; pf < TETRO_TAM; pf++) {
    /*
        Recorremos filas de la pieza.
    */

        for (pc = 0; pc < TETRO_TAM; pc++) {
        /*
            Recorremos columnas de la pieza.
        */

            if (pieza->m[pf][pc]) {
            /*
                Si hay un bloque en esa posición de la pieza...
            */

                int tf = y + pf;
                /*
                    Calculamos fila real en el tablero.
                */

                int tc = x + pc;
                /*
                    Calculamos columna real en el tablero.
                */

                if (tf >= 0 && tf < TABLERO_FILAS && tc >= 0 && tc < TABLERO_COLUMNAS) {
                /*
                    Nos aseguramos de que esa posición esté dentro del tablero.

                    Esto evita escribir fuera de la matriz,
                    que en C sería un error grave.
                */

                    tablero->celda[tf][tc] = pieza->color;
                    /*
                        Guardamos el color de la pieza en esa celda.

                        Antes:
                            la celda podía estar vacía.

                        Después:
                            queda ocupada por un bloque fijo.
                    */
                }
            }
        }
    }
}

static uint8_t fila_completa(const tTablero *tablero, int fila)
/*
    Esta función revisa si una fila está completa.

    static:
        Significa que solo se puede usar dentro de tablero.c.

    Devuelve:
        1 → fila completa.
        0 → fila incompleta.
*/
{
    int c;
    /*
        Variable para recorrer columnas.
    */

    for (c = 0; c < TABLERO_COLUMNAS; c++) {
    /*
        Recorremos todas las columnas de la fila.
    */

        if (tablero->celda[fila][c] == 0) {
        /*
            Si encontramos una celda vacía,
            la fila NO está completa.
        */

            return 0;
            /*
                Devolvemos 0.
            */
        }
    }

    return 1;
    /*
        Si no encontramos ningún 0,
        significa que todas las celdas están ocupadas.

        Entonces la fila está completa.
    */
}

static void bajar_filas(tTablero *tablero, int filaEliminada)
/*
    Esta función baja todas las filas superiores
    cuando se elimina una fila.

    static:
        Solo se usa dentro de tablero.c.
*/
{
    int f;
    /*
        Variable para recorrer filas.
    */

    for (f = filaEliminada; f > 0; f--) {
    /*
        Empezamos en la fila eliminada y vamos subiendo.

        La idea es copiar cada fila superior hacia abajo.
    */

        memcpy(tablero->celda[f], tablero->celda[f - 1], sizeof(tablero->celda[f]));
        /*
            Copiamos la fila de arriba en la fila actual.

            tablero->celda[f]:
                fila destino.

            tablero->celda[f - 1]:
                fila origen, la de arriba.

            sizeof(tablero->celda[f]):
                tamaño de una fila completa.

            Ejemplo:
                la fila 10 recibe lo que estaba en la fila 9.
                la fila 9 recibe lo que estaba en la fila 8.
                etc.
        */
    }

    memset(tablero->celda[0], 0, sizeof(tablero->celda[0]));
    /*
        Después de bajar todas las filas, la fila de arriba queda libre.

        Entonces la limpiamos con 0.
    */
}

uint8_t tablero_limpiar_lineas(tTablero *tablero)
/*
    Busca filas completas, las elimina y baja las filas de arriba.

    Devuelve la cantidad de líneas borradas.
*/
{
    int f;
    /*
        Variable para recorrer filas.
    */

    uint8_t lineas = 0;
    /*
        Contador de líneas borradas.

        Arranca en 0.
    */

    for (f = TABLERO_FILAS - 1; f >= 0; f--) {
    /*
        Recorremos desde abajo hacia arriba.

        TABLERO_FILAS - 1:
            última fila del tablero.

        En Tetris conviene revisar desde abajo,
        porque las líneas se completan generalmente abajo.
    */

        if (fila_completa(tablero, f)) {
        /*
            Si la fila actual está completa...
        */

            bajar_filas(tablero, f);
            /*
                Eliminamos esa fila y bajamos las superiores.
            */

            lineas++;
            /*
                Sumamos una línea borrada.
            */

            f++;
            /*
                Esto es importante.

                Como bajaron filas, en la misma posición f
                ahora hay una fila nueva que antes estaba arriba.

                Entonces hacemos f++ para que el for,
                al hacer f--, vuelva a revisar la misma fila.
            */
        }
    }

    return lineas;
    /*
        Devolvemos cuántas líneas se borraron.

        Esto después se usa para sumar puntos.
    */
}

uint8_t tablero_hay_bloques_en_zona_oculta(const tTablero *tablero)
/*
    Revisa si hay bloques en las filas ocultas de arriba.

    Si hay bloques ahí, significa que la pila llegó arriba
    y el jugador perdió.
*/
{
    int f, c;
    /*
        Variables para recorrer filas y columnas.
    */

    for (f = 0; f < TABLERO_FILAS_OCULTAS; f++) {
    /*
        Recorremos solamente las filas ocultas.

        En este proyecto son las primeras 2 filas.
    */

        for (c = 0; c < TABLERO_COLUMNAS; c++) {
        /*
            Recorremos las columnas de cada fila oculta.
        */

            if (tablero->celda[f][c] != 0) {
            /*
                Si encontramos una celda ocupada,
                hay bloques en la zona oculta.
            */

                return 1;
                /*
                    Indicamos que hay bloques arriba.
                */
            }
        }
    }

    return 0;
    /*
        Si no encontramos bloques en la zona oculta,
        devolvemos 0.
    */
}
