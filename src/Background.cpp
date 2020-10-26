#define WIDTH 800
#define HEIGHT 600
#define WIDTH_SRC 304
#define HEIGHT_SRC 224
#define RATIO_ASPECTO WIDTH_SRC/WIDTH
#define POS_Y 0
#define POS_X_INIC 0
#define ANCHO_IMAGEN 3392

#include "Background.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Texturas.h"

Background::Background(SDL_Renderer* renderer){
    set_src_rect(POS_X_INIC, POS_Y, HEIGHT_SRC, WIDTH_SRC);
    set_dest_rect(POS_X_INIC, POS_Y, HEIGHT, WIDTH);
    texturas.flip = SDL_FLIP_NONE;
    path_to_image = "./res/Nivel_1_sinTuberia.png";
    pos_x_actual = 0;
    renderizar(renderer);
}


void Background::scroll(int velocidad_scroll, SDL_Renderer* renderer){
    //if ((pos_x_actual + velocidad_scroll + WIDTH_SRC)<=ANCHO_IMAGEN);
    pos_x_actual += velocidad_scroll*RATIO_ASPECTO;
    set_src_rect(pos_x_actual, POS_Y, HEIGHT_SRC, WIDTH_SRC);
    SDL_RenderCopyEx(renderer, texturas.textura, &(frames_render.src_rect), &(frames_render.dest_rect), 0, NULL, texturas.flip);
}

