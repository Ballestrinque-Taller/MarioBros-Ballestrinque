#include <Log.h>
#include "Dibujador.h"
#include <bits/stdc++.h>

#define ANCHO_ID 40
#define ANCHO_HONGO_SRC 16
#define ANCHO_CHAR 10
#define ALTO_BARRA_NIVEL 40
#define ALTO_ID 30
#define ANCHO_PJ 30
#define MARGEN_VIDAS 5
#define ALTO_PJ 40

Dibujador::Dibujador(){
    texto_nivel = new TextWriter();
    texto_nivel->set_msg_rect(200,50,100,400);

}

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
            text_puntos.at(cantidad_de_jugadores)->set_msg_rect((cantidad_de_jugadores)*(ANCHO_ID+2*ANCHO_PJ),ALTO_BARRA_NIVEL+(ALTO_ID*3)/2,ALTO_ID, ANCHO_CHAR*std::to_string(entidades_a_dibujar.at(i).puntaje).size());
            text_puntos.at(cantidad_de_jugadores)->write_text(std::to_string(entidades_a_dibujar.at(i).puntaje).c_str(), renderer);
            for (int j=0; j<entidades_a_dibujar.at(i).vidas; j++){
                entidad_t dibujo_vida;
                strcpy(dibujo_vida.path_textura,"./res/ItemsObjectsNPCS.png");
                dibujo_vida.src_rect.x = ANCHO_HONGO_SRC; dibujo_vida.src_rect.y=0; dibujo_vida.src_rect.w=ANCHO_HONGO_SRC; dibujo_vida.src_rect.h=ANCHO_HONGO_SRC;
                dibujo_vida.dest_rect.w = ANCHO_ID/2;
                dibujo_vida.dest_rect.x = (cantidad_de_jugadores)*(ANCHO_ID+2*ANCHO_PJ)+j*(dibujo_vida.dest_rect.w+MARGEN_VIDAS);
                dibujo_vida.dest_rect.y = ALTO_BARRA_NIVEL+(ALTO_ID*5)/2;
                dibujo_vida.dest_rect.h = dibujo_vida.dest_rect.w;
                SDL_Texture* textura = crear_textura(dibujo_vida, renderer);
                texturas.push_back(textura);
                SDL_RenderCopyEx(renderer, textura, &(dibujo_vida).src_rect, &(dibujo_vida).dest_rect, 0, nullptr, SDL_FLIP_NONE);
                //SDL_DestroyTexture(textura);
            }
            cantidad_de_jugadores++;
        }
        if((entidades_a_dibujar.at(i).es_jugador && !entidades_a_dibujar.at(i).muerto) || !entidades_a_dibujar.at(i).es_jugador)
            SDL_RenderCopyEx(renderer, texturas.at(i), &(entidades_a_dibujar.at(i).src_rect), &(entidades_a_dibujar.at(i).dest_rect), 0, nullptr, entidades_a_dibujar.at(i).flip);
    }
    for (auto& puntaje : puntajes_jugadores){
        delete(puntaje);
    }
    for(auto& nombre: nombres_jugadores){
        delete(nombre);
    }
    nombres_jugadores.clear();
    puntajes_jugadores.clear();
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
        //if((entidad.es_jugador && !entidad.muerto) || !entidad.es_jugador) {
            SDL_Texture *textura = crear_textura(entidad, renderer);
            if (textura == nullptr) {
                LOG(Log::ERROR) << "No se pudo generar la textura: " << entidad.path_textura << std::endl;
            } else {
                texturas.push_back(textura);
            }
        //}
    }
}

SDL_Texture* Dibujador::crear_textura(entidad_t entidad, SDL_Renderer* renderer){
    SDL_Texture *textura = nullptr;
    //if((entidad.es_jugador && !entidad.muerto) || !entidad.es_jugador) {
        SDL_Surface *surface = IMG_Load(entidad.path_textura);
        if (surface == nullptr) {
            LOG(Log::ERROR) << "No se encontro el archivo: " << entidad.path_textura
                            << ". Cargando textura por defecto..." << std::endl;
            surface = IMG_Load(entidad.default_path);
        }
        SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0x92, 0x27,
                                                      0x8F)); //0x92 0x27 0x8F es el color del divisor de mario.
        textura = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    //}
    return textura;
}

void Dibujador::generar_identificador_jugador(){
    TextWriter* text_writer_pj = new TextWriter();
    TextWriter* text_writer_puntos = new TextWriter();
    text_writer_pj->set_msg_rect((cantidad_de_jugadores)*(ANCHO_ID+2*ANCHO_PJ),ALTO_BARRA_NIVEL,ALTO_ID, ANCHO_ID);
    //ACOMODAR LOS MARGENES BIEN DE ESTO
    text_writers.push_back(text_writer_pj);
    text_puntos.push_back(text_writer_puntos);
}

