#include "Enemigo.h"
#include "Texturas.h"
#include "Renderer.h"
#include <SDL2/SDL.h>

#define ANCHO_FRAME 808
#define ALTO_FRAME 128
#define POS_INICIAL_X_FRAME 0
#define POS_INICIAL_Y_FRAME 0

#define ANCHO_ENEMIGO_PANTALLA 800
#define ALTO_ENEMIGO_PANTALLA 128

Enemigo::Enemigo(SDL_Renderer* renderer, int pos_x, int pos_y){
    set_dest_rect(pos_x, pos_y, ALTO_ENEMIGO_PANTALLA, ANCHO_ENEMIGO_PANTALLA);
    set_src_rect(POS_INICIAL_X_FRAME, POS_INICIAL_Y_FRAME, ALTO_FRAME, ANCHO_FRAME);
    texturas.flip = SDL_FLIP_NONE;
    path_to_image = "../res/EnemiesBosses.png";
    aceleracion_x = 0;
    renderizar(renderer);
}

void Enemigo::cambiar_frame(SDL_Renderer* renderer){
    SDL_RenderCopyEx(renderer, texturas.textura, &(frames_render.src_rect), &(frames_render.dest_rect), 0, NULL, texturas.flip);
}