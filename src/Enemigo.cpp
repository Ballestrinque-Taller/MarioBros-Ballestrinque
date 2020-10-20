#include "Enemigo.h"
#include "Texturas.h"
#include "Renderer.h"
#include <SDL2/SDL.h>

#define ANCHO_FRAME 808
#define ALTO_FRAME 128
#define POS_INICIAL_X_FRAME 0
#define POS_INICIAL_Y_FRAME 0
#define MAX_FRAME 1

#define ANCHO_ENEMIGO_PANTALLA 800
#define ALTO_ENEMIGO_PANTALLA 128

Enemigo::Enemigo(){
    texturas.flip = SDL_FLIP_NONE;
    aceleracion_x = 0;
    muerto = false;
    tick_actual = 0;
}