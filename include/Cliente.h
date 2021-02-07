#ifndef MARIOBROS_BALLESTRINQUE_CLIENTE_H
#define MARIOBROS_BALLESTRINQUE_CLIENTE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "Jugador.h"
#include "Dibujador.h"
#include "Mensajes.h"
#include <unistd.h>
#include "ReproductorDeSonido.h"

#define ERROR_JUEGO -1

#define ANCHO_VENTANA 800
#define ALTO_VENTANA 600

#define DERECHA 1
#define IZQUIERDA -1

#define FPS 30
#define FRAME_DELAY 1000/FPS

#define POS_X_TEXTO 800
#define POS_Y_TEXTO 0
#define HEIGHT_TEXTO 30
#define WIDTH_TEXTO 90

#define POS_X_TEMP 0
#define POS_Y_TEMP 0
#define WIDTH_MSG_TEMP 90
#define HEIGHT_MSG_TEMP  30

class Cliente{
    private:
        bool game_over();
        bool error_svr = false;
        int recibir_estado_conex_servidor();
        void mostrar_login(std::string, int puerto);
        void enviar_credenciales(std::string usuario, std::string password);
        int tiempo_restante_timer = 0;
        TextWriter* nivel_label = nullptr;
        TextWriter* temporizador_label = nullptr;
        int socket_cliente;
        int nivel_recibido = 0;
        int nivel_actual=0;
        std::vector<entidad_t> entidades;
        bool quit = false;
        SDL_Window * ventana = nullptr;
        SDL_Renderer * renderer = nullptr;
        int inicializar_ventana();
        Dibujador* dibujador = nullptr;
        void enviar_evento_a_servidor(mensaje_cliente_a_servidor_t* mensaje_ptr);
        pthread_t thread_render;
        int login(std::string ip, int puerto);
        bool render_iniciado = false;
        pthread_mutex_t mutex_render;
        ReproductorDeSonido* reproductorDeSonido = nullptr;

    public:
        Cliente(std::string, int puerto);
        void bucle_juego();
        static void render_thread(Cliente* cliente);
        void render();
        ~Cliente();
        void recibir_renders_del_servidor();

        //FINES DE TESTING
        std::vector<entidad_t> get_entidades();

};

#endif //MARIOBROS_BALLESTRINQUE_CLIENTE_H
