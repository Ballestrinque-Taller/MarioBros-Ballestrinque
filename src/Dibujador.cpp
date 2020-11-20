#include <Log.h>
#include "Dibujador.h"

Dibujador::Dibujador(SDL_Renderer* renderer_sdl){
    this->renderer = renderer_sdl;
}

void Dibujador::dibujar(std::vector<entidad_t> entidades_a_dibujar){
    for (int i=0;i<entidades_a_dibujar.size();i++){
        SDL_RenderCopyEx(renderer, texturas.at(i), &(entidades_a_dibujar[i].src_rect), &(entidades_a_dibujar[i].dest_rect), 0, nullptr, entidades_a_dibujar[i].flip);
    }
}

void Dibujador::crear_texturas(std::vector<entidad_t> entidades_a_texturizar){
    for (auto & textura : texturas){
        SDL_DestroyTexture(textura);
    }
    for (auto & entidad : entidades_a_texturizar){
        SDL_Surface* surface =  IMG_Load(entidad.path_textura.c_str());
        if (surface == nullptr){
        LOG(Log::ERROR) << "No se encontro el archivo: " << entidad.path_textura << ". Cargando textura por defecto..." << std::endl;
        surface = IMG_Load(entidad.default_path.c_str());
        }
        SDL_SetColorKey( surface, SDL_TRUE, SDL_MapRGB( surface->format, 0x92, 0x27, 0x8F ) ); //0x92 0x27 0x8F es el color del divisor de mario.
        texturas.push_back(SDL_CreateTextureFromSurface(renderer, surface));
        SDL_FreeSurface(surface);
    }
}