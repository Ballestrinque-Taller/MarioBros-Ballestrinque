#include "Enemigo.h"
#include "Texturas.h"
#include "Renderer.h"
#include <SDL2/SDL.h>

#define ANCHO_FRAME 808
#define ALTO_FRAME 128
#define POS_INICIAL_X_FRAME 0
#define POS_INICIAL_Y_FRAME 0
#define MAX_FRAME 1
#define MAX_ACEL_GRAVEDAD 10
#define DECAIMIENTO_ACEL_Y 1

#define ANCHO_ENEMIGO_PANTALLA 800
#define ALTO_ENEMIGO_PANTALLA 128

Enemigo::Enemigo(){
    texturas.flip = SDL_FLIP_NONE;
    velocidad_x = -VELOCIDAD_ENEMIGOS;
    muerto = false;
    tick_actual = 0;
}

Enemigo::~Enemigo(){
}

void Enemigo::aceleracion_gravitatoria(){
    //IF !COLISION && ACEL < MAX_ACEL_GRAVEDAD (BAJA MENOS DE LO MAXIMO)
    if (velocidad_y < MAX_ACEL_GRAVEDAD && frames_render.dest_rect.y < 535 - frames_render.dest_rect.h) {
        velocidad_y += DECAIMIENTO_ACEL_Y;
        en_aire = true;
    }
        //IF COLISION && ACEL PARA ABAJO (POSITIVA)
    else if (velocidad_y > 0 && frames_render.dest_rect.y >= 535 - frames_render.dest_rect.h) {
        velocidad_y = 0;
        frames_render.dest_rect.y = 535 - frames_render.dest_rect.h;
        en_aire = false;
    }
}

void Enemigo::desplazar(){
    frames_render.dest_rect.x += velocidad_x;
    aceleracion_gravitatoria();
    frames_render.dest_rect.y += velocidad_y;
}

void Enemigo::cambiar_direccion(){
    bool colision = false;
    if(colision){
        velocidad_x = -velocidad_x;
    }
}