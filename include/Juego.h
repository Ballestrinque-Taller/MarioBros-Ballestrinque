#ifndef MARIOBROS_BALLESTRINQUE_JUEGO_H
#define MARIOBROS_BALLESTRINQUE_JUEGO_H

#include <SDL2/SDL.h>

#include "Jugador.h"
#include "Enemigo.h"
#include "Camara.h"
#include "Escenario.h"
#include "LectorXML.h"
#include <vector>


class Juego {
    private:
        SDL_Window * ventana = nullptr;
        SDL_Renderer * renderer = nullptr;
        LectorXML* lectorXml = nullptr;
        Background* background = nullptr;
        Camara* camara;
        Jugador* jugador;
        bool quit = false;
        int estado_error;
        int inicializar_ventana();
        std::vector<Enemigo*> enemigos;
        std::vector<Escenario*> escenarios;

    public:
        void update(SDL_Event evento);
        void render();
        Juego();
        ~Juego();
        void game_loop();
};


#endif //MARIOBROS_BALLESTRINQUE_JUEGO_H
