#include "Jugador.h"
#include "Renderer.h"

#define DERECHA 1
#define IZQUIERDA -1
#define MAX_ACELERACION 7
#define TICK_ACELERACION 3
#define DECAIMIENTO_ACEL_Y 1
#define MAX_ACEL_GRAVEDAD 10
#define ACELERACION_SALTO 20

Jugador::Jugador(SDL_Renderer* renderer){
    path_to_image = "./res/mario.png";
    set_dest_rect(0,0,200,200);
    set_src_rect(0,0,920,920);
    aceleracion_x = 0;
    aceleracion_y = 0;
    Jugador::renderizar(renderer);
}

void Jugador::renderizar(SDL_Renderer* renderer){
    SDL_Surface* surface =  IMG_Load(path_to_image.c_str());
    SDL_Surface* surface2 = IMG_Load("./res/icono_mario.png");
    texturas.texturas[0] = SDL_CreateTextureFromSurface(renderer, surface);
    texturas.texturas[1] = SDL_CreateTextureFromSurface(renderer, surface2);
    SDL_FreeSurface(surface);
    SDL_FreeSurface(surface2);
    SDL_RenderCopy(renderer, texturas.texturas[0], &(frames_render.src_rect), &(frames_render.dest_rect));
}

void Jugador::cambiar_frame(SDL_Renderer* renderer){
    if (aceleracion_x<0){
        textura_actual = texturas.texturas[0];
    }
    else{
        textura_actual = texturas.texturas[1];
    }
    SDL_RenderCopy(renderer, textura_actual, &(frames_render.src_rect), &(frames_render.dest_rect));

}

void Jugador::acelerar_x(int direccion){
    if (direccion == DERECHA && aceleracion_x < MAX_ACELERACION){
        aceleracion_x += TICK_ACELERACION;
    }
    else if (direccion == IZQUIERDA && aceleracion_x > -(MAX_ACELERACION)){
        aceleracion_x -= TICK_ACELERACION;
    }
    acelerando = true;
}

void Jugador::saltar() {
    if (aceleracion_y == 0) {
        aceleracion_y = -ACELERACION_SALTO;
        //textura_actual = texturas.textura_salto;
    }
}

void Jugador::desplazar(){
    frames_render.dest_rect.x += aceleracion_x;
    aceleracion_gravitatoria();
    frames_render.dest_rect.y += aceleracion_y;
    if(aceleracion_x < 0 && !acelerando && !en_aire)
        aceleracion_x += TICK_ACELERACION;
    else if (aceleracion_x > 0 && !acelerando && !en_aire)
        aceleracion_x -= TICK_ACELERACION;
    acelerando = false;
}

void Jugador::aceleracion_gravitatoria(){
    //IF !COLISION && ACEL < MAX_ACEL_GRAVEDAD (BAJA MENOS DE LO MAXIMO)
    if (aceleracion_y < MAX_ACEL_GRAVEDAD && frames_render.dest_rect.y < 600-frames_render.dest_rect.h){
        aceleracion_y += DECAIMIENTO_ACEL_Y;
        en_aire = true;
    }
    //IF COLISION && ACEL PARA ABAJO (POSITIVA)
    else if (aceleracion_y > 0 && frames_render.dest_rect.y >= 600-frames_render.dest_rect.h){
        aceleracion_y = 0;
        en_aire = false;
    }
}