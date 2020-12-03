#include "Cliente.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "Log.h"
#include "Mensajes.h"

#define STOP_RECEPTION_AND_TRANSMISSION 2
#define TIMEOUT 11

Cliente::Cliente(std::string ip, int puerto){
    SET_LOGGING_LEVEL(Log::DEBUG);
    socket_cliente = socket(AF_INET,SOCK_STREAM,0);
    if (socket_cliente == ERROR_JUEGO){
        LOG(Log::ERROR)<<"No se pudo crear el socket."<<std::endl;
        exit(ERROR_JUEGO);
    }
    //mostrar_login();
    login(ip, puerto); //Se fleta cuando tengamos la UI porque lo va a enviar el boton
    pthread_mutex_init(&mutex_render, nullptr);
}

int Cliente::login(std::string ip, int puerto){
    sockaddr_in address;
    address.sin_addr.s_addr = inet_addr(ip.c_str());
    address.sin_port = puerto;
    address.sin_family = AF_INET;
    int estado_conexion = connect(socket_cliente, (struct sockaddr*)&address, sizeof(address));
    return estado_conexion;
}

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
        bytes_enviados = send(socket_cliente, (mensaje_ptr+total_bytes_enviados), (sizeof(mensaje_cliente_a_servidor_t)-total_bytes_enviados), MSG_NOSIGNAL);
        if (bytes_enviados < 0) {
            LOG(Log::ERROR) << "No se pudo enviar el mensaje al servidor: "<<socket<<". Error number: " << errno <<  std::endl;
        } else if (bytes_enviados == 0) {
            enviando = false;
        } else {
            total_bytes_enviados += bytes_enviados;
        }
    }
}

void Cliente::bucle_juego(){
    SDL_Event evento;
    mensaje_cliente_a_servidor_t mensaje;
    pthread_create(&thread_render, nullptr, reinterpret_cast<void *(*)(void *)>(Cliente::render_thread), this);
    LOG(Log::INFO)<<"Iniciando Bucle de Juego."<<std::endl;
    while (!quit){
        render_iniciado = true;
        //CORRE EN UN THREAD INDEPENDIENTE AL RENDER QUE SE RALENTIZA A LOS FPS
        while (SDL_PollEvent(&evento) != 0) {
            if (evento.type == SDL_QUIT) {
                quit = true;
            }
            mensaje.evento = evento;
            enviar_evento_a_servidor(&mensaje);
        }
    }
}

void Cliente::recibir_renders_del_servidor(){
    int total_bytes_recibidos = 0;
    int bytes_recibidos = 0;
    int bytes_struct = sizeof(mensaje_servidor_a_cliente_t);
    int cantidad_entidades_recibidas = 0;
    int cantidad_entidades_a_recibir = 1;
    bool recibiendo = true;
    pthread_mutex_lock(&mutex_render);
    entidades.clear();
    while(cantidad_entidades_recibidas < cantidad_entidades_a_recibir) {
        char* buffer = (char*)malloc(bytes_struct);


        while ((bytes_struct > total_bytes_recibidos)) {
            bytes_recibidos = recv(socket_cliente, (buffer + total_bytes_recibidos),(bytes_struct - total_bytes_recibidos), MSG_NOSIGNAL);
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
            //LOG(Log::DEBUG) << "Mensaje recibido. Bytes: " << total_bytes_recibidos << std::endl;
            cantidad_entidades_recibidas++;
            entidades.push_back(((mensaje_servidor_a_cliente_t*)buffer)->entidad);
            if(cantidad_entidades_a_recibir == 1)
                cantidad_entidades_a_recibir = ((mensaje_servidor_a_cliente_t *) buffer)->cantidad_entidades;
            nivel_recibido = (((mensaje_servidor_a_cliente_t*)buffer)->num_nivel);
            tiempo_restante_timer = (((mensaje_servidor_a_cliente_t*)buffer)->tiempo_restante);
            total_bytes_recibidos = 0;
        }
        pthread_mutex_unlock(&mutex_render);
        free(buffer);
    }
}

void Cliente::render_thread(Cliente* cliente){
    cliente->render();
}

void Cliente::render(){
    inicializar_ventana();
    dibujador = new Dibujador();
    while(!quit) {
        size_t frame_start = SDL_GetTicks();
        recibir_renders_del_servidor();
        if (nivel_recibido > nivel_actual && dibujador != nullptr) {
            if (nivel_label != nullptr)
                delete(nivel_label);
            if (temporizador_label != nullptr)
                delete(temporizador_label);
            nivel_label = new TextWriter();
            temporizador_label = new TextWriter();
            nivel_label->set_msg_rect(POS_X_TEXTO-WIDTH_TEXTO, POS_Y_TEXTO, HEIGHT_TEXTO, WIDTH_TEXTO);
            temporizador_label->set_msg_rect(POS_X_TEMP,POS_Y_TEMP,HEIGHT_MSG_TEMP,WIDTH_MSG_TEMP);
            pthread_mutex_lock(&mutex_render);
            dibujador->crear_texturas(entidades, renderer);
            pthread_mutex_unlock(&mutex_render);
            nivel_actual = nivel_recibido;
        }
        pthread_mutex_lock(&mutex_render);
        dibujador->dibujar(entidades, nivel_label, nivel_actual, temporizador_label, tiempo_restante_timer, renderer);
        pthread_mutex_unlock(&mutex_render);
        size_t frame_time = SDL_GetTicks() - frame_start;
        if (FRAME_DELAY > frame_time)
            SDL_Delay(FRAME_DELAY - frame_time);
    }
}

Cliente::~Cliente(){
    if (dibujador != nullptr)
        delete(dibujador);
    if (nivel_label != nullptr)
        delete(nivel_label);
    if (render_iniciado) {
        pthread_cancel(thread_render);
        pthread_join(thread_render, nullptr);
    }
    close(socket_cliente);
    shutdown(socket_cliente, STOP_RECEPTION_AND_TRANSMISSION);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(ventana);
    SDL_Quit();
}

std::vector<entidad_t> Cliente::get_entidades(){
    return entidades;
}