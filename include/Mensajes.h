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
#define CREDENCIALES_YA_UTILIZADAS 3

typedef struct mensaje_cliente_a_servidor{
    SDL_Event evento;
}mensaje_cliente_a_servidor_t;

typedef struct entidad{
    char path_textura[40];
    char default_path[40];
    SDL_RendererFlip flip;
    SDL_Rect src_rect;
    SDL_Rect dest_rect;
    bool es_jugador;
    char usuario[15];
    uint8_t sonido_a_reproducir;
    size_t puntaje;
    uint8_t vidas;
    bool muerto;
}entidad_t;

typedef struct mensaje_servidor_a_cliente{
    entidad_t entidad;
    uint8_t num_nivel;
    uint16_t cantidad_entidades;
    uint16_t tiempo_restante;

}mensaje_servidor_a_cliente_t;

typedef struct credenciales{
    char usuario[15];
    char password[100];
}credenciales_t;

typedef struct mensaje_retorno_conexion{
    int estado_conexion;
}mensaje_retorno_conexion_t;


#endif //MARIOBROS_BALLESTRINQUE_MENSAJES_H
