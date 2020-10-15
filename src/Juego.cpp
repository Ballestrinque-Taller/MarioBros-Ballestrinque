#include "../include/Juego.h"
#include <SDL2/SDL.h>
#include <iostream>
#include "../include/Jugador.h"

#define ERROR -1
#define ANCHO_VENTANA 800
#define ALTO_VENTANA 600


Juego::Juego() {
    estado_error = Juego::inicializar_ventana();
    jugador_test = new Jugador();
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
    return 0;
}

void Juego::game_loop() {
    SDL_Event evento;
    if (estado_error == ERROR)
        quit = true;
    while (!quit){
        render();
        update(evento);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(ventana);
    SDL_Quit();
}

void Juego::update(SDL_Event evento) {

    while (SDL_PollEvent(&evento) != 0) {
        input(evento);
    }
}

void Juego::input(SDL_Event evento){
    if (evento.type == SDL_KEYDOWN) {
        switch(evento.key.keysym.sym){
            case (SDLK_ESCAPE):
                quit=true;
                break;
        }
    }
}

void Juego::render(){
    SDL_RenderClear(renderer);
    jugador_test->renderizar(renderer);
    SDL_RenderPresent(renderer);
}