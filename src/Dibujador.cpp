#include <Log.h>
#include "Dibujador.h"

Dibujador::Dibujador(SDL_Renderer* renderer_sdl){
    //this->renderer = renderer_sdl;
}

void Dibujador::dibujar(std::vector<entidad_t> entidades_a_dibujar, SDL_Renderer* renderer){
    SDL_RenderClear(renderer);
    for (int i=0;i<=entidades_a_dibujar.size()-1;i++){
        SDL_RenderCopyEx(renderer, texturas.at(i), &(entidades_a_dibujar.at(i).src_rect), &(entidades_a_dibujar.at(i).dest_rect), 0, nullptr, entidades_a_dibujar.at(i).flip);
    }
    SDL_RenderPresent(renderer);
}

void Dibujador::crear_texturas(std::vector<entidad_t> entidades_a_texturizar, SDL_Renderer* renderer){
    for (auto & textura : texturas){
        SDL_DestroyTexture(textura);
    }
    for (auto & entidad : entidades_a_texturizar){
        std::cout<<"Haciendo textura: "<<entidad.path_textura<<std::endl;
        SDL_Surface* surface = IMG_Load(entidad.path_textura);
        if (surface == nullptr){
            LOG(Log::ERROR) << "No se encontro el archivo: " << entidad.path_textura << ". Cargando textura por defecto..." << std::endl;
            surface = IMG_Load(entidad.default_path);
        }
        SDL_SetColorKey( surface, SDL_TRUE, SDL_MapRGB( surface->format, 0x92, 0x27, 0x8F ) ); //0x92 0x27 0x8F es el color del divisor de mario.
        SDL_Texture* textura = SDL_CreateTextureFromSurface(renderer, surface);
        if (textura == nullptr){
            LOG(Log::ERROR)<<"No se pudo generar la textura: "<<entidad.path_textura << std::endl;
        }else {
            texturas.push_back(textura);
        }
        SDL_FreeSurface(surface);
    }
}

Dibujador::~Dibujador(){
    for (auto & textura : texturas){
        SDL_DestroyTexture(textura);
    }
}