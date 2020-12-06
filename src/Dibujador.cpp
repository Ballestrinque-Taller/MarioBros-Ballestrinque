#include <Log.h>
#include "Dibujador.h"

void Dibujador::dibujar(std::vector<entidad_t> entidades_a_dibujar, TextWriter* nivel_label, int nivel_actual, TextWriter* temporizador_label, int tiempo_restante, SDL_Renderer* renderer){
    SDL_RenderClear(renderer);
    for (int i=0;i<=entidades_a_dibujar.size()-1;i++){
        if(entidades_a_dibujar.at(i).esta_desconectado) {
            SDL_DestroyTexture(texturas.at(i));
            texturas.at(i) = crear_textura(entidades_a_dibujar.at(i), renderer);
        }
        SDL_RenderCopyEx(renderer, texturas.at(i), &(entidades_a_dibujar.at(i).src_rect), &(entidades_a_dibujar.at(i).dest_rect), 0, nullptr, entidades_a_dibujar.at(i).flip);
    }
    nivel_label->write_text((std::string("Nivel ") + std::to_string(nivel_actual)).c_str(), renderer);
    temporizador_label->write_text((std::string("TIME ")+std::to_string(tiempo_restante)).c_str(), renderer);
    SDL_RenderPresent(renderer);
}

void Dibujador::crear_texturas(std::vector<entidad_t> entidades_a_texturizar, SDL_Renderer* renderer){
    for (auto & textura : texturas){
        SDL_DestroyTexture(textura);
    }
    texturas.clear();
    for (auto & entidad : entidades_a_texturizar){
        SDL_Texture* textura = crear_textura(entidad, renderer);
        if (textura == nullptr){
            LOG(Log::ERROR)<<"No se pudo generar la textura: "<<entidad.path_textura << std::endl;
        }else {
            texturas.push_back(textura);
        }
    }
}

SDL_Texture* Dibujador::crear_textura(entidad_t entidad, SDL_Renderer* renderer){
    SDL_Surface* surface = IMG_Load(entidad.path_textura);
    if (surface == nullptr){
        LOG(Log::ERROR) << "No se encontro el archivo: " << entidad.path_textura << ". Cargando textura por defecto..." << std::endl;
        surface = IMG_Load(entidad.default_path);
    }
    SDL_SetColorKey( surface, SDL_TRUE, SDL_MapRGB( surface->format, 0x92, 0x27, 0x8F ) ); //0x92 0x27 0x8F es el color del divisor de mario.
    SDL_Texture* textura = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return textura;
}

Dibujador::~Dibujador(){
    for (auto & textura : texturas){
        SDL_DestroyTexture(textura);
    }
}