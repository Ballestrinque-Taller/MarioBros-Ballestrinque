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

//TODO: VER SI ESTA FUNCION VUELA
void Renderer::renderizar(SDL_Renderer* renderer){
    renderizado = true;
    SDL_Surface* surface =  IMG_Load(path_to_image.c_str());
    if (surface == nullptr){
        LOG(Log::ERROR) << "No se encontro el archivo: " << path_to_image << ". Cargando textura por defecto..." << std::endl;
        surface = IMG_Load(default_path.c_str());
    }
    SDL_SetColorKey( surface, SDL_TRUE, SDL_MapRGB( surface->format, 0x92, 0x27, 0x8F ) ); //0x92 0x27 0x8F es el color del divisor de mario.
    texturas.textura = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    SDL_RenderCopyEx(renderer, texturas.textura, &(frames_render.src_rect), &(frames_render.dest_rect), 0, nullptr, texturas.flip);
}

void Renderer::set_dest_rect_x(int x){
    frames_render.dest_rect.x=x;
}

int Renderer::get_dest_rect_x(){
    return frames_render.dest_rect.x;
}

Renderer::~Renderer(){
    LOG(Log::DEBUG)<<"Destruyendo renderer..."<<std::endl;
    SDL_DestroyTexture(texturas.textura);
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