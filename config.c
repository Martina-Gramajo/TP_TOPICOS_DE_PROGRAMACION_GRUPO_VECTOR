#include "config.h"
/*
    Incluimos config.h porque acá implementamos las funciones declaradas ahí.

    Gracias a este include, este archivo conoce:
        - tConfig
        - eResolucion
        - RES_CGA
        - RES_VGA
        - CONFIG_ARCHIVO
        - los prototipos de las funciones
*/

#include <stdio.h>
/*
    Incluimos stdio.h para usar funciones de archivos y entrada/salida.

    En este archivo usamos:
        FILE
        fopen
        fscanf
        fprintf
        fclose
*/

#include <string.h>
/*
    Incluimos string.h para usar funciones de manejo de texto.

    En este archivo usamos:
        strcmp
        strncmp
*/

#include <stdlib.h>
/*
    Incluimos stdlib.h para funciones generales.

    En este archivo usamos:
        atoi

    atoi convierte texto a número entero.

    Ejemplo:
        "800" → 800
*/

void config_aplicar_resolucion(tConfig *cfg)
/*
    Esta función recibe un puntero a tConfig.

    Su tarea es mirar qué resolución está configurada
    y completar el ancho y alto correspondientes.

    También se asegura de que la escala tenga un valor válido
    si estaba en 0.
*/
{
    if (cfg->resolucion == RES_VGA) {
    /*
        Preguntamos si la resolución elegida es VGA.

        cfg->resolucion:
            Accede al campo resolucion de la estructura.

        Como cfg es un puntero, usamos ->
        En estructuras normales usaríamos .
    */

        cfg->ancho = 640;
        /*
            Si es VGA, el ancho lógico es 640.
        */

        cfg->alto = 480;
        /*
            Si es VGA, el alto lógico es 480.
        */

        if (cfg->escala == 0) {
        /*
            Si la escala está en 0, significa que no hay una escala válida.

            Entonces le ponemos una por defecto.
        */

            cfg->escala = 1;
            /*
                Para VGA usamos escala 1 por defecto.
            */
        }
    } else {
    /*
        Si no es VGA, asumimos que es CGA.
    */

        cfg->ancho = 320;
        /*
            Si es CGA, el ancho lógico es 320.
        */

        cfg->alto = 200;
        /*
            Si es CGA, el alto lógico es 200.
        */

        if (cfg->escala == 0) {
        /*
            Si la escala no tiene valor válido, ponemos una por defecto.
        */

            cfg->escala = 3;
            /*
                Para CGA usamos escala 3 por defecto,
                porque 320x200 es chico y conviene agrandarlo.
            */
        }
    }
}

void config_cargar_default(tConfig *cfg)
/*
    Esta función carga la configuración inicial por defecto.

    Es lo primero que se hace en main antes de leer config.txt
    o aplicar argumentos.
*/
{
    cfg->resolucion = RES_CGA;
    /*
        Por defecto usamos resolución CGA.
    */

    cfg->escala = 3;
    /*
        Por defecto usamos escala 3.
    */

    cfg->velocidadInicialMs = 1000;
    /*
        La pieza empieza cayendo cada 1000 milisegundos.

        1000 ms = 1 segundo.
    */

    cfg->paleta = 0;
    /*
        Por defecto usamos la paleta 0, que es la clásica.
    */

    config_aplicar_resolucion(cfg);
    /*
        Después de definir la resolución, calculamos ancho y alto.

        Como resolucion = RES_CGA, esta función va a poner:
            ancho = 320
            alto = 200
    */
}

