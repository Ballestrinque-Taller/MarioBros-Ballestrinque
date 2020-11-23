#include "Servidor.h"
#include "Log.h"
#include "Mensajes.h"
#include <arpa/inet.h>
#define ERROR_SVR -1
#define MAX_CONEXIONES 10



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

#define TIEMPO_MAX_SIN_CONEXION 15000
#define EXIT_GAME -1
#define RECIBIENDO_MENSAJES 0
#define NO_RECIBIENDO_MENSAJES -2


Servidor::Servidor(std::string ip, int puerto){
    SET_LOGGING_LEVEL(Log::DEBUG);
    socket_svr = socket(AF_INET,SOCK_STREAM,0);
    if (socket_svr == ERROR_SVR){
        LOG(Log::ERROR)<<"No se pudo crear el socket."<<std::endl;
        exit(ERROR_SVR);
    }
    svr_address.sin_family = AF_INET;
    svr_address.sin_addr.s_addr = INADDR_ANY;
    //svr_address.sin_addr.s_addr = inet_addr(ip.c_str());
    svr_address.sin_port = puerto;
    if(bind(socket_svr, (sockaddr*)&svr_address, sizeof(struct sockaddr)) == ERROR_SVR){
        LOG(Log::ERROR)<<"No se pudo bindear el socket."<<std::endl;
        exit(ERROR_SVR);
    }
    if(listen(socket_svr, MAX_CONEXIONES) == ERROR_SVR){
        LOG(Log::ERROR)<<"No se pudo poner a escuchar al socket."<<std::endl;
        exit(ERROR_SVR);
    }
    pthread_mutex_init(&mutex_desplazamiento, nullptr);
}

int Servidor::generar_conexion(){
    conexiones.push_back(accept(socket_svr, nullptr,0));
    if(conexiones.back() == ERROR_SVR){
        LOG(Log::ERROR)<<"No se pudo aceptar la conexión del client_adress"<<std::endl;
        conexiones.pop_back();
        return ERROR_SVR;
    }
    pthread_t* thread_cliente = (pthread_t*)malloc(sizeof(pthread_t));
    pthread_create(thread_cliente, nullptr, reinterpret_cast<void *(*)(void *)>(Servidor::intercambiar_mensajes), this);
    threads.push_back(thread_cliente);
    return 0;
}

void Servidor::enviar_mensaje(int num_cliente){
    Jugador* jugador_final;
    std::vector<entidad_t> mensajes;

    mensajes.push_back(obtener_mensaje(background));

    for (auto & escenario:escenarios){
        mensajes.push_back(obtener_mensaje(escenario));
    }

    for (auto & enemigo: enemigos){
        mensajes.push_back(obtener_mensaje(enemigo));
    }

    for(int i=0; i<jugadores.size(); i++){
        if (i==num_cliente){
            jugador_final = jugadores[i];
        }else{
            mensajes.push_back(obtener_mensaje(jugadores[i]));
        }
    }

    mensajes.push_back(obtener_mensaje(jugador_final));
    for (auto & mensaje : mensajes) {
        bucle_send(&mensaje, conexiones.at(num_cliente));
    }
}

int Servidor::bucle_send(entidad_t* mensaje_ptr, int socket){
    int bytes_struct = sizeof(entidad_t);
    int total_bytes_enviados = 0;
    int bytes_enviados = 0;
    bool enviando = true;

    while((bytes_struct>total_bytes_enviados) &&(enviando)) {
        bytes_enviados = send(socket, (mensaje_ptr+total_bytes_enviados), (sizeof(mensaje_servidor_a_cliente_t)-total_bytes_enviados), MSG_NOSIGNAL);
        if (bytes_enviados < 0) {
            LOG(Log::ERROR) << "No se pudo enviar el mensaje al cliente: "<<socket<<". Error number: " << errno <<  std::endl;
            return EXIT_GAME;
        } else if (bytes_enviados == 0) {
            enviando = false;
        } else {
            total_bytes_enviados += bytes_enviados;
        }
    }
    return 0;
}

entidad_t Servidor::obtener_mensaje(Renderer* render){
    entidad_t entidad;
    entidad.path_textura = render->get_path_img();
    entidad.default_path = render->get_def_path();
    entidad.dest_rect = render->get_dest_rect();
    entidad.src_rect = render->get_src_rect();
    entidad.flip = render->get_flip();
    return entidad;
}

