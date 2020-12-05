#include "Cliente.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "Log.h"
#include "Mensajes.h"

#define ANCHO_LOGO 176
#define ALTO_LOGO 88

#define ANCHO_BG 298
#define ALTO_BG 224

#define STOP_RECEPTION_AND_TRANSMISSION 2
#define TIMEOUT 11

Cliente::Cliente(std::string ip, int puerto){
    SET_LOGGING_LEVEL(Log::DEBUG);
    socket_cliente = socket(AF_INET,SOCK_STREAM,0);
    if (socket_cliente == ERROR_JUEGO){
        LOG(Log::ERROR)<<"No se pudo crear el socket."<<std::endl;
        exit(ERROR_JUEGO);
    }
    pthread_mutex_init(&mutex_render, nullptr);
    inicializar_ventana();
    mostrar_login(ip, puerto);
    login(ip, puerto);
}

void generar_textura_logo(SDL_Texture** textura_logo, SDL_Rect* dest_rect_logo, SDL_Rect* src_rect_logo, SDL_Renderer* renderer){
    SDL_Surface* logo_s = IMG_Load("./res/Logo.png");
    (*textura_logo) = SDL_CreateTextureFromSurface(renderer, logo_s);
    SDL_FreeSurface(logo_s);
    dest_rect_logo->x = 400-ANCHO_LOGO*3/2;
    dest_rect_logo->y = 0;
    dest_rect_logo->h = ALTO_LOGO*3;
    dest_rect_logo->w = ANCHO_LOGO*3;
    src_rect_logo->x = 0;
    src_rect_logo->y = 0;
    src_rect_logo->h = ALTO_LOGO;
    src_rect_logo->w = ANCHO_LOGO;
}

void generar_textura_background(SDL_Texture** textura_bg, SDL_Rect* dest_rect_bg, SDL_Rect* src_rect_bg, SDL_Renderer* renderer){
    SDL_Surface* logo_s = IMG_Load("./res/Nivel_1_sinTuberia.png");
    (*textura_bg) = SDL_CreateTextureFromSurface(renderer, logo_s);
    SDL_FreeSurface(logo_s);
    dest_rect_bg->x = 0;
    dest_rect_bg->y = 0;
    dest_rect_bg->h = ALTO_BG*2.68;
    dest_rect_bg->w = ANCHO_BG*2.68;
    src_rect_bg->x = 0;
    src_rect_bg->y = 0;
    src_rect_bg->h = ALTO_BG;
    src_rect_bg->w = ANCHO_BG;
}

