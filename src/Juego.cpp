#include <SDL2/SDL.h>
#include <iostream>

#include "Juego.h"
#include "Camara.h"
#include "Background.h"
#include "Log.h"

#define ERROR_JUEGO -1

#define ANCHO_VENTANA 800
#define ALTO_VENTANA 600

#define DERECHA 1
#define IZQUIERDA -1

#define FPS 60
#define FRAME_DELAY 1000/FPS

#define POS_X_TEXTO 800
#define POS_Y_TEXTO 0
#define HEIGHT_TEXTO 30
#define WIDTH_TEXTO 90
/*
Juego::Juego(std::string path_xml) {

}

Juego::~Juego(){
    LOG(Log::DEBUG) << "Destruyendo juego.cpp" << std::endl;
    delete(camara);
    delete(lectorXml);
    for (auto & jugador : jugadores)
        delete(jugador);
    delete(background);
    delete(temporizador);
    for (size_t i=0; i<enemigos.size(); i++){ //Recorro el vector y deleteo cada enemigo
        delete(enemigos.at(i));
    };
    enemigos.clear();
    for (size_t i=0; i<escenarios.size(); i++){
        delete(escenarios.at(i));
    };
    escenarios.clear();
}
*/



/*
void Juego::render(SDL_Renderer** renderer_cliente){
    SDL_RenderClear((*renderer_cliente));
    camara->scroll_background(background, (*renderer_cliente));

    for (auto & enemigo : enemigos) {
        enemigo->cambiar_frame((*renderer_cliente), camara);
    }
    
    for (auto & escenario : escenarios) {
        escenario->cambiar_frame((*renderer_cliente), camara);
    }
    for (auto & jugador : jugadores)
        jugador->cambiar_frame((*renderer_cliente), camara);
    temporizador->render((*renderer_cliente));
    nivel_label->write_text((std::string("Nivel ") + std::to_string(nivel_actual)).c_str(), (*renderer_cliente));
    SDL_RenderPresent((*renderer_cliente));
}
 */