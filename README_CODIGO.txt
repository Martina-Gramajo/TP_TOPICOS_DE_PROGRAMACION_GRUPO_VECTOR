TETRIS GBT - base separada por archivos

Archivos:
- main.c: arranque del programa, GBT, ventana, loop principal.
- config.h/c: configuracion, archivo config.txt y argumentos de main.
- juego.h/c: estados del juego, input, caida, pausa, game over.
- tablero.h/c: matriz del tablero, colisiones, fijar piezas y limpiar lineas.
- tetromino.h/c: piezas I J L O S T Z y rotacion izquierda/derecha.
- render.h/c: dibujo grafico con GBT, bloques, texto, pantallas.
- stats.h/c: guarda record simple por jugador en stats.txt.

Importante:
No se incluye la biblioteca GBT. Deben tener configurado Code::Blocks con:
- Search directories / Compiler: ruta a include de GBT
- Search directories / Linker: ruta a lib de GBT
- Other linker options: -lgbt
- gbt.dll copiada junto al .exe generado.

Argumentos:
- CGA o VGA
- ESCALA=n
- VEL=n

Ejemplos:
tetris.exe CGA ESCALA=3 VEL=1000
tetris.exe VGA ESCALA=1 VEL=800

Controles:
- ENTER: iniciar / confirmar / volver a jugar
- Flecha izquierda/derecha: mover
- Flecha abajo: bajar manualmente y sumar puntos
- Z: rotar izquierda
- X: rotar derecha
- P: pausar / volver
- ESC: salir
