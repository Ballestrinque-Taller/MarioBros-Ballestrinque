#include "../include/Renderer.h"
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

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