int comparador_jugadores(entidad_t jugador_a_comparar, entidad_t jugador_comparado){
    if (jugador_a_comparar.puntaje > jugador_comparado.puntaje){
        return 1;
    }else if (jugador_a_comparar.puntaje < jugador_comparado.puntaje){
        return -1;
    }
    return 0;
}

void sort_jugadores(std::vector<entidad_t>* jugadores){
    for(int i=0;i<(*jugadores).size()-1;i++){
        int maximo = i;
        for(int j=1;j<(*jugadores).size();j++){
            if((*jugadores).at(j).puntaje>(*jugadores).at(maximo).puntaje)
                maximo = j;
        }
        entidad_t aux = (*jugadores).at(i);
        (*jugadores).at(i) = (*jugadores).at(maximo);
        (*jugadores).at(maximo) = aux;
    }
}

void Dibujador::dibujar_cambio_nivel(std::vector<entidad_t> jugadores, std::string texto_arriba, SDL_Renderer* renderer) {
    //std::sort(jugadores.begin(),jugadores.end(), comparador_jugadores);
    sort_jugadores(&jugadores);
    SDL_RenderClear(renderer);
    entidad_t background;
    background.dest_rect.x = 0, background.dest_rect.y = 0, background.dest_rect.h = 600, background.dest_rect.w = 800;
    background.src_rect.x = 0, background.src_rect.y = 0, background.src_rect.h = 600, background.src_rect.w = 800;
    strcpy(background.path_textura, "./res/FONDO_TRANSICION_redimensionado.png");
    for (int i = 0; i < jugadores.size(); ++i) {
        jugadores.at(i).dest_rect.x = 200;
        jugadores.at(i).dest_rect.y = 150 + 90 * (i+1);
        jugadores.at(i).dest_rect.h = 80;
        jugadores.at(i).src_rect.x = 0;
        jugadores.at(i).src_rect.y = 0;
        jugadores.at(i).src_rect.h = 32;
        jugadores.at(i).flip = SDL_FLIP_NONE;
        jugadores.at(i).muerto = false;
        if(puntajes_jugadores.size() < jugadores.size()) {
            nombres_jugadores.push_back(new TextWriter());
            nombres_jugadores.at(i)->set_msg_rect(300, 150 + 90 * (i + 1), 40, 20 * strlen(jugadores.at(i).usuario));
            puntajes_jugadores.push_back(new TextWriter);
            puntajes_jugadores.at(i)->set_msg_rect(500, 150 + 90 * (i + 1), 40, 20 * std::to_string(jugadores.at(i).puntaje).size()  );
        }
    }
    crear_texturas(jugadores, renderer);
    texturas.insert(texturas.begin(),crear_textura(background, renderer));
    for(int i = 0 ; i < texturas.size(); i++){
        if(i==0){
            SDL_RenderCopyEx(renderer, texturas.at(i), &(background.src_rect), &(background.dest_rect), 0, nullptr, SDL_FLIP_NONE);
        }else {
            SDL_RenderCopyEx(renderer, texturas.at(i), &(jugadores.at(i-1).src_rect), &(jugadores.at(i-1).dest_rect), 0,
                             nullptr, jugadores.at(i-1).flip);
        }
    }
    for(int i=0; i< jugadores.size(); i++){
        nombres_jugadores.at(i)->write_text(jugadores.at(i).usuario, renderer);
        puntajes_jugadores.at(i)->write_text(std::to_string(jugadores.at(i).puntaje).c_str(), renderer);
    }
    texto_nivel->write_text(texto_arriba.c_str(), renderer);
    SDL_RenderPresent(renderer);
}

void Dibujador::dibujar_fin_juego(std::vector<entidad_t> jugadores, SDL_Renderer* renderer){
    sort_jugadores(&jugadores);
    char felicitaciones[16];
    strcpy(felicitaciones,"Felicitaciones ");
    dibujar_cambio_nivel(jugadores, strcat(felicitaciones,jugadores.at(0).usuario) ,renderer);
}

Dibujador::~Dibujador(){
    for (auto & textura : texturas){
        SDL_DestroyTexture(textura);
    }
    texturas.clear();
    for (auto & text_writer: text_writers){
        delete(text_writer);
    }
    for (auto & text_puntaje: text_puntos){
        delete(text_puntaje);
    }
    text_writers.clear();
    text_puntos.clear();
}