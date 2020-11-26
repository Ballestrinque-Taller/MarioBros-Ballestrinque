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

typedef struct entidad{
    char path_textura[256];
    char default_path[256];
    SDL_RendererFlip flip;
    SDL_Rect src_rect;
    SDL_Rect dest_rect;
}entidad_t;

typedef struct mensaje_servidor_a_cliente{
    entidad_t entidad;
    int num_nivel;
    int cantidad_entidades;
}mensaje_servidor_a_cliente_t;


#endif //MARIOBROS_BALLESTRINQUE_MENSAJES_H
