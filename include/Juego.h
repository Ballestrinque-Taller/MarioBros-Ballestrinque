#ifndef MARIOBROS_BALLESTRINQUE_JUEGO_H
#define MARIOBROS_BALLESTRINQUE_JUEGO_H

#include <SDL2/SDL.h>

#include "Jugador.h"
#include "Enemigo.h"
#include "Camara.h"
#include "Escenario.h"


class Juego {
    private:
        SDL_Window * ventana = nullptr;
        SDL_Renderer * renderer = nullptr;
        Background* background;
        Camara* camara;
        Jugador* jugador;
        Enemigo* enemigo;
        Escenario* escenario;
        bool quit = false;
        int estado_error;
        int inicializar_ventana();
        Enemigo* enemigos[100];
        int cant_enemigos;

        Escenario* escenarios[50];
        int cant_escenarios;

    public:
        void update(SDL_Event evento);
        void render();
        Juego();
        void game_loop();
};


#endif //MARIOBROS_BALLESTRINQUE_JUEGO_H