void config_leer_archivo(tConfig *cfg)
/*
    Esta función intenta leer la configuración desde config.txt.

    Si el archivo existe, actualiza cfg.
    Si no existe, no hace nada.
*/
{
    FILE *pf = fopen(CONFIG_ARCHIVO, "rt");
    /*
        FILE *pf:
            Declara un puntero a archivo.

        fopen:
            Intenta abrir un archivo.

        CONFIG_ARCHIVO:
            Es "config.txt".

        "rt":
            r = read, leer.
            t = text, texto.

        Entonces:
            abre config.txt para lectura en modo texto.
    */

    if (!pf) {
    /*
        Si pf es NULL, significa que el archivo no pudo abrirse.

        Puede pasar porque:
            - config.txt no existe
            - no hay permisos
            - hay algún problema de lectura

        En ese caso, salimos de la función.
    */

        return;
        /*
            No se modifica cfg.
            Se mantienen los valores anteriores.
        */
    }

    int resolucion = 0;
    /*
        Variable auxiliar para leer la resolución desde el archivo.

        Usamos int porque fscanf lee cómodo con %d.
    */

    unsigned escala = 0;
    /*
        Variable auxiliar para leer la escala.
    */

    unsigned velocidad = 1000;
    /*
        Variable auxiliar para leer la velocidad.

        La inicializamos en 1000 por seguridad.
    */

    unsigned paleta = 0;
    /*
        Variable auxiliar para leer la paleta.
    */

    if (fscanf(pf, "resolucion=%d\n", &resolucion) == 1 &&
        fscanf(pf, "escala=%u\n", &escala) == 1 &&
        fscanf(pf, "velocidad=%u\n", &velocidad) == 1 &&
        fscanf(pf, "paleta=%u\n", &paleta) == 1) {
    /*
        Leemos el archivo línea por línea.

        fscanf:
            Lee texto con un formato específico.

        Ejemplo esperado:
            resolucion=0
            escala=3
            velocidad=1000
            paleta=0

        &resolucion:
            Pasamos la dirección de memoria para que fscanf escriba ahí.

        == 1:
            Significa que logró leer correctamente un dato.

        El if usa &&:
            Solo entra si las 4 lecturas salieron bien.
    */

        cfg->resolucion = resolucion == 1 ? RES_VGA : RES_CGA;
        /*
            Operador ternario:

            condicion ? valor_si_verdadero : valor_si_falso

            Si resolucion == 1:
                cfg->resolucion = RES_VGA

            Si no:
                cfg->resolucion = RES_CGA
        */

        cfg->escala = (uint8_t)escala;
        /*
            Guardamos la escala leída.

            Convertimos unsigned a uint8_t porque en tConfig
            escala es uint8_t.
        */

        cfg->velocidadInicialMs = velocidad < 100 ? 100 : velocidad;
        /*
            Si la velocidad leída es menor a 100,
            la forzamos a 100.

            Esto evita velocidades absurdamente bajas.

            Si velocidad < 100:
                guarda 100

            Si no:
                guarda velocidad
        */

        cfg->paleta = (uint8_t)paleta;
        /*
            Guardamos la paleta leída.
        */

        config_aplicar_resolucion(cfg);
        /*
            Después de leer resolución y escala,
            recalculamos ancho y alto.
        */
    }

    fclose(pf);
    /*
        Cerramos el archivo.

        Siempre que abrimos un archivo con fopen,
        debemos cerrarlo con fclose.
    */
}

void config_guardar_archivo(const tConfig *cfg)
/*
    Esta función guarda la configuración actual en config.txt.

    Recibe const tConfig *cfg:
        const indica que la función no va a modificar cfg.
        Solo va a leer sus valores.
*/
{
    FILE *pf = fopen(CONFIG_ARCHIVO, "wt");
    /*
        Abrimos config.txt para escritura en modo texto.

        "wt":
            w = write, escribir.
            t = text, texto.

        Si el archivo no existe, se crea.
        Si existe, se pisa su contenido anterior.
    */

    if (!pf) {
    /*
        Si no se pudo abrir el archivo, salimos.

        No conviene romper el juego solo porque no se pudo guardar config.
    */

        return;
    }

    fprintf(pf, "resolucion=%d\n", cfg->resolucion == RES_VGA ? 1 : 0);
    /*
        Escribimos la resolución.

        Si es VGA, guardamos 1.
        Si no, guardamos 0.
    */

    fprintf(pf, "escala=%u\n", (unsigned)cfg->escala);
    /*
        Escribimos la escala.

        Convertimos a unsigned para imprimir con %u.
    */

    fprintf(pf, "velocidad=%u\n", (unsigned)cfg->velocidadInicialMs);
    /*
        Escribimos la velocidad inicial en milisegundos.
    */

    fprintf(pf, "paleta=%u\n", (unsigned)cfg->paleta);
    /*
        Escribimos la paleta elegida.
    */

    fclose(pf);
    /*
        Cerramos el archivo.
    */
}

