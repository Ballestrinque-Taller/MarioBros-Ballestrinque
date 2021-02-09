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
#define CAMBIANDO_NIVEL 255
#define FIN_JUEGO 254


Cliente::Cliente(std::string ip, int puerto){
    SET_LOGGING_LEVEL(Log::DEBUG);
    socket_cliente = socket(AF_INET,SOCK_STREAM,0);
    if (socket_cliente == ERROR_JUEGO){
        LOG(Log::ERROR)<<"No se pudo crear el socket."<<std::endl;
        exit(ERROR_JUEGO);
    }
    pthread_mutex_init(&mutex_render, nullptr);
    inicializar_ventana();
    reproductorDeSonido = new ReproductorDeSonido();
    mostrar_login(ip, puerto);
    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    if (setsockopt (socket_cliente, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout,sizeof(timeout)) < 0)
        LOG(Log::ERROR)<<"No se pudo settear el timeout del cliente recv a 5"<<std::endl;
    if (setsockopt (socket_cliente, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout,sizeof(timeout)) < 0)
        LOG(Log::ERROR)<<"No se pudo settear el timeout del cliente send a 5"<<std::endl;
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
    SDL_Surface* logo_s = IMG_Load("./res/Nivel_1.png");
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
    auto *retorno_servidor = new TextWriter();
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
    retorno_servidor->set_msg_rect(400 - 580 / 2, 550, 50, 580);
    int string_seleccionado = 0;
    SDL_Event evento_input;
    while(estado_conexion != CONECTADO && !quit) {
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
                    if(estado_conexion != CONECTADO){
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

        if(string_seleccionado == 0) {
            campo_usuario->write_text(">Usuario: ", renderer);
        }else{
            campo_usuario->write_text("Usuario: ", renderer);
        }

        usuario->set_msg_rect(260, 300, 50, inputs.at(0).size()*20);
        usuario->write_text(inputs.at(0).c_str(), renderer);
        if(string_seleccionado == 1) {
            campo_password->write_text(">Password: ", renderer);
        }else{
            campo_password->write_text("Password: ", renderer);
        }

        password->set_msg_rect(260, 420, 50, inputs.at(1).size()*20);
        password->write_text(inputs.at(1).c_str(), renderer);

        if(estado_conexion == CREDENCIALES_INVALIDAS){
            retorno_servidor->write_text("Error: Credenciales Invalidas.", renderer);
        }
        if(estado_conexion == CONECTADO){
            retorno_servidor->write_text("Esperando Jugadores...", renderer);
        }
        if(estado_conexion == TIMEOUT){
            retorno_servidor->write_text("Error: Connection Timeout.", renderer);
        }
        if(estado_conexion == JUEGO_LLENO){
            retorno_servidor->write_text("El juego esta lleno.", renderer);
        }
        if(estado_conexion == CREDENCIALES_YA_UTILIZADAS){
            retorno_servidor->write_text("Usuario ya conectado.", renderer);
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
    int bytes_enviados = 0;
    bool enviando = true;
    while((bytes_struct>total_bytes_enviados)) {
        bytes_enviados = send(socket_cliente, ((char*)&credenciales)+total_bytes_enviados, sizeof(credenciales_t)-total_bytes_enviados, MSG_NOSIGNAL);
        if (bytes_enviados < 0) {
            LOG(Log::ERROR) << "No se pudo enviar las credenciales al servidor: "<<socket<<". Error number: " << errno <<  std::endl;
            break;
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
    mensaje_retorno_conexion_t mensaje;
    while ((bytes_struct > total_bytes_recibidos)) {
        bytes_recibidos = recv(socket_cliente, (buffer + total_bytes_recibidos),(bytes_struct - total_bytes_recibidos), MSG_NOSIGNAL);
        if (bytes_recibidos < 0) {
            LOG(Log::ERROR) << "No se pudo recibir el mensaje. Error number: " << errno << std::endl;
            return TIMEOUT;
        } else if (bytes_recibidos == 0) {
            recibiendo = false;
        } else {
            total_bytes_recibidos += bytes_recibidos;
        }
    }
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
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0){
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
            quit = true;
            error_svr = true;
        } else if (bytes_enviados == 0) {
            enviando = false;
            quit = true;
            error_svr = true;
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
    bool musica_iniciada = false;
    while (!quit){
        if(nivel_recibido == 1 && !musica_iniciada){
            musica_iniciada = true;
            reproductorDeSonido->reproducir_musica();
        }
        render_iniciado = true;
        //CORRE EN UN THREAD INDEPENDIENTE AL RENDER QUE SE RALENTIZA A LOS FPS
        while (SDL_PollEvent(&evento) != 0) {
            if (evento.type == SDL_QUIT) {
                pthread_mutex_lock(&mutex_render);
                quit = true;
                pthread_mutex_unlock(&mutex_render);
            }
            if (evento.type == SDL_KEYDOWN && evento.key.keysym.sym == SDLK_m)
                reproductorDeSonido->toggle_musica();
            mensaje.evento = evento;
            enviar_evento_a_servidor(&mensaje);
        }
    }
    pthread_cancel(thread_render);
    pthread_join(thread_render, nullptr);
    if (error_svr){
        LOG(Log::ERROR)<<"El servidor se cerro. El cliente se cerrará"<<std::endl;
        SDL_ShowSimpleMessageBox(0,"Error: Server Shutdown", "El servidor ha sido cerrado y se procede con el cierre del cliente.", ventana);
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
                quit = true;
                error_svr = true;
            } else if (bytes_recibidos == 0) {
                recibiendo = false;
                quit=true;
                error_svr = true;
            } else {
                total_bytes_recibidos += bytes_recibidos;
            }
        }

        //PROCESAMIENTO DEL MENSAJE
        if (total_bytes_recibidos == bytes_struct) {
            cantidad_entidades_recibidas++;
            entidad_t entidad = ((mensaje_servidor_a_cliente_t*)buffer)->entidad;
            if(entidad.es_jugador) {
                strcpy(entidad.usuario, (((mensaje_servidor_a_cliente_t *) buffer)->entidad).usuario);
                if((((mensaje_servidor_a_cliente_t*)buffer)->num_nivel) != CAMBIANDO_NIVEL)
                    reproductorDeSonido->reproducir_sonido(entidad.sonido_a_reproducir);
            }
            entidades.push_back(entidad);
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

bool Cliente::game_over(){
    bool gameOver = true;
    for (auto& entidad: entidades){
        if(entidad.es_jugador && !entidad.muerto) {
            gameOver = false;
            break;
        }
    }
    if(tiempo_restante_timer<= 0 && !gameOver){
        gameOver = true;
    }
    return gameOver;
}

void Cliente::render(){
    SDL_DestroyRenderer(renderer);
    renderer = SDL_CreateRenderer(ventana, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    dibujador = new Dibujador();
    recibir_renders_del_servidor();
    bool musica_estaba_encendida = true;
    bool sonido_fin_nivel_reproducido = false;
    while(!quit && !game_over() && nivel_recibido != FIN_JUEGO) {
        size_t frame_start = SDL_GetTicks();
        if (nivel_recibido > nivel_actual && (dibujador != nullptr && (CAMBIANDO_NIVEL != nivel_recibido)) && nivel_recibido != FIN_JUEGO) {
            sonido_fin_nivel_reproducido = false;
            if (!reproductorDeSonido->musica_encendida() && musica_estaba_encendida) {
                reproductorDeSonido->toggle_musica();
            }
            if (nivel_label != nullptr)
                delete(nivel_label);
            if (temporizador_label != nullptr)
                delete(temporizador_label);
            nivel_label = new TextWriter();
            temporizador_label = new TextWriter();
            nivel_label->set_msg_rect(POS_X_TEXTO-WIDTH_TEXTO, POS_Y_TEXTO, HEIGHT_TEXTO, WIDTH_TEXTO);
            temporizador_label->set_msg_rect(POS_X_TEMP,POS_Y_TEMP,HEIGHT_MSG_TEMP,WIDTH_MSG_TEMP);
            nivel_actual = nivel_recibido;
        }
        if(nivel_recibido == CAMBIANDO_NIVEL && nivel_actual < 1){
            dibujador->dibujar_cambio_nivel(entidades, "Nivel: "+std::to_string(nivel_actual + 1), renderer);
            size_t frame_time = SDL_GetTicks() - frame_start;
            if (FRAME_DELAY > frame_time)
                SDL_Delay(FRAME_DELAY - frame_time);
        }
        else if(nivel_recibido == CAMBIANDO_NIVEL && nivel_actual >=1){
            musica_estaba_encendida = reproductorDeSonido->musica_encendida();
            if(musica_estaba_encendida) {
                reproductorDeSonido->toggle_musica();
            }
            if(!sonido_fin_nivel_reproducido) {
                reproductorDeSonido->reproducir_sonido(SONIDO_FIN_DE_NIVEL);
                sonido_fin_nivel_reproducido = true;
            }
            dibujador->dibujar_cambio_nivel(entidades, "Nivel: "+std::to_string(nivel_actual + 1), renderer);
            dibujador->set_ronda_cambiada(false);
            size_t frame_time = SDL_GetTicks() - frame_start;
            if (FRAME_DELAY > frame_time)
                SDL_Delay(FRAME_DELAY - frame_time);
        }else{
            dibujador->set_ronda_cambiada(true);
            pthread_mutex_lock(&mutex_render);
            dibujador->crear_texturas(entidades, renderer);
            dibujador->dibujar(entidades, nivel_label, nivel_actual, temporizador_label, tiempo_restante_timer, renderer);
            pthread_mutex_unlock(&mutex_render);
            size_t frame_time = SDL_GetTicks() - frame_start;
            if (FRAME_DELAY > frame_time)
                SDL_Delay(FRAME_DELAY - frame_time);
        }
        recibir_renders_del_servidor();
    }
    if(reproductorDeSonido != nullptr) {
        if (reproductorDeSonido->musica_encendida())
            reproductorDeSonido->toggle_musica();
    }

    size_t tiempo_desde_muertes = SDL_GetTicks();
    bool reproduci_sonido_game_over = false;
    while(!quit && game_over()){
        recibir_renders_del_servidor();
        size_t frame_start = SDL_GetTicks();
        dibujador->dibujar_cambio_nivel(entidades, "Game Over", renderer);
        dibujador->set_ronda_cambiada(false);
        size_t frame_time = SDL_GetTicks() - frame_start;
        if((SDL_GetTicks()-tiempo_desde_muertes > 3000 || tiempo_restante_timer <= 0) && !reproduci_sonido_game_over){
            reproduci_sonido_game_over = true;
            reproductorDeSonido->reproducir_sonido(SONIDO_GAME_OVER);
        }
        if (FRAME_DELAY > frame_time)
            SDL_Delay(FRAME_DELAY - frame_time);
    }
    if(nivel_recibido == FIN_JUEGO && reproductorDeSonido != nullptr){
        reproductorDeSonido->reproducir_musica(MUSICA_VICTORIA);
        recibir_renders_del_servidor();
        dibujador->dibujar_fin_juego(entidades, renderer);
    }
    while(!quit && nivel_recibido == FIN_JUEGO){
        size_t frame_start = SDL_GetTicks();
        //dibujador->set_ronda_cambiada(false);
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
    if(reproductorDeSonido != nullptr)
        delete(reproductorDeSonido);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(ventana);
    SDL_Quit();
}

std::vector<entidad_t> Cliente::get_entidades(){
    return entidades;
}