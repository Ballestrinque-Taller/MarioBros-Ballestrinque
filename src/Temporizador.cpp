#include "Temporizador.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#define POS_X 0
#define POS_Y 0
#define WIDTH_MSG 90
#define HEIGHT_MSG  30

Temporizador :: Temporizador(size_t tiempo_duracion_nivel){
    tiempo_inicial = SDL_GetTicks();
    tiempo_restante = tiempo_duracion_nivel;
    tiempo_parametro = tiempo_duracion_nivel;
    set_msg_rect(POS_X,POS_Y,HEIGHT_MSG,WIDTH_MSG);
}

void Temporizador::reiniciar(size_t tiempo){
    tiempo_inicial = SDL_GetTicks();
    tiempo_restante = tiempo;
    tiempo_parametro = tiempo;
}

void Temporizador :: update(){
    size_t tiempo_actual = SDL_GetTicks();
    size_t diferencia_de_tiempo =  tiempo_actual - tiempo_inicial;
    tiempo_restante = tiempo_parametro - diferencia_de_tiempo/1000;
}

void Temporizador::render(SDL_Renderer* renderer){
    TextWriter::write_text((std::string("TIME ")+std::to_string(tiempo_restante)).c_str(), renderer);
}

Temporizador::~Temporizador(){
}