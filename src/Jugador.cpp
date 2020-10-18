#include "Jugador.h"
#include "Renderer.h"

#define DERECHA 1
#define IZQUIERDA -1

#define MAX_ACELERACION 7
#define TICK_ACELERACION 3
#define DECAIMIENTO_ACEL_Y 1
#define DECAIMIENTO_ROZAMIENTO 1
#define MAX_ACEL_GRAVEDAD 10
#define ACELERACION_SALTO 20

#define ANCHO_FRAME 17
#define ALTO_FRAME 34
#define ANCHO_IMAGEN 358

#define TIEMPO_FRAME 5
#define FRAME_SALTO 5
#define FRAME_MOV_FINAL 3
#define FRAME_AGACHADO 6

Jugador::Jugador(SDL_Renderer* renderer){
    path_to_image = "../res/MARIO_NORMAL.png";
    set_dest_rect(0,0,200,200);
    set_src_rect(0,0,ALTO_FRAME,ANCHO_FRAME);
    aceleracion_x = 0;
    aceleracion_y = 0;
    frame_actual = 0;
    agachado = false;
    en_aire = true;
    tick_actual = TIEMPO_FRAME;
    texturas.flip = SDL_FLIP_NONE;
    Jugador::renderizar(renderer);
}

void Jugador::renderizar(SDL_Renderer* renderer){
    SDL_Surface* surface =  IMG_Load(path_to_image.c_str());
    SDL_SetColorKey( surface, SDL_TRUE, SDL_MapRGB( surface->format, 0x92, 0x27, 0x8F ) ); //0x92 0x27 0x8F es el color del divisor
    texturas.textura = SDL_CreateTextureFromSurface(renderer, surface);
    textura_actual = texturas.textura;
    SDL_FreeSurface(surface);
    SDL_RenderCopyEx(renderer, texturas.textura, &(frames_render.src_rect), &(frames_render.dest_rect), 0, NULL, texturas.flip);
}

void Jugador::cambiar_frame(SDL_Renderer* renderer){
    tick_actual++;
    if (tick_actual >= TIEMPO_FRAME && aceleracion_x != 0 && !en_aire){
        tick_actual = 0;
        frame_actual++;
            if(frame_actual > FRAME_MOV_FINAL)
                frame_actual = 0;
    }
    else if(agachado){
        tick_actual = 0;
        frame_actual = FRAME_AGACHADO;
    }
    else if(en_aire){
        tick_actual = 0;
        frame_actual = FRAME_SALTO;
    }
    else if (aceleracion_x == 0 && !en_aire){
        tick_actual = 0;
        frame_actual = 0;
    }
    set_src_rect(frame_actual*ANCHO_FRAME,0,ALTO_FRAME,ANCHO_FRAME);
    SDL_RenderCopyEx(renderer, textura_actual, &(frames_render.src_rect), &(frames_render.dest_rect), 0, NULL, texturas.flip);
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
    if (aceleracion_y == 0 && !en_aire)
        aceleracion_y = -ACELERACION_SALTO;
}

void Jugador::desplazar(){
    frames_render.dest_rect.x += aceleracion_x;
    aceleracion_gravitatoria();
    frames_render.dest_rect.y += aceleracion_y;
    rozamiento();
    acelerando = false;
}

void Jugador::rozamiento(){
    if(aceleracion_x < 0 && !acelerando && !en_aire)
        aceleracion_x += DECAIMIENTO_ROZAMIENTO;
    else if (aceleracion_x > 0 && !acelerando && !en_aire)
        aceleracion_x -= DECAIMIENTO_ROZAMIENTO;
}

void Jugador::aceleracion_gravitatoria() {
    //IF !COLISION && ACEL < MAX_ACEL_GRAVEDAD (BAJA MENOS DE LO MAXIMO)
    if (aceleracion_y < MAX_ACEL_GRAVEDAD && frames_render.dest_rect.y < 600 - frames_render.dest_rect.h) {
        aceleracion_y += DECAIMIENTO_ACEL_Y;
        en_aire = true;
    }
    //IF COLISION && ACEL PARA ABAJO (POSITIVA)
    else if (aceleracion_y > 0 && frames_render.dest_rect.y >= 600 - frames_render.dest_rect.h) {
        aceleracion_y = 0;
        en_aire = false;
    }
}

void Jugador::recibir_evento(SDL_Event evento) {
    if (evento.type == SDL_KEYDOWN){
        switch (evento.key.keysym.sym) {
                case (SDLK_a):
                    texturas.flip = SDL_FLIP_HORIZONTAL;
                    acelerar_x(IZQUIERDA);
                    break;
                case (SDLK_d):
                    texturas.flip = SDL_FLIP_NONE;
                    acelerar_x(DERECHA);
                    break;
                case (SDLK_s):
                    agacharse();
                    break;
                case (SDLK_w):
                    saltar();
                    break;
        }
    }
    else if (evento.type == SDL_KEYUP){
        if (evento.key.keysym.sym == SDLK_s)
            agachado = false;
    }
}

void Jugador::agacharse(){
    if (!en_aire){
        //BAJAR HITBOX A LA MITAD
        agachado = true;
        aceleracion_x=0;
    }
}