void Cliente::mostrar_login(std::string ip, int puerto) {
    int estado_conexion = 99;
    auto *campo_usuario = new TextWriter();
    auto *usuario = new TextWriter();
    auto *campo_password = new TextWriter();
    auto *password = new TextWriter();
    auto *credenciales_invalidas = new TextWriter();
    SDL_Texture* logo_t;
    SDL_Texture* background_t;
    SDL_Rect dest_rect_logo;
    SDL_Rect src_rect_logo;
    SDL_Rect dest_rect_bg;
    SDL_Rect src_rect_bg;
    generar_textura_logo(&logo_t, &dest_rect_logo, &src_rect_logo, renderer);
    generar_textura_background(&background_t, &dest_rect_bg, &src_rect_bg, renderer);
    bool quit = false;
    std::vector<std::string> inputs;
    inputs.push_back(std::string(""));
    inputs.push_back(std::string(""));
    campo_usuario->set_msg_rect(100, 300, 50, 160);
    campo_password->set_msg_rect(100, 420, 50, 160);
    credenciales_invalidas->set_msg_rect(400-580/2,530,50, 580);
    int string_seleccionado = 0;
    SDL_Event evento_input;
    while(estado_conexion != CONECTADO && estado_conexion != JUEGO_LLENO && !quit) {
        while (SDL_PollEvent(&evento_input) != 0) {
            if (evento_input.type == SDL_QUIT){
                quit = true;
            }
            if (evento_input.type == SDL_KEYDOWN) {
                SDL_StartTextInput();
                if (evento_input.key.keysym.sym == SDLK_BACKSPACE) {
                    SDL_StartTextInput();
                    if (!inputs.at(string_seleccionado).empty())
                        inputs.at(string_seleccionado).pop_back();
                    SDL_StopTextInput();
                }else if (evento_input.key.keysym.sym == SDLK_RETURN){
                    login(ip, puerto);
                    enviar_credenciales(inputs.at(0), inputs.at(1));
                    estado_conexion = recibir_estado_conex_servidor();
                    if(estado_conexion == CREDENCIALES_INVALIDAS){
                        close(socket_cliente);
                        socket_cliente = socket(AF_INET,SOCK_STREAM,0);
                    }
                }else if (evento_input.key.keysym.sym == SDLK_TAB){
                    string_seleccionado = (string_seleccionado+1) % 2;
                }
                else {
                    SDL_StartTextInput();
                    inputs.at(string_seleccionado).push_back(evento_input.key.keysym.sym);
                    SDL_StopTextInput();
                }
                SDL_StopTextInput();
            }
        }
        pthread_mutex_lock(&mutex_render);
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, background_t, &src_rect_bg, &dest_rect_bg);
        SDL_RenderCopy(renderer, logo_t, &src_rect_logo, &dest_rect_logo);

        campo_usuario->write_text("Usuario: ", renderer);

        usuario->set_msg_rect(260, 300, 50, inputs.at(0).size()*20);
        usuario->write_text(inputs.at(0).c_str(), renderer);

        campo_password->write_text("Password: ", renderer);

        password->set_msg_rect(260, 420, 50, inputs.at(1).size()*20);
        password->write_text(inputs.at(1).c_str(), renderer);

        if(estado_conexion == CREDENCIALES_INVALIDAS){
            credenciales_invalidas->write_text("Error: Credenciales Invalidas", renderer);
        }

        SDL_RenderPresent(renderer);
        pthread_mutex_unlock(&mutex_render);
    }
    SDL_DestroyTexture(logo_t);
    SDL_DestroyTexture(background_t);
    delete(usuario);
    delete(campo_usuario);
    delete(campo_password);
    delete(password);
    if (estado_conexion == CONECTADO){
        bucle_juego();
    }
    else{
        close(socket_cliente);
    }
}

void Cliente::enviar_credenciales(std::string usuario, std::string password){
    credenciales_t credenciales;
    strcpy(credenciales.password, password.c_str());
    strcpy(credenciales.usuario, usuario.c_str());
    int bytes_struct = sizeof(credenciales_t);
    int total_bytes_enviados = 0;
    size_t bytes_enviados = 0;
    bool enviando = true;
    while((bytes_struct>total_bytes_enviados)) {
        bytes_enviados = send(socket_cliente, ((char*)&credenciales)+total_bytes_enviados, sizeof(credenciales_t)-total_bytes_enviados, MSG_NOSIGNAL);
        if (bytes_enviados < 0) {
            LOG(Log::ERROR) << "No se pudo enviar las credenciales al servidor: "<<socket<<". Error number: " << errno <<  std::endl;
        } else if (bytes_enviados == 0) {
            enviando = false;
        } else {
            total_bytes_enviados += bytes_enviados;
        }
    }
}

int Cliente::recibir_estado_conex_servidor(){
    int total_bytes_recibidos = 0;
    int bytes_recibidos = 0;
    int bytes_struct = sizeof(mensaje_retorno_conexion_t);
    bool recibiendo = true;
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
    mensaje_retorno_conexion_t mensaje;
    mensaje.estado_conexion = (((mensaje_retorno_conexion_t*)buffer)->estado_conexion);
    free(buffer);
    return mensaje.estado_conexion;
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
    //inicializar_ventana();
    SDL_DestroyRenderer(renderer);
    renderer = SDL_CreateRenderer(ventana, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
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