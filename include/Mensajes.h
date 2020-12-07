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

#define CONECTADO 0
#define JUEGO_LLENO 1
#define CREDENCIALES_INVALIDAS 2

typedef struct mensaje_cliente_a_servidor{
    SDL_Event evento;
}mensaje_cliente_a_servidor_t;

typedef struct entidad{
    char path_textura[128];
    char default_path[128];
    SDL_RendererFlip flip;
    SDL_Rect src_rect;
    SDL_Rect dest_rect;
    bool es_jugador;
    char usuario[100];
}entidad_t;

typedef struct mensaje_servidor_a_cliente{
    entidad_t entidad;
    int num_nivel;
    int cantidad_entidades;
    int tiempo_restante;
}mensaje_servidor_a_cliente_t;

typedef struct credenciales{
    char usuario[100];
    char password[100];
}credenciales_t;

typedef struct mensaje_retorno_conexion{
    int estado_conexion;
}mensaje_retorno_conexion_t;


#endif //MARIOBROS_BALLESTRINQUE_MENSAJES_H
