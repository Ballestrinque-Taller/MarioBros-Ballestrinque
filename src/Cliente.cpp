#include "Cliente.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "Log.h"

Cliente::Cliente(){
    inicializar_ventana();
    //mostrar_login();
    SET_LOGGING_LEVEL(Log::DEBUG);
}

//VER COMO IMPLEMENTAR LOGIN Y QUE EN EL LOGIN TE ASIGNE UN JUGADOR AL LOGGEARTE.

int Cliente::inicializar_ventana(){
    LOG(Log::INFO) << "Inicializando ventana de la aplicacion." << std::endl;
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
    if(icono_surface == nullptr){
        LOG(Log::ERROR)<<"No cargo el icono de la aplicacion. Valor del surface: " << icono_surface << ". Path: " << fileName << std::endl;
        return ERROR_JUEGO;
    }
    SDL_SetWindowIcon(ventana,icono_surface);
    SDL_FreeSurface(icono_surface);

    return 0;
}

void Cliente::bucle_juego(){
    SDL_Event evento;
    while (!quit){
        while (SDL_PollEvent(&evento) != 0) {
            if (evento.type == SDL_QUIT)
                quit = true;
            jugador->recibir_evento(evento);
        }
        //ENVIAR EVENTO AL SERVIDOR
        //PEDIR AL SERVIDOR EL RENDER
    }
}