#include <Log.h>
#include "Dibujador.h"

#define ANCHO_ID 40
#define ALTO_BARRA_NIVEL 40
#define ALTO_ID 30
#define ANCHO_PJ 30
#define ALTO_PJ 40

void Dibujador::dibujar(std::vector<entidad_t> entidades_a_dibujar, TextWriter* nivel_label, int nivel_actual, TextWriter* temporizador_label, int tiempo_restante, SDL_Renderer* renderer){
    cantidad_de_jugadores = 0;
    SDL_RenderClear(renderer);
    for (int i=0;i<=entidades_a_dibujar.size()-1;i++){
        if(entidades_a_dibujar.at(i).es_jugador) {
            SDL_DestroyTexture(texturas.at(i));
            texturas.at(i) = crear_textura(entidades_a_dibujar.at(i), renderer);
            if(!genere_identificadores) {
                generar_identificador_jugador();
            }
            SDL_Rect dest_rect;
            dest_rect.x = (cantidad_de_jugadores)*(ANCHO_ID+2*ANCHO_PJ)+ANCHO_ID;
            dest_rect.y = ALTO_BARRA_NIVEL;
            dest_rect.h = ALTO_PJ;
            dest_rect.w = ANCHO_PJ;
            SDL_Rect src_rect = entidades_a_dibujar.at(i).src_rect;
            src_rect.x = 0;
            SDL_RenderCopyEx(renderer, texturas.at(i), &(src_rect), &(dest_rect), 0, nullptr, SDL_FLIP_NONE);
            text_writers.at(cantidad_de_jugadores)->write_text(entidades_a_dibujar.at(i).usuario, renderer);
            cantidad_de_jugadores++;
        }
        SDL_RenderCopyEx(renderer, texturas.at(i), &(entidades_a_dibujar.at(i).src_rect), &(entidades_a_dibujar.at(i).dest_rect), 0, nullptr, entidades_a_dibujar.at(i).flip);
    }
    genere_identificadores = true;
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
    SDL_SetColorKey( surface, SDL_TRUE, SDL_MapRGB( surface->format, 0x47, 0x70, 0x4C ) );
    SDL_Texture* textura = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return textura;
}

void Dibujador::generar_identificador_jugador(){
    TextWriter* text_writer_pj = new TextWriter();
    text_writer_pj->set_msg_rect((cantidad_de_jugadores)*(ANCHO_ID+2*ANCHO_PJ),ALTO_BARRA_NIVEL,ALTO_ID, ANCHO_ID);
    text_writers.push_back(text_writer_pj);
}

Dibujador::~Dibujador(){
    for (auto & textura : texturas){
        SDL_DestroyTexture(textura);
    }
    texturas.clear();
    for (auto & text_writer: text_writers){
        delete(text_writer);
    }
    text_writers.clear();
}