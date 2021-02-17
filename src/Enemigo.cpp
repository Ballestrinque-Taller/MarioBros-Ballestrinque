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
    en_aire = true;
    velocidad_y = 0;
}

void Enemigo::aceleracion_gravitatoria(){
    if(!en_aire)
        velocidad_y = 0;
    if(velocidad_y < MAX_ACEL_GRAVEDAD && en_aire){
        velocidad_y += DECAIMIENTO_ACEL_Y;
    }
}

void Enemigo::desplazar(){
    frames_render.dest_rect.x += velocidad_x;
    aceleracion_gravitatoria();
    frames_render.dest_rect.y += velocidad_y;
}

void Enemigo::cambiar_direccion(){
    velocidad_x = -velocidad_x;
    if(texturas.flip == SDL_FLIP_NONE){
        texturas.flip = SDL_FLIP_HORIZONTAL;
    }else{
        texturas.flip = SDL_FLIP_NONE;
    }
}

void Enemigo::morir(){
    muerto=true;
    velocidad_x = 0;
}

bool Enemigo::esta_muerto(){
    return muerto;
}

void Enemigo::set_enemigo_en_aire(bool estado){
    en_aire = estado;
    if (!en_aire)
        velocidad_y = 0;
}

int Enemigo::get_velocidad_x() {
    return velocidad_x;
}