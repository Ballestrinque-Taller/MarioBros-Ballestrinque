#ifndef MARIOBROS_BALLESTRINQUE_MENSAJES_H
#define MARIOBROS_BALLESTRINQUE_MENSAJES_H

#include <SDL2/SDL.h>
#include <vector>
#include "Enemigo.h"
#include "Jugador.h"
#include "Escenario.h"
#include "Temporizador.h"
#include "Background.h"
#include "Camara.h"

typedef struct mensaje_cliente_a_servidor{
    SDL_Event evento;
}mensaje_cliente_a_servidor_t;

//TODO:
typedef struct mensaje_servidor_a_cliente{
    std::vector<Jugador> jugadores;
    std::vector<Enemigo> enemigos;
    std::vector<Escenario> escenarios;
    Background* background;
    Camara* camara;
    Temporizador* temporizador;
}mensaje_servidor_a_cliente_t;


#endif //MARIOBROS_BALLESTRINQUE_MENSAJES_H
