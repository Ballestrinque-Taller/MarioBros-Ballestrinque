#ifndef MARIOBROS_BALLESTRINQUE_JUEGO_H
#define MARIOBROS_BALLESTRINQUE_JUEGO_H

#include <SDL2/SDL.h>

#include "Jugador.h"
#include "Enemigo.h"
#include "Camara.h"


class Juego {
    private:
        SDL_Window * ventana = nullptr;
        SDL_Renderer * renderer = nullptr;
        Camara* camara;
        Jugador* jugador;
        Enemigo* enemigo;
        bool quit = false;
        int estado_error;
        int inicializar_ventana();
        Enemigo* enemigos[100];
        int cant_enemigos;


    public:
        void update(SDL_Event evento);
        void render();
        Juego();
        void game_loop();
};


#endif //MARIOBROS_BALLESTRINQUE_JUEGO_H
