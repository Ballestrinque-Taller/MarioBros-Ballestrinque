#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

#include "Jugador.h"
#include "Juego.h"
#include "Enemigo.h"
#include "Goomba.h"
#include "Tortuga.h"
#include "Camara.h"
#include "Background.h"
#include "Escenario.h"
#include "Moneda.h"
#include "Ladrillo.h"
#include "Sorpresa.h"
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


Juego::Juego(std::string path_xml) {
    estado_error = Juego::inicializar_ventana();
    camara = new Camara();
    lectorXml = new LectorXML(renderer, path_xml);
    if (lectorXml->generar_nivel(&enemigos,&escenarios, &background, &temporizador, std::string("nivel1")) == ERROR_XML){
        lectorXml->set_default();
        lectorXml->generar_nivel(&enemigos,&escenarios, &background, &temporizador, std::string("nivel1"));
    }
    lectorXml->generar_jugador(&jugadores);
    nivel_actual = 1;
    nivel_label = new TextWriter();
    nivel_label->set_msg_rect(POS_X_TEXTO-WIDTH_TEXTO, POS_Y_TEXTO, HEIGHT_TEXTO, WIDTH_TEXTO);
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

int Juego::inicializar_ventana(){
    //LOG(Log::INFO) << "Inicializando ventana de la aplicacion." << std::endl;
    if (SDL_Init(SDL_INIT_VIDEO) != 0){
        LOG(Log::ERROR) << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return ERROR_JUEGO;
    }

    ventana = SDL_CreateWindow("Hermanos Mario", 100, 100, ANCHO_VENTANA, ALTO_VENTANA, SDL_WINDOW_SHOWN);
    if (ventana == nullptr){
        LOG(Log::ERROR) << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        return ERROR_JUEGO;
    }


    renderer = SDL_CreateRenderer(ventana, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr){
        SDL_DestroyWindow(ventana);
        LOG(Log::ERROR) << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return ERROR_JUEGO;
    }
    std::string fileName = "./res/icono_mario.png";
    SDL_Surface* icono_surface = IMG_Load(fileName.c_str());
    if(icono_surface == NULL){
        LOG(Log::ERROR)<<"No cargo el icono de la aplicacion. Valor del surface: " << icono_surface << ". Path: " << fileName << std::endl;
        return ERROR_JUEGO;
    }
    SDL_SetWindowIcon(ventana,icono_surface);
    SDL_FreeSurface(icono_surface);


    return 0;
}

void Juego::game_loop() {
    SDL_Event evento;
    if (estado_error == ERROR_JUEGO)
        quit = true;
    while (!quit){
        while (!background->es_fin_nivel() && !quit) {
            int frame_start = SDL_GetTicks();
            update(evento);
            render();
            int frame_time = SDL_GetTicks() - frame_start;
            if (FRAME_DELAY > frame_time)
                SDL_Delay(FRAME_DELAY - frame_time);
        }
        if(!quit) {
            nivel_actual++;
            camara->stop_scrolling();
            for (auto & jugador : jugadores)
                jugador->reset_posicion();
            std::string nivel_str = (std::string("nivel")+std::to_string(nivel_actual));
            if (lectorXml->generar_nivel(&enemigos, &escenarios, &background, &temporizador, nivel_str) == QUIT)
                quit = true;
        }
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(ventana);
    SDL_Quit();
}

void Juego::update(SDL_Event evento) {
    temporizador->update();
    for (auto & jugador : jugadores)
        jugador->desplazar();
    for (auto & enemigo : enemigos){
        enemigo->desplazar();
    }
    while (SDL_PollEvent(&evento) != 0) {
        if (evento.type == SDL_QUIT)
            quit = true;
        jugadores.at(0)->recibir_evento(evento);
    }
}

void Juego::render(){
    SDL_RenderClear(renderer);
    camara->scroll_background(background, renderer);

    for (auto & enemigo : enemigos) {
        enemigo->cambiar_frame(renderer, camara);
    }
    
    for (auto & escenario : escenarios) {
        escenario->cambiar_frame(renderer, camara);
    }
    for (auto & jugador : jugadores)
        jugador->cambiar_frame(renderer, camara);
    temporizador->render(renderer);
    nivel_label->write_text((std::string("Nivel ") + std::to_string(nivel_actual)).c_str(), renderer);
    SDL_RenderPresent(renderer);
}
