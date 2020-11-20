#include "Cliente.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "Log.h"
#include "Mensajes.h"

Cliente::Cliente(){
    inicializar_ventana();
    dibujador = new Dibujador(renderer);
    //mostrar_login();
    SET_LOGGING_LEVEL(Log::DEBUG);
}

//VER COMO IMPLEMENTAR LOGIN.

int Cliente::inicializar_ventana(){
    LOG(Log::INFO) << "Inicializando ventana de la aplicacion." << std::endl;
    if (SDL_Init(SDL_INIT_VIDEO) != 0){
        LOG(Log::ERROR) << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return ERROR_JUEGO;
    }

    ventana = SDL_CreateWindow("Hermanos Mario", 100, 100, ANCHO_VENTANA, ALTO_VENTANA, SDL_WINDOW_SHOWN);
    if (ventana == nullptr){
        LOG(Log::ERROR) << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        return ERROR_JUEGO;
    }

    renderer = SDL_CreateRenderer(ventana, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr){
        SDL_DestroyWindow(ventana);
        LOG(Log::ERROR) << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return ERROR_JUEGO;
    }

    std::string fileName = "./res/icono_mario.png";
    SDL_Surface* icono_surface = IMG_Load(fileName.c_str());
    if(icono_surface == nullptr){
        LOG(Log::ERROR)<<"No cargo el icono de la aplicacion. Valor del surface: " << icono_surface << ". Path: " << fileName << std::endl;
        return ERROR_JUEGO;
    }
    SDL_SetWindowIcon(ventana,icono_surface);
    SDL_FreeSurface(icono_surface);

    return 0;
}

void Cliente::enviar_evento_a_servidor(mensaje_cliente_a_servidor_t* mensaje_ptr){
    int bytes_struct = sizeof(mensaje_cliente_a_servidor_t);
    int total_bytes_enviados = 0;
    size_t bytes_enviados = 0;
    bool enviando = true;

    while((bytes_struct>total_bytes_enviados) &&(enviando)) {
        bytes_enviados = send(socket_svr, (mensaje_ptr+total_bytes_enviados), (sizeof(mensaje_cliente_a_servidor_t)-total_bytes_enviados), MSG_NOSIGNAL);
        if (bytes_enviados < 0) {
            LOG(Log::ERROR) << "No se pudo enviar el mensaje al cliente: "<<socket<<". Error number: " << errno <<  std::endl;
        } else if (bytes_enviados == 0) {
            enviando = false;
        } else {
            total_bytes_enviados += bytes_enviados;
        }
    }
}

//TODO:
void Cliente::bucle_juego(){
    SDL_Event evento;
    while (!quit){
        //CORRE EN UN THREAD INDEPENDIENTE AL RENDER
        while (SDL_PollEvent(&evento) != 0) {
            if (evento.type == SDL_QUIT)
                quit = true;
            mensaje_cliente_a_servidor_t mensaje;
            mensaje.evento = evento;
            enviar_evento_a_servidor(&mensaje);
        }
        render();
    }
}

void Cliente::recibir_renders_del_servidor(){
    int total_bytes_recibidos = 0;
    int bytes_recibidos = 0;
    int bytes_struct = sizeof(mensaje_servidor_a_cliente_t);
    int cantidad_entidades_recibidas = 0;
    int cantidad_entidades_a_recibir = 1;
    bool recibiendo = true;
    while(cantidad_entidades_recibidas < cantidad_entidades_a_recibir) {
        char *buffer = (char *) malloc(bytes_struct);
        while ((bytes_struct > total_bytes_recibidos) && (recibiendo)) {
            bytes_recibidos = recv(socket_svr, (buffer + total_bytes_recibidos),
                                   (bytes_struct - total_bytes_recibidos), MSG_NOSIGNAL);
            if (bytes_recibidos < 0) {
                LOG(Log::ERROR) << "No se pudo recibir el mensaje. Error number: " << errno << std::endl;
            } else if (bytes_recibidos == 0) {
                recibiendo = false;
            } else {
                total_bytes_recibidos += bytes_recibidos;
            }
        }

        //PROCESAMIENTO DEL MENSAJE
        if (total_bytes_recibidos == bytes_struct) {
            cantidad_entidades_recibidas++;
            entidades.push_back(((mensaje_servidor_a_cliente_t *) buffer)->entidad); //PUEDE LLEGAR A ROMPER
            if(cantidad_entidades_a_recibir == 1)
                cantidad_entidades_a_recibir = ((mensaje_servidor_a_cliente_t *) buffer)->cantidad_entidades;
        }
        free(buffer);
    }
}

//TODO:
void Cliente::render(){
    size_t frame_start = SDL_GetTicks();
    recibir_renders_del_servidor();
    dibujador->dibujar(entidades);
    size_t frame_time = SDL_GetTicks() - frame_start;
    if (FRAME_DELAY > frame_time)
        SDL_Delay(FRAME_DELAY - frame_time);
}