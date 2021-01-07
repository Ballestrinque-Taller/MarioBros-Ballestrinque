#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

#include "Renderer.h"
#include "Log.h"

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

void Renderer::set_dest_rect_x(int x){
    frames_render.dest_rect.x=x;
}

void Renderer::set_dest_rect_y(int y) {
    frames_render.dest_rect.y=y;
}

int Renderer::get_dest_rect_x(){
    return frames_render.dest_rect.x;
}

SDL_Rect Renderer::get_src_rect(){
    return frames_render.src_rect;
}

SDL_Rect Renderer::get_dest_rect(){
    return frames_render.dest_rect;
}

std::string Renderer::get_path_img(){
    return path_to_image;
}

std::string Renderer::get_def_path(){
    return default_path;
}

SDL_RendererFlip Renderer::get_flip(){
    return texturas.flip;
}