void config_aplicar_argumentos(tConfig *cfg, int argc, char *argv[])
/*
    Esta función revisa los argumentos con los que se ejecutó el programa.

    Ejemplo:
        tetris.exe VGA ESCALA=2 VEL=800

    argc:
        Cantidad de argumentos.

    argv:
        Lista de textos.

    Esta función puede cambiar la configuración.
*/
{
    int i;
    /*
        Variable para recorrer los argumentos.
    */

    for (i = 1; i < argc; i++) {
    /*
        Recorremos desde i = 1.

        ¿Por qué no desde 0?
            Porque argv[0] suele ser el nombre del programa.

        Ejemplo:
            argv[0] = "tetris.exe"
            argv[1] = "VGA"
            argv[2] = "ESCALA=2"
    */

        if (strcmp(argv[i], "CGA") == 0 || strcmp(argv[i], "cga") == 0) {
        /*
            strcmp compara dos textos.

            Devuelve 0 si son iguales.

            Entonces:
                strcmp(argv[i], "CGA") == 0
            significa:
                el argumento actual es "CGA".

            También aceptamos "cga" en minúscula.
        */

            cfg->resolucion = RES_CGA;
            /*
                Si el argumento es CGA, configuramos resolución CGA.
            */

        } else if (strcmp(argv[i], "VGA") == 0 || strcmp(argv[i], "vga") == 0) {
        /*
            Si el argumento es VGA o vga...
        */

            cfg->resolucion = RES_VGA;
            /*
                Configuramos resolución VGA.
            */

        } else if (strncmp(argv[i], "ESCALA=", 7) == 0) {
        /*
            strncmp compara solo los primeros N caracteres.

            Acá comparamos los primeros 7 caracteres con "ESCALA=".

            Entonces detecta argumentos como:
                ESCALA=1
                ESCALA=2
                ESCALA=3
        */

            int escala = atoi(argv[i] + 7);
            /*
                argv[i] + 7 salta los primeros 7 caracteres.

                Si argv[i] es:
                    "ESCALA=3"

                argv[i] + 7 apunta a:
                    "3"

                atoi convierte "3" a número 3.
            */

            if (escala >= 1 && escala <= 8) {
            /*
                Validamos que la escala esté entre 1 y 8.

                Evitamos valores imposibles o exagerados.
            */

                cfg->escala = (uint8_t)escala;
                /*
                    Guardamos la escala en cfg.
                */
            }

        } else if (strncmp(argv[i], "VEL=", 4) == 0) {
        /*
            Detectamos argumentos que empiezan con "VEL=".

            Ejemplo:
                VEL=800
                VEL=1000
        */

            int vel = atoi(argv[i] + 4);
            /*
                Saltamos "VEL=" y convertimos lo que sigue a número.

                Ejemplo:
                    "VEL=800" → "800" → 800
            */

            if (vel >= 100 && vel <= 3000) {
            /*
                Validamos que la velocidad esté entre 100 y 3000 ms.

                100 sería muy rápido.
                3000 sería bastante lento.
            */

                cfg->velocidadInicialMs = (uint32_t)vel;
                /*
                    Guardamos la velocidad inicial.
                */
            }
        }
    }

    config_aplicar_resolucion(cfg);
    /*
        Al final, recalculamos ancho y alto.

        Esto es importante porque quizá los argumentos cambiaron
        la resolución.
    */
}
