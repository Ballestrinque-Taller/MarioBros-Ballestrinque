#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

#include "Jugador.h"
#include "Juego.h"

#define ERROR -1

#define ANCHO_VENTANA 800
#define ALTO_VENTANA 600

#define DERECHA 1
#define IZQUIERDA -1

#define FPS 60
#define FRAME_DELAY 1000/FPS


Juego::Juego() {
    estado_error = Juego::inicializar_ventana();
    jugador = new Jugador(renderer);
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
    // Carga del icono
    std::string fileName = "../res/icono_mario.png";
    SDL_Surface* icono_surface = IMG_Load(fileName.c_str());
    if(icono_surface == NULL){
        std::cout << "No cargo el Icono " << std::endl;
        return ERROR;
    }
    SDL_SetColorKey( icono_surface, SDL_TRUE, SDL_MapRGB( icono_surface->format, 0xFF, 0xFF, 0xFF ) );
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
        update(evento);
        render();
        int frame_time = SDL_GetTicks()-frame_start;
        if(FRAME_DELAY>frame_time)
            SDL_Delay(FRAME_DELAY-frame_time);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(ventana);
    SDL_Quit();
}

void Juego::update(SDL_Event evento) {
    jugador->desplazar();
    while (SDL_PollEvent(&evento) != 0) {
        if (evento.type == SDL_QUIT)
            quit = true;
        jugador->recibir_evento(evento);
    }
}

void Juego::render(){
    SDL_RenderClear(renderer);
    jugador->cambiar_frame(renderer);
    SDL_RenderPresent(renderer);
}
