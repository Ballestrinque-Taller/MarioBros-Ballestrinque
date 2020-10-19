#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

#include "Renderer.h"

void Renderer::set_src_rect(int x,int y,int height,int width){
    frames_render.src_rect.x=x;
    frames_render.src_rect.y=y;
    frames_render.src_rect.h=height;
    frames_render.src_rect.w=width;
}

void Renderer::set_dest_rect(int x,int y,int height,int width){
    frames_render.dest_rect.x=x;
    frames_render.dest_rect.y=y;
    frames_render.dest_rect.h=height;
    frames_render.dest_rect.w=width;
}

void Renderer::renderizar(SDL_Renderer* renderer){
    SDL_Surface* surface =  IMG_Load(path_to_image.c_str());
    SDL_SetColorKey( surface, SDL_TRUE, SDL_MapRGB( surface->format, 0x92, 0x27, 0x8F ) ); //0x92 0x27 0x8F es el color del divisor
    texturas.textura = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    SDL_RenderCopyEx(renderer, texturas.textura, &(frames_render.src_rect), &(frames_render.dest_rect), 0, NULL, texturas.flip);
}