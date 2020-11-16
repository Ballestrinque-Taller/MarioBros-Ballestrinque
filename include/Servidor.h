#ifndef MARIOBROS_BALLESTRINQUE_SERVIDOR_H
#define MARIOBROS_BALLESTRINQUE_SERVIDOR_H

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <vector>
#include "Mensajes.h"
#include "Jugador.h"
#include "Enemigo.h"
#include "Camara.h"
#include "Escenario.h"
#include "LectorXML.h"
#include <vector>
#include "Temporizador.h"

struct Mensaje{
    //WEA A IMPLEMENTAR
}mensaje_t;

class Servidor{
    private:
        //Cosas de Juego
        TextWriter* nivel_label;
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
        Temporizador* temporizador = nullptr;
        void update();
        void game_loop();

        //Cosas de sockets
        int socket_svr;
        sockaddr_in svr_address;
        std::vector<int> conexiones;
        mensaje_servidor_a_cliente_t obtener_mensaje();

        //Cosas de Threads
        std::vector<pthread_t*> threads;
        pthread_mutex_t mutex_desplazamiento;

        static void intercambiar_mensajes(Servidor* servidor);


    public:
        //Cosas de sockets
        Servidor();
        int generar_conexion(sockaddr* client_address);
        int recibir_mensaje(int sock_cliente);
        void enviar_mensaje(int sock_cliente);

        //Cosas de Threads
        int get_cantidad_de_conexiones();

        //Cosas de juego
        void iniciar_juego(std::string path_xml);
        void finalizar_juego();
};

#endif //MARIOBROS_BALLESTRINQUE_SERVIDOR_H
