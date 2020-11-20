#ifndef MARIOBROS_BALLESTRINQUE_CLIENTE_H
#define MARIOBROS_BALLESTRINQUE_CLIENTE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <sys/socket.h>
#include "Jugador.h"
#include "Dibujador.h"
#include "Mensajes.h"

#define ERROR_JUEGO -1

#define ANCHO_VENTANA 800
#define ALTO_VENTANA 600

#define DERECHA 1
#define IZQUIERDA -1

#define FPS 60
#define FRAME_DELAY 1000/FPS

#define POS_X_TEXTO 800
#define POS_Y_TEXTO 0
#define HEIGHT_TEXTO 30
#define WIDTH_TEXTO 90

class Cliente{
    private:
        int socket_svr;
        std::vector<entidad_t> entidades;
        bool quit = false;
        SDL_Window * ventana = nullptr;
        SDL_Renderer * renderer = nullptr;
        int inicializar_ventana();
        void render();
        Dibujador* dibujador;
        void recibir_renders_del_servidor();
        void enviar_evento_a_servidor(mensaje_cliente_a_servidor_t* mensaje_ptr);
    public:
        Cliente();
        void bucle_juego();

};

#endif //MARIOBROS_BALLESTRINQUE_CLIENTE_H
