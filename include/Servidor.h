#ifndef MARIOBROS_BALLESTRINQUE_SERVIDOR_H
#define MARIOBROS_BALLESTRINQUE_SERVIDOR_H

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <vector>
#include <ctime>
#include "Mensajes.h"
#include "Renderer.h"
#include "Jugador.h"
#include "Enemigo.h"
#include "Camara.h"
#include "Escenario.h"
#include "LectorXML.h"
#include <vector>
#include "Temporizador.h"
#include <unistd.h>

//PARAMETROS SVR
#define MAX_CONEXIONES 4
#define TIEMPO_MAX_SIN_CONEXION 2

//ESTADOS MENSAJES
#define EXIT_GAME -1
#define RECIBIENDO_MENSAJES 0
#define NO_RECIBIENDO_MENSAJES -2

//ERRORES
#define ERROR_SVR -1

class Servidor{
    private:
        //Cosas de Juego
        bool juego_iniciado = false;
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
        mensaje_servidor_a_cliente_t obtener_mensaje(Renderer* render);


        //Cosas de Threads
        std::vector<pthread_t*> threads;
        pthread_mutex_t mutex_desplazamiento;

        static void intercambiar_mensajes(Servidor* servidor);


    public:
        //Cosas de sockets
        Servidor(std::string ip, int puerto);
        ~Servidor();
        int aceptar_conexion();
        int recibir_mensaje(int sock_cliente);
        void enviar_mensaje(int sock_cliente);
        in_addr_t get_ip();
        int bucle_send(mensaje_servidor_a_cliente_t* entidad, int socket);

        //Cosas de Threads
        int get_cantidad_de_conexiones();

        //Cosas de juego
        void iniciar_juego(std::string path_xml);
        void finalizar_juego();

        //TESTING. SOLO UTILIZAR EN TESTS
        void set_juego_iniciado();
        int get_socket();
};

#endif //MARIOBROS_BALLESTRINQUE_SERVIDOR_H
