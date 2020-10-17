#ifndef MARIOBROS_BALLESTRINQUE_JUEGO_H
#define MARIOBROS_BALLESTRINQUE_JUEGO_H

#include <SDL2/SDL.h>

#include "Jugador.h"


class Juego {
    private:
        SDL_Window * ventana = nullptr;
        SDL_Renderer * renderer = nullptr;
        Jugador* jugador;
        bool quit = false;
        int estado_error;
        int inicializar_ventana();


    public:
        void update(SDL_Event evento);
        void render();
        void input(SDL_Event evento);
        Juego();
        void game_loop();
};


#endif //MARIOBROS_BALLESTRINQUE_JUEGO_H
