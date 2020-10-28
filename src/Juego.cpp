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
#define ERROR -1

#define ANCHO_VENTANA 800
#define ALTO_VENTANA 600

#define DERECHA 1
#define IZQUIERDA -1

#define FPS 60
#define FRAME_DELAY 1000/FPS


Juego::Juego() {
    estado_error = Juego::inicializar_ventana();
    camara = new Camara();
    lectorXml = new LectorXML(renderer);
    jugador = new Jugador(renderer);
    lectorXml->generar_nivel(&enemigos,&escenarios, &background, std::string("nivel1"));
}

Juego::~Juego(){
    delete(camara);
    delete(lectorXml);
    delete(jugador);
    delete(background);
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
    if (SDL_Init(SDL_INIT_VIDEO) != 0){
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return ERROR;
    }

    ventana = SDL_CreateWindow("Hermanos Mario", 100, 100, ANCHO_VENTANA, ALTO_VENTANA, SDL_WINDOW_SHOWN);
    if (ventana == nullptr){
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        return ERROR;
    }


    renderer = SDL_CreateRenderer(ventana, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr){
        SDL_DestroyWindow(ventana);
        std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return ERROR;
    }
    std::string fileName = "./res/icono_mario.png";
    SDL_Surface* icono_surface = IMG_Load(fileName.c_str());
    if(icono_surface == NULL){
        std::cout << "No cargo el Icono " << std::endl;
        return ERROR;
    }
    //SDL_SetRenderDrawColor(renderer, 144, 202, 249, 255);
    //SDL_SetColorKey( icono_surface, SDL_TRUE, SDL_MapRGB( icono_surface->format, 0xFF, 0xFF, 0xFF ) );
    SDL_SetWindowIcon(ventana,icono_surface);
    SDL_FreeSurface(icono_surface);


    return 0;
}

void Juego::game_loop() {
    SDL_Event evento;
    if (estado_error == ERROR)
        quit = true;
    while (!quit){
        int frame_start = SDL_GetTicks();
        while (!background->es_fin_nivel() && !quit) {
            update(evento);
            render();
            int frame_time = SDL_GetTicks() - frame_start;
            if (FRAME_DELAY > frame_time)
                SDL_Delay(FRAME_DELAY - frame_time);
        }
        camara->stop_scrolling();
        jugador->reset_posicion();
        lectorXml->generar_nivel(&enemigos,&escenarios, &background, std::string("nivel2"));
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(ventana);
    SDL_Quit();
}

void Juego::update(SDL_Event evento) {
    jugador->desplazar();
    for (int i=0;i<enemigos.size();i++){
        enemigos.at(i)->desplazar();
    }
    while (SDL_PollEvent(&evento) != 0) {
        if (evento.type == SDL_QUIT)
            quit = true;
        jugador->recibir_evento(evento);
    }
}

void Juego::render(){
    SDL_RenderClear(renderer);
    camara->scroll_background(background, renderer);
    for (size_t i=0; i<enemigos.size(); i++){
        enemigos.at(i)->cambiar_frame(renderer, camara);
    }
    for (size_t i=0; i<escenarios.size(); i++){
        escenarios.at(i)->cambiar_frame(renderer, camara);
    }
    jugador->cambiar_frame(renderer, camara);
    SDL_RenderPresent(renderer);
}
