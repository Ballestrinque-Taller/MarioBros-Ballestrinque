#ifndef MARIOBROS_BALLESTRINQUE_JUEGO_H
#define MARIOBROS_BALLESTRINQUE_JUEGO_H

#include <SDL2/SDL.h>

#include "Jugador.h"
#include "Enemigo.h"
#include "Camara.h"
#include "Escenario.h"
#include "LectorXML.h"
#include <vector>
#include "Temporizador.h"




class Juego {
    private:
      /*  TextWriter* nivel_label;
        int nivel_actual;
        SDL_Window * ventana = nullptr;
        SDL_Renderer * renderer = nullptr;
        LectorXML* lectorXml = nullptr;
        Background* background = nullptr;
        Camara* camara;
        std::vector<Jugador*> jugadores;
        bool quit = false;
        int estado_error;
        std::vector<Enemigo*> enemigos;
        std::vector<Escenario*> escenarios;
        Temporizador* temporizador = nullptr;*/

    public:
        void render(SDL_Renderer** renderer_cliente); //VER QUE PASA CON ESTO
        //Juego(std::string path_xml);
        ~Juego();
        void game_loop();
};


#endif //MARIOBROS_BALLESTRINQUE_JUEGO_H