void Servidor::intercambiar_mensajes(Servidor* servidor){
    int conexion = RECIBIENDO_MENSAJES;
    int cliente = servidor->get_cantidad_de_conexiones() - 1;
    size_t tiempo_sin_reconexion = 0;
    size_t primer_tiempo_sin_reconexion = 0;
    while(conexion == RECIBIENDO_MENSAJES || ((conexion == NO_RECIBIENDO_MENSAJES) && (tiempo_sin_reconexion < TIEMPO_MAX_SIN_CONEXION))) {
        conexion = servidor->recibir_mensaje(cliente);
        servidor->enviar_mensaje(cliente);
        if(conexion == NO_RECIBIENDO_MENSAJES && primer_tiempo_sin_reconexion == 0){
            primer_tiempo_sin_reconexion = SDL_GetTicks();
        }else if(conexion == NO_RECIBIENDO_MENSAJES){
            tiempo_sin_reconexion = SDL_GetTicks() - primer_tiempo_sin_reconexion;
        }
        else{
            primer_tiempo_sin_reconexion = 0;
        }
    }
}

int Servidor::get_cantidad_de_conexiones(){
    return conexiones.size();
}

int Servidor::recibir_mensaje(int num_cliente){
    int total_bytes_recibidos = 0;
    int bytes_recibidos = 0;
    int bytes_struct = sizeof(mensaje_cliente_a_servidor_t);
    char* buffer = (char*)malloc(bytes_struct);
    bool recibiendo = true;
    while((bytes_struct>total_bytes_recibidos) &&(recibiendo)) {
        bytes_recibidos = recv(conexiones.at(num_cliente), (buffer + total_bytes_recibidos), (bytes_struct - total_bytes_recibidos), MSG_NOSIGNAL);
        if (bytes_recibidos < 0) {
            LOG(Log::ERROR) << "No se pudo recibir el mensaje. Error number: " << errno <<  std::endl;
            return EXIT_GAME;
        } else if (bytes_recibidos == 0) {
            recibiendo = false;
        } else {
            total_bytes_recibidos += bytes_recibidos;
        }
    }

    //PROCESAMIENTO DEL MENSAJE
    if (total_bytes_recibidos == bytes_struct) {
        Jugador* jugador = jugadores.at(num_cliente);
        pthread_mutex_lock(&mutex_desplazamiento);
        jugador->recibir_evento(((mensaje_cliente_a_servidor_t *) buffer)->evento);
        pthread_mutex_unlock(&mutex_desplazamiento);
    }
    free(buffer);
    if ((((mensaje_cliente_a_servidor_t *) buffer)->evento).type==SDL_QUIT)
        return EXIT_GAME;
    if(recibiendo)
        return RECIBIENDO_MENSAJES;
    return NO_RECIBIENDO_MENSAJES;
}

void Servidor::update() {
    temporizador->update();
    for (auto & jugador : jugadores)
        jugador->desplazar();
    for (auto & enemigo : enemigos){
        enemigo->desplazar();
    }
}

void Servidor::iniciar_juego(std::string path_xml){
    camara = new Camara();
    lectorXml = new LectorXML(path_xml);
    if (lectorXml->generar_nivel(&enemigos,&escenarios, &background, &temporizador, std::string("nivel1")) == ERROR_XML){
        lectorXml->set_default();
        lectorXml->generar_nivel(&enemigos,&escenarios, &background, &temporizador, std::string("nivel1"));
    }
    lectorXml->generar_jugador(&jugadores);
    nivel_actual = 1;
    nivel_label = new TextWriter();
    nivel_label->set_msg_rect(POS_X_TEXTO-WIDTH_TEXTO, POS_Y_TEXTO, HEIGHT_TEXTO, WIDTH_TEXTO);
    game_loop();
}

void Servidor::finalizar_juego(){
    LOG(Log::DEBUG) << "Finalizando juego" << std::endl;
    delete(camara);
    delete(lectorXml);
    for (auto & jugador : jugadores)
        delete(jugador);
    delete(background);
    delete(temporizador);
    for (size_t i=0; i<enemigos.size(); i++){
        delete(enemigos.at(i));
    };
    enemigos.clear();
    for (size_t i=0; i<escenarios.size(); i++){
        delete(escenarios.at(i));
    };
    escenarios.clear();
}

void Servidor::game_loop() {
    if (estado_error == ERROR_JUEGO)
        quit = true;
    while (!quit){
        while (!background->es_fin_nivel() && !quit) {
            //LOS THREADS YA ESTAN RECIBIENDO Y ENVIANDO POR CADA CLIENTE
            update();
        }
        if(!quit) {
            nivel_actual++;
            camara->stop_scrolling();
            for (auto & jugador : jugadores)
                jugador->reset_posicion();
            std::string nivel_str = (std::string("nivel")+std::to_string(nivel_actual));
            if (lectorXml->generar_nivel(&enemigos, &escenarios, &background, &temporizador, nivel_str) == QUIT)
                quit = true;
        }
    }
    finalizar_juego();
}

Servidor::~Servidor() {
    for(auto & thread:threads){
        pthread_join(*thread,0);
        free(thread);
    }
}

in_addr_t Servidor::get_ip(){
    return svr_address.sin_addr.s_addr;
}

