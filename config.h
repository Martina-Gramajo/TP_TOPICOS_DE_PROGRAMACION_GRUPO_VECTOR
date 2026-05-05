#ifndef CONFIG_H_INCLUDED
/*
    Esto es un "include guard".

    Sirve para evitar que este archivo .h se incluya más de una vez
    durante la compilación.

    Si CONFIG_H_INCLUDED todavía NO está definido,
    el compilador entra y lee el contenido del archivo.
*/

#define CONFIG_H_INCLUDED
/*
    Acá definimos CONFIG_H_INCLUDED.

    Entonces, si este mismo archivo vuelve a incluirse en otro lugar,
    el #ifndef de arriba ya no se cumple y se evita duplicar definiciones.
*/

#include <stdint.h>
/*
    Incluimos stdint.h para poder usar tipos de datos con tamaño fijo.

    Ejemplos:
        uint8_t   → entero sin signo de 8 bits
        uint16_t  → entero sin signo de 16 bits
        uint32_t  → entero sin signo de 32 bits

    En videojuegos y gráficos suele ser útil usar tipos con tamaño claro.
*/

#define CONFIG_ARCHIVO "config.txt"
/*
    Definimos el nombre del archivo donde se guarda la configuración.

    En este proyecto, la configuración se guarda y se lee desde:
        config.txt

    Como usamos un #define, si mañana queremos cambiar el nombre del archivo,
    lo cambiamos acá y no en todo el código.
*/

typedef enum {
    RES_CGA = 0,
    RES_VGA = 1
} eResolucion;
/*
    Este enum define las resoluciones posibles.

    enum:
        Es una lista de valores con nombre.

    RES_CGA:
        Representa resolución CGA.
        En este proyecto equivale a 320x200.

    RES_VGA:
        Representa resolución VGA.
        En este proyecto equivale a 640x480.

    Internamente:
        RES_CGA vale 0.
        RES_VGA vale 1.
*/

typedef struct {
    eResolucion resolucion;
    /*
        Guarda qué resolución se está usando.

        Puede ser:
            RES_CGA
            RES_VGA
    */

    uint16_t ancho;
    /*
        Guarda el ancho lógico de la ventana.

        Si es CGA:
            ancho = 320

        Si es VGA:
            ancho = 640
    */

    uint16_t alto;
    /*
        Guarda el alto lógico de la ventana.

        Si es CGA:
            alto = 200

        Si es VGA:
            alto = 480
    */

    uint8_t escala;
    /*
        Guarda el factor de escala de la ventana.

        La escala agranda visualmente la ventana.

        Ejemplo:
            resolución lógica = 320x200
            escala = 3

        La ventana se ve más grande, pero internamente seguimos dibujando
        en coordenadas de 320x200.
    */

    uint32_t velocidadInicialMs;
    /*
        Guarda la velocidad inicial de caída de la pieza, en milisegundos.

        Ejemplo:
            1000 significa que la pieza baja cada 1000 ms,
            o sea, cada 1 segundo.
    */

    uint8_t paleta;
    /*
        Guarda qué paleta de colores se usa.

        En tu proyecto:
            0 → paleta clásica
            1 → paleta noche
    */
} tConfig;
/*
    tConfig es una estructura que agrupa toda la configuración del juego.

    En vez de tener variables sueltas:
        resolucion
        ancho
        alto
        escala
        velocidad
        paleta

    Las guardamos todas juntas dentro de una sola estructura.
*/

void config_cargar_default(tConfig *cfg);
/*
    Declara la función que carga valores por defecto en una variable tConfig.

    Recibe:
        tConfig *cfg

    El * significa que recibe un puntero.
    O sea, recibe la dirección de memoria de una estructura tConfig.

    Así la función puede modificar la configuración original.
*/

void config_aplicar_resolucion(tConfig *cfg);
/*
    Declara la función que, según la resolución elegida,
    completa el ancho y el alto.

    Si cfg->resolucion es RES_CGA:
        ancho = 320
        alto = 200

    Si cfg->resolucion es RES_VGA:
        ancho = 640
        alto = 480
*/

void config_leer_archivo(tConfig *cfg);
/*
    Declara la función que intenta leer la configuración desde config.txt.

    Si el archivo existe y está bien escrito, actualiza cfg.

    Si el archivo no existe, no pasa nada:
    se mantienen los valores que ya tenía cfg.
*/

void config_guardar_archivo(const tConfig *cfg);
/*
    Declara la función que guarda la configuración en config.txt.

    Recibe:
        const tConfig *cfg

    const significa que la función no debería modificar cfg.
    Solo lo lee para escribir sus datos en el archivo.
*/

void config_aplicar_argumentos(tConfig *cfg, int argc, char *argv[]);
/*
    Declara la función que lee argumentos del main.

    Ejemplo de ejecución:
        tetris.exe VGA ESCALA=1 VEL=800

    argc:
        Cantidad de argumentos.

    argv:
        Lista de textos con esos argumentos.

    Esta función puede modificar cfg según lo que reciba.
*/

#endif
/*
    Fin del include guard.
*/
