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
#include "Hongo.h"
#include "Camara.h"
#include "Escenario.h"
#include "LectorXML.h"
#include <vector>
#include "Temporizador.h"
#include <unistd.h>
#include "Colisionador.h"

//PARAMETROS SVR
#define TIEMPO_MAX_SIN_CONEXION 10
#define CAMBIANDO_NIVEL 255
#define FIN_JUEGO 254

//ESTADOS MENSAJES
#define EXIT_GAME -1
#define RECIBIENDO_MENSAJES 0
#define NO_RECIBIENDO_MENSAJES -2
#define JUEGO_INICIADO 3

//ERRORES
#define ERROR_SVR -1

class Colisionador;

class Servidor{
    private:
        //Cosas de Juego
        bool game_over();
        bool fin_juego = false;
        Colisionador* colisionador = nullptr;
        int cant_clientes_exit=0;
        bool juego_iniciado = false;
        bool quit = false;
        int nivel_actual = CAMBIANDO_NIVEL;
        LectorXML* lectorXml = nullptr;
        SDL_Window * ventana = nullptr;
        SDL_Renderer * renderer = nullptr;
        Background* background = nullptr;
        Camara* camara = nullptr;
        std::vector<Jugador*> jugadores;
        std::vector<std::string> usuarios;
        std::vector<std::string> usuarios_desconectados;
        std::vector<Hongo*> hongos;

        int estado_error;
        std::vector<Enemigo*> enemigos;
        std::vector<Escenario*> escenarios;
        std::vector<Moneda*> monedas;
        std::vector<Jugador*> jugadores_fin;
        Temporizador* temporizador = nullptr;
        void update();
        void game_loop();

        //Cosas de sockets
        int socket_svr;
        sockaddr_in svr_address;
        std::vector<int> conexiones;
        mensaje_servidor_a_cliente_t obtener_mensaje(Renderer* render);
        mensaje_servidor_a_cliente_t obtener_mensaje_jugador(Jugador* jugador, bool cliente_recibio_sonido);
        credenciales_t recibir_credenciales(int socket);
        void enviar_retorno_conexion(int socket, int retorno);
        int encontrar_pos_usuario_a_reconectar(int cliente);
        void reconectar_jugador_con_nuevo_cliente(int num_cliente);


        //Cosas de Threads
        int delay_envio_render_ms = 0;
        std::vector<pthread_t*> threads;
        pthread_t thread_conexiones;
        pthread_mutex_t mutex_desplazamiento;
        pthread_mutex_t mutex_render;
        int pos_de_conexion = -1;


        static void intercambiar_mensajes(Servidor* servidor);
        static void intercambiar_mensajes_reconexion(Servidor *servidor);
        int get_pos_de_conexion();
        void enviar_pantalla_entre_nivel();
        bool contiene_jugador(Jugador* jugador_buscado);


public:
        //Cosas de sockets
        int chequear_credenciales_validas(int socket);
        Servidor(std::string ip, int puerto, std::string path_xml);
        ~Servidor();
        int aceptar_conexion();
        int recibir_mensaje(int sock_cliente);
        void enviar_mensaje(int sock_cliente);
        in_addr_t get_ip();
        int bucle_send(mensaje_servidor_a_cliente_t* entidad, int num_cliente);
        void set_pos_de_conexion(int pos);

        //Cosas de Threads
        int get_cantidad_de_conexiones();
        static void aceptar_conexiones_thread(Servidor* servidor);
        void set_aceptando_conexiones_false();
        static void enviar_mensaje_thread(std::vector<void*>* parametros);
        bool aceptando_conexiones;

        //Cosas de juego
        bool juego_finalizado();
        void reconectar_jugador(int num_cliente);
        void grisar_jugador(int num_cliente);
        int get_cantidad_jugadores();
        void iniciar_juego();
        void finalizar_juego();
        void consumir_moneda(SDL_Rect pos_moneda);
        void matar_enemigo(SDL_Rect pos_enemigo);
        void spawn_hongo(int x, int y);
        void spawn_moneda(int x, int y);
        void consumir_hongo(SDL_Rect pos_hongo);


    //TESTING. SOLO UTILIZAR EN TESTS
        void set_juego_iniciado();
        int get_socket();
};

#endif //MARIOBROS_BALLESTRINQUE_SERVIDOR_H
