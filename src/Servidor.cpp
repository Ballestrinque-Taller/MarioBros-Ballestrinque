#include "Servidor.h"
#include "Log.h"
#include "Mensajes.h"
#include <arpa/inet.h>
#include <algorithm>


#define ERROR_JUEGO -1

#define ANCHO_VENTANA 800
#define ALTO_VENTANA 600

#define ANCHO_ENTIDAD -40

#define DERECHA 1
#define IZQUIERDA -1

#define FPS 30
#define FRAME_DELAY 1000/FPS

#define TIMEOUT 11
#define INTERRUPT 4

#define STOP_RECEPTION_AND_TRANSMISSION 2

#define TIEMPO_ENTRE_NIVEL 5


Servidor::Servidor(std::string ip, int puerto, std::string path_xml){
    colisionador = new Colisionador(this);
    lectorXml = new LectorXML(path_xml);
    SET_LOGGING_LEVEL(Log::DEBUG);
    socket_svr = socket(AF_INET,SOCK_STREAM,0);
    if (socket_svr == ERROR_SVR){
        LOG(Log::ERROR)<<"No se pudo crear el socket."<<std::endl;
        exit(ERROR_SVR);
    }
    int enable=1;
    if (setsockopt(socket_svr, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
        LOG(Log::ERROR) << "setsockopt(SO_REUSEADDR) failed" << std::endl;
    }
    int optval = 1;
    if(setsockopt(socket_svr, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval)) < 0){
        LOG(Log::ERROR) << "setsockopt(SO_REUSEPORT) failed" << std::endl;
    }
    svr_address.sin_family = AF_INET;
    //svr_address.sin_addr.s_addr = INADDR_ANY;
    svr_address.sin_addr.s_addr = inet_addr(ip.c_str());
    svr_address.sin_port = puerto;
    if(bind(socket_svr, (sockaddr*)&svr_address, sizeof(struct sockaddr)) == ERROR_SVR){
        LOG(Log::ERROR)<<"No se pudo bindear el socket. errno: "<<errno<<std::endl;
        exit(ERROR_SVR);
    }
    if(listen(socket_svr, lectorXml->get_cantidad_jugadores()+1) == ERROR_SVR){
        LOG(Log::ERROR)<<"No se pudo poner a escuchar al socket."<<std::endl;
        exit(ERROR_SVR);
    }
    while((float)FPS/1000*delay_envio_render_ms<(float)1){
        delay_envio_render_ms++;
    }
    pthread_mutex_init(&mutex_desplazamiento, nullptr);
    pthread_create(&thread_conexiones, nullptr, reinterpret_cast<void *(*)(void *)>(aceptar_conexiones_thread), this);
    aceptando_conexiones = true;
    std::cout << "Servidor Generado. IP: " << ip << ", Puerto: " << puerto << ". Cantidad MAX Jugadores: " << lectorXml->get_cantidad_jugadores() << std::endl;
}

void Servidor::aceptar_conexiones_thread(Servidor* servidor){
    while (servidor->get_cantidad_de_conexiones() < servidor->get_cantidad_jugadores() && servidor->aceptando_conexiones) {
        int retorno = servidor->aceptar_conexion();
        if (retorno == ERROR_SVR) {
            break;
        }
        servidor->set_pos_de_conexion(servidor->get_cantidad_de_conexiones()-1);
        servidor->enviar_retorno_conexion(servidor->get_cantidad_de_conexiones()-1, servidor->chequear_credenciales_validas(servidor->get_cantidad_de_conexiones()-1));
    }
    servidor->set_aceptando_conexiones_false();
    while (!servidor->juego_finalizado()) {
        int retorno = servidor->aceptar_conexion();
        if(retorno == ERROR_SVR)
            break;
        int pos_de_reconexion = servidor->encontrar_pos_usuario_a_reconectar(servidor->get_cantidad_de_conexiones()-1);
        if(pos_de_reconexion != -1){
            servidor->reconectar_jugador_con_nuevo_cliente(pos_de_reconexion);
        }else{
            servidor->enviar_retorno_conexion(servidor->get_cantidad_de_conexiones() - 1, JUEGO_LLENO);
        }
    }
    pthread_exit(nullptr);
}

int Servidor::encontrar_pos_usuario_a_reconectar(int cliente){
    credenciales_t credenciales = recibir_credenciales(conexiones.at(cliente));
    bool encontre = false;
    int pos = 0;
    for (pos=0; pos<usuarios_desconectados.size(); pos++){
        if(strcmp(usuarios_desconectados.at(pos).c_str(), credenciales.usuario) == 0){
            encontre = true;
            usuarios_desconectados.erase(usuarios_desconectados.begin()+pos);
            break;
        }
    }
    if(encontre) {
        for (pos = 0; pos < usuarios.size(); pos++){
            if(strcmp(usuarios.at(pos).c_str(),credenciales.usuario)==0){
                break;
            }
        }
    }
    if(!encontre)
        return -1;
    return pos;
}

void Servidor::reconectar_jugador_con_nuevo_cliente(int num_cliente) {
    cant_clientes_exit--;
    int conexion_nueva = conexiones.at(conexiones.size()-1);
    close(conexiones.at(num_cliente));
    conexiones.erase(conexiones.begin()+conexiones.size()-1);
    pthread_cancel(*(threads.at(num_cliente)));
    pthread_join(*(threads.at(num_cliente)), nullptr);
    free(threads.at(num_cliente));
    threads.erase(threads.begin()+num_cliente);
    pos_de_conexion = num_cliente;
    //pthread_create(threads.at(num_cliente), nullptr, reinterpret_cast<void *(*)(void *)>(Servidor::intercambiar_mensajes_reconexion),this);
    conexiones.emplace(conexiones.begin()+num_cliente, conexion_nueva);
    conexiones.erase(conexiones.begin()+num_cliente+1);
    enviar_retorno_conexion(num_cliente, CONECTADO);
    reconectar_jugador(num_cliente);
}

void Servidor::set_pos_de_conexion(int pos){
    pos_de_conexion = pos;
}

int Servidor::get_pos_de_conexion(){
    return pos_de_conexion;
}

bool Servidor::juego_finalizado(){
    return quit;
}

int Servidor::chequear_credenciales_validas(int cliente){
    credenciales_t credenciales = recibir_credenciales(conexiones.at(cliente));
    int retorno_credenciales;
    for (int i=0; i<usuarios.size();i++){
        std::string usuario = usuarios.at(i);
        if(strcmp(usuario.c_str(), credenciales.usuario) == 0)
            retorno_credenciales = CREDENCIALES_YA_UTILIZADAS;
    }
    bool posee_credenciales = lectorXml->posee_credenciales(credenciales);
    if (posee_credenciales && retorno_credenciales != CREDENCIALES_YA_UTILIZADAS){
        retorno_credenciales = CONECTADO;
        usuarios.push_back(credenciales.usuario);
    }else if(!posee_credenciales){
        retorno_credenciales = CREDENCIALES_INVALIDAS;
    }
    return retorno_credenciales;
}

void Servidor::enviar_retorno_conexion(int cliente, int retorno){
    if(retorno == CONECTADO) {
        std::cout << "Conexion aceptada del socket: " << conexiones.at(cliente) << std::endl;
        pthread_t *thread_cliente = (pthread_t *) malloc(sizeof(pthread_t));
        pthread_create(thread_cliente, nullptr, reinterpret_cast<void *(*)(void *)>(Servidor::intercambiar_mensajes),
                       this);
        threads.emplace(threads.begin()+cliente,thread_cliente);
    }

    mensaje_retorno_conexion_t mensaje;
    mensaje.estado_conexion = retorno;
    int bytes_struct = sizeof(mensaje_retorno_conexion_t);
    int total_bytes_enviados = 0;
    int bytes_enviados = 0;
    bool enviando = true;

    while((bytes_struct>total_bytes_enviados) &&(enviando)) {
        bytes_enviados = send(conexiones.at(cliente), ((char*)&retorno)+total_bytes_enviados, (sizeof(mensaje_retorno_conexion_t)-total_bytes_enviados),MSG_NOSIGNAL);
        if (bytes_enviados < 0) {
            //LOG(Log::DEBUG) << "No se pudo enviar el mensaje de conexion al cliente: "<<socket<<". Error number: " << errno <<  std::endl;
        } else if (bytes_enviados == 0) {
            enviando = false;
        } else {
            total_bytes_enviados += bytes_enviados;
        }
    }
    if(retorno != CONECTADO){
        int sock = conexiones.at(cliente);
        close(conexiones.at(cliente));
        conexiones.erase(conexiones.begin()+cliente);
        std::cout<<"Conexion rechazada del socket: "<<sock<<std::endl;
    }
}

credenciales_t Servidor::recibir_credenciales(int socket){
    credenciales_t credenciales;
    int total_bytes_recibidos = 0;
    int bytes_recibidos = 0;
    int bytes_struct = sizeof(credenciales_t);
    char* buffer = (char*)malloc(bytes_struct);
    bool recibiendo = true;
    while((bytes_struct>total_bytes_recibidos) && (recibiendo)) {
        bytes_recibidos = recv(socket, (buffer + total_bytes_recibidos), (bytes_struct - total_bytes_recibidos), MSG_NOSIGNAL);
        if (bytes_recibidos < 0 && errno != TIMEOUT) {
            //LOG(Log::ERROR) << "No se pudo recibir el mensaje. Error number: " << errno <<  std::endl;
            return credenciales;
        } else if (bytes_recibidos == 0 || errno == TIMEOUT) {
            recibiendo = false;
        } else {
            total_bytes_recibidos += bytes_recibidos;
        }
    }
    strcpy(credenciales.usuario, ((credenciales_t*)buffer)->usuario);
    strcpy(credenciales.password, ((credenciales_t*)buffer)->password);
    free (buffer);
    return credenciales;
}

void Servidor::set_aceptando_conexiones_false() {
    aceptando_conexiones = false;
}

int Servidor::aceptar_conexion(){
    conexiones.push_back(accept(socket_svr, nullptr, nullptr));
    if(conexiones.size() == lectorXml->get_cantidad_jugadores()) {
        return JUEGO_INICIADO;
    }
    if(conexiones.back() == ERROR_SVR){
        LOG(Log::ERROR)<<"No se pudo aceptar la conexión del client_address"<<std::endl;
        conexiones.pop_back();
        return ERROR_SVR;
    }
    return 0;
}

void Servidor::enviar_mensaje(int num_cliente){
    int num_entidades = 0;
    bool algun_jugador_desconectado = false;
    Jugador* jugador_final = nullptr;
    std::vector<mensaje_servidor_a_cliente_t> mensajes;
    pthread_mutex_lock(&mutex_render);
    if (background != nullptr) {
        mensajes.push_back(obtener_mensaje(background));
        num_entidades++;
    }

    for (auto &hongo: hongos) {
        if (hongo->get_dest_rect_x()<=ANCHO_VENTANA && hongo->get_dest_rect_x()>=ANCHO_ENTIDAD) {
            mensajes.push_back(obtener_mensaje(hongo));
            num_entidades++;
        }
    }

    for (auto &escenario:escenarios) {
        if (escenario->get_dest_rect_x()<=ANCHO_VENTANA && escenario->get_dest_rect_x()>=ANCHO_ENTIDAD) {
            mensajes.push_back(obtener_mensaje(escenario));
            num_entidades++;
        }
    }

    for (auto &moneda:monedas){
        if(moneda->get_dest_rect_x()<=ANCHO_VENTANA && moneda->get_dest_rect_x()>=ANCHO_ENTIDAD){
            mensajes.push_back(obtener_mensaje(moneda));
            num_entidades++;
        }
    }


    for (auto &enemigo: enemigos) {
        if (enemigo->get_dest_rect_x()<=ANCHO_VENTANA && enemigo->get_dest_rect_x()>=ANCHO_ENTIDAD) {
            mensajes.push_back(obtener_mensaje(enemigo));
            num_entidades++;
        }
    }

    for (int i = 0; i < jugadores.size(); i++) {
        if (i == num_cliente) {
            jugador_final = jugadores[i];
        } else {
            mensajes.push_back(obtener_mensaje_jugador(jugadores[i], jugadores[i]->cliente_recibio_sonido(num_cliente)));
            if(!jugadores.at(i)->cliente_recibio_sonido(num_cliente))
                jugadores[i]->agregar_cliente_que_recibio_sonido(num_cliente);
            strcpy(mensajes.back().entidad.usuario, usuarios[i].c_str());
        }
        num_entidades++;
    }
    if (jugador_final != nullptr) {
        mensajes.push_back(obtener_mensaje_jugador(jugador_final, jugador_final->cliente_recibio_sonido(num_cliente)));
        if(!jugador_final->cliente_recibio_sonido(num_cliente))
            jugador_final->agregar_cliente_que_recibio_sonido(num_cliente);
        strcpy(mensajes.back().entidad.usuario, usuarios[num_cliente].c_str());
    }
    pthread_mutex_unlock(&mutex_render);
    for (auto &mensaje : mensajes) {
        mensaje.cantidad_entidades = num_entidades;
        bucle_send(&mensaje, num_cliente);

    }
    usleep(delay_envio_render_ms*1500);
}

int Servidor::bucle_send(mensaje_servidor_a_cliente_t* mensaje_ptr, int num_cliente){
    int bytes_struct = sizeof(mensaje_servidor_a_cliente_t);
    int total_bytes_enviados = 0;
    int bytes_enviados = 0;
    bool enviando = true;

    while((bytes_struct>total_bytes_enviados) &&(enviando)) {
        bytes_enviados = send(conexiones.at(num_cliente), (mensaje_ptr+total_bytes_enviados), (sizeof(mensaje_servidor_a_cliente_t)-total_bytes_enviados),MSG_NOSIGNAL);
        if (bytes_enviados < 0) {
            LOG(Log::DEBUG) << "No se pudo enviar el mensaje al cliente: "<<socket<<". Error number: " << errno <<  std::endl;
            return EXIT_GAME;
        } else if (bytes_enviados == 0) {
            enviando = false;
        } else {
            total_bytes_enviados += bytes_enviados;
        }
    }
    return 0;
}

mensaje_servidor_a_cliente_t Servidor::obtener_mensaje(Renderer* render){
    mensaje_servidor_a_cliente_t mensaje;
    strcpy(mensaje.entidad.path_textura,render->get_path_img().c_str());
    strcpy(mensaje.entidad.default_path,render->get_def_path().c_str());
    mensaje.entidad.dest_rect = render->get_dest_rect();
    mensaje.entidad.src_rect = render->get_src_rect();
    mensaje.entidad.flip = render->get_flip();
    mensaje.entidad.es_jugador = false;
    mensaje.entidad.sonido_a_reproducir = NO_HAY_SONIDO;
    mensaje.entidad.vidas = 0;
    mensaje.entidad.puntaje = 0;
    mensaje.entidad.muerto = false;
    mensaje.num_nivel = nivel_actual;
    if(temporizador != nullptr)
        mensaje.tiempo_restante = temporizador->get_tiempo_restante();
    return mensaje;
}

mensaje_servidor_a_cliente_t Servidor::obtener_mensaje_jugador(Jugador* jugador, bool cliente_recibio_sonido){
    mensaje_servidor_a_cliente_t mensaje = obtener_mensaje(jugador);
    mensaje.entidad.es_jugador = true;
    if(!cliente_recibio_sonido) {
        mensaje.entidad.sonido_a_reproducir = jugador->get_sonido_a_reproducir();
    }else{
        mensaje.entidad.sonido_a_reproducir = NO_HAY_SONIDO;
    }
    mensaje.entidad.vidas = jugador->get_cantidad_vidas();
    mensaje.entidad.puntaje = jugador->get_puntaje();
    mensaje.entidad.muerto = jugador->esta_muerto();
    return mensaje;
}

void Servidor::grisar_jugador(int num_cliente){
    pthread_mutex_lock(&mutex_render);
    jugadores.at(num_cliente)->grisar();
    pthread_mutex_unlock(&mutex_render);
}

void Servidor::reconectar_jugador(int num_cliente){
    jugadores.at(num_cliente)->reconectar();
}

void Servidor::intercambiar_mensajes(Servidor* servidor){
    bool inicie_thread_enviar = false;
    int conexion = RECIBIENDO_MENSAJES;
    int cliente = servidor->get_pos_de_conexion();
    bool cliente_conectado = true;
    size_t tiempo_sin_reconexion = 0;
    size_t primer_tiempo_sin_reconexion = 0;

    pthread_t thread_enviar;
    std::vector<void*> parametros_thread;
    parametros_thread.push_back(servidor);
    parametros_thread.push_back(&cliente);

    //&& (tiempo_sin_reconexion < TIEMPO_MAX_SIN_CONEXION)
    while(conexion == RECIBIENDO_MENSAJES || ((conexion == NO_RECIBIENDO_MENSAJES) )) {
        if (servidor->juego_iniciado) {
            if (!inicie_thread_enviar) {
                pthread_create(&thread_enviar, nullptr, reinterpret_cast<void *(*)(void *)>(enviar_mensaje_thread),
                               &parametros_thread);
                inicie_thread_enviar = true;
            }
            //El otro thread esta enviando
            conexion = servidor->recibir_mensaje(cliente);
            if (conexion == RECIBIENDO_MENSAJES) {
                primer_tiempo_sin_reconexion = 0;
                tiempo_sin_reconexion = 0;
                if (!cliente_conectado) {
                    servidor->reconectar_jugador(cliente);
                    LOG(Log::INFO) << "Cliente reconectado: " << cliente <<std::endl;
                    cliente_conectado = true;
                }
            }else if((tiempo_sin_reconexion >= TIEMPO_MAX_SIN_CONEXION && conexion == NO_RECIBIENDO_MENSAJES) || conexion == EXIT_GAME){
                if(cliente_conectado) {
                    servidor->grisar_jugador(cliente);
                    LOG(Log::INFO) << "Cliente desconectado: " << cliente << std::endl;
                    cliente_conectado = false;
                }
            }else if (conexion == NO_RECIBIENDO_MENSAJES && primer_tiempo_sin_reconexion > 0) {
                tiempo_sin_reconexion = time(nullptr) - primer_tiempo_sin_reconexion;
            }else{ //conexion == NO_RECIBIENDO_MENSAJES && primer_tiempo_sin_reconexion == 0
                primer_tiempo_sin_reconexion = time(nullptr);
            }
        }
    }
    if (inicie_thread_enviar) {
        pthread_cancel(thread_enviar);
        pthread_join(thread_enviar, nullptr);
    }
    pthread_exit(nullptr);
}

void Servidor:: enviar_mensaje_thread(std::vector<void*>* parametros){
    Servidor* servidor = (Servidor*)((*parametros).at(0));
    int cliente = (*(int*)((*parametros).at(1)));
    while (servidor->juego_iniciado) {
        servidor->enviar_mensaje(cliente);
    }
    pthread_exit(nullptr);
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
    //SET TIMEOUT
    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    if (setsockopt (conexiones.at(num_cliente), SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout,sizeof(timeout)) < 0)
        LOG(Log::ERROR)<< "No se pudo establecer el timeout en: "<<timeout.tv_sec<<" para el socket: "<<conexiones.at(num_cliente)<<std::endl;
    while((bytes_struct>total_bytes_recibidos) && (recibiendo)) {
        bytes_recibidos = recv(conexiones.at(num_cliente), (buffer + total_bytes_recibidos), (bytes_struct - total_bytes_recibidos), MSG_NOSIGNAL);
        if (bytes_recibidos < 0 && errno != TIMEOUT && errno != INTERRUPT) {
            LOG(Log::ERROR) << "No se pudo recibir el mensaje. Error number: " << errno <<  std::endl;
            return EXIT_GAME;
        } else if (bytes_recibidos == 0 || (bytes_recibidos < 0 && (errno == TIMEOUT || errno == INTERRUPT))) {
            recibiendo = false;
        } else {
            total_bytes_recibidos += bytes_recibidos;
        }
    }

    //PROCESAMIENTO DEL MENSAJE
    if (total_bytes_recibidos == bytes_struct) {
        if ((((mensaje_cliente_a_servidor_t *) buffer)->evento).type==SDL_QUIT){
            usuarios_desconectados.push_back(usuarios.at(num_cliente));
            cant_clientes_exit++;
            return EXIT_GAME;
        }
        if(jugadores.size()>num_cliente) {
            pthread_mutex_lock(&mutex_desplazamiento);
            Jugador *jugador = jugadores.at(num_cliente);
            jugador->recibir_evento(((mensaje_cliente_a_servidor_t *) buffer)->evento);
            pthread_mutex_unlock(&mutex_desplazamiento);
        }
    }
    free(buffer);
    if(recibiendo)
        return RECIBIENDO_MENSAJES;
    return NO_RECIBIENDO_MENSAJES;
}

void Servidor::enviar_pantalla_entre_nivel(){
    if(!enemigos.empty()){
        for(auto &enemigo:enemigos){
            delete(enemigo);
        }
        enemigos.clear();
    }
    if(!monedas.empty()){
        for(auto &moneda:monedas){
            delete(moneda);
        }
        monedas.clear();
    }
    if(!escenarios.empty()){
        for(auto &escenario:escenarios){
            delete(escenario);
        }
        escenarios.clear();
    }
    if(!hongos.empty()){
        for(auto &hongo:hongos){
            delete(hongo);
        }
        hongos.clear();
    }
    if(background != nullptr)
        delete(background);
    background = nullptr;
}

bool Servidor::contiene_jugador(Jugador* jugador_buscado){
    for(int i = 0; i < jugadores_fin.size(); i++){
        if(jugador_buscado == jugadores_fin.at(i))
            return true;
    }
    return false;
}

void Servidor::update() {
    temporizador->update();
    colisionador->clear_entidades();
    std::vector<Escenario*> bloques;
    std::vector<Moneda*> monedas_vec;
    std::vector<Enemigo*> enemigos_vec;
    std::vector<Hongo*> hongos_vec;
    std::vector<Sorpresa*> sorpresas_vec;
    for(Escenario* escenario: escenarios){
        //if((escenario->get_dest_rect().x<ANCHO_VENTANA) && (escenario->get_dest_rect().x>(0-ANCHO_LADRILLO_PANTALLA))){
        bloques.push_back(escenario);
        //}
    }
    for(Enemigo* enemigo: enemigos){
        //if((enemigo->get_dest_rect_x()<ANCHO_VENTANA) && (enemigo->get_dest_rect_x()>(0-ANCHO_LADRILLO_PANTALLA))){
        enemigos_vec.push_back(enemigo);
        //}
    }
    for(Moneda* moneda: this->monedas){
        if(moneda->get_dest_rect_x()<ANCHO_VENTANA && moneda->get_dest_rect_x()>0-ANCHO_LADRILLO_PANTALLA){
            monedas_vec.push_back(moneda);
        }
    }
    for(Hongo* hongo: this->hongos){
        //if(hongo->get_dest_rect_x()<ANCHO_VENTANA && hongo->get_dest_rect_x()>-ANCHO_HONGO){
        hongos_vec.push_back(hongo);
        //}
        camara->acomodar_a_imagen(hongo);

    }
    for(Escenario* escenario: this->escenarios){
        if(escenario->es_sorpresa() && escenario->get_dest_rect_x()<ANCHO_VENTANA && escenario->get_dest_rect_x()>-ANCHO_SORPRESA_PANTALLA){
            sorpresas_vec.push_back((Sorpresa*)escenario);
        }
    }
    colisionador->agregar_bloques(bloques);
    colisionador->agregar_monedas(monedas_vec);
    colisionador->agregar_enemigos(enemigos_vec);
    colisionador->agregar_hongos(hongos_vec);
    colisionador->agregar_sorpresas(sorpresas_vec);
    for (auto & jugador : jugadores) {
        if(!jugador->esta_muerto()) {
            pthread_mutex_lock(&mutex_desplazamiento);
            jugador->cambiar_frame(camara);
            jugador->desplazar();
            pthread_mutex_unlock(&mutex_desplazamiento);
            colisionador->jugador_colisionar(jugador);
            if(jugador->finalizo_nivel() && !contiene_jugador(jugador))
                jugadores_fin.push_back(jugador);
        }else{
            pthread_mutex_lock(&mutex_desplazamiento);
            jugador->cambiar_frame(camara);
            jugador->desplazar();
            pthread_mutex_unlock(&mutex_desplazamiento);
        }

    }
    for (auto & enemigo : enemigos){
        enemigo->cambiar_frame(camara);
        enemigo->desplazar();
        colisionador->enemigo_colisionar(enemigo);
    }
    for (auto & escenario: escenarios){
        escenario->cambiar_frame(camara);
    }
    for (auto & moneda: monedas){
        moneda->cambiar_frame(camara);
        moneda->desplazar();
        if (moneda->velocidad_y >= 8) {
            consumir_moneda(moneda->get_dest_rect());
        }
    }
    for (auto & hongo: hongos){
        //colisionador->hongo_colisionar_con_pared(hongo);
        hongo->desplazar();
    }


    camara->scroll_background(background);
    camara->stop_scrolling();
    usleep(15000);
    bloques.clear();
    monedas_vec.clear();
    enemigos_vec.clear();
    hongos_vec.clear();
    sorpresas_vec.clear();
}

void Servidor::consumir_moneda(SDL_Rect pos_moneda){
    for(int i=0;i<monedas.size();i++){
        Moneda* moneda = monedas.at(i);
        if(moneda->get_dest_rect().x == pos_moneda.x && moneda->get_dest_rect().y == pos_moneda.y){
            delete(moneda);
            monedas.erase(monedas.begin()+i);
            break;
        }
    }
}

void Servidor::spawn_moneda(int x, int y) {
    Moneda* monedita = new Moneda(x, y, "./res/moneda_sorpresa.png");
    monedita->animar();
    monedas.push_back(monedita);
}


void Servidor::matar_enemigo(SDL_Rect pos_enemigo){
    for(int i=0;i<enemigos.size();i++) {
        Enemigo *enemigo = enemigos.at(i);
        if (enemigo->get_dest_rect().x == pos_enemigo.x && enemigo->get_dest_rect().y == pos_enemigo.y){
            enemigo->morir();
        break;
        }
    }
}

void Servidor::spawn_hongo(int x, int y) {
    hongos.push_back(new Hongo(x,y));
}

void Servidor::consumir_hongo(SDL_Rect pos_hongo){
    for(int i=0;i<hongos.size();i++){
        Hongo* hongo = hongos.at(i);
        if(hongo->get_dest_rect().x == pos_hongo.x && hongo->get_dest_rect().y == pos_hongo.y){
            delete(hongo);
            hongos.erase(hongos.begin()+i);
            break;
        }
    }
}

void Servidor::iniciar_juego(){
    std::cout << "Iniciando juego..."<< std::endl;
    camara = new Camara();
    /*if (lectorXml->generar_nivel(&enemigos, &monedas ,&escenarios, &background, &temporizador, std::string("nivel1")) == ERROR_XML){
        lectorXml->set_default();
        lectorXml->generar_nivel(&enemigos, &monedas, &escenarios, &background, &temporizador, std::string("nivel1"));
    }*/
    lectorXml->generar_jugador(&jugadores);
    nivel_actual = CAMBIANDO_NIVEL;

    juego_iniciado=true;
    game_loop();
}

void Servidor::finalizar_juego(){
    std::cout<<"Juego Finalizado. Procediendo al cierre del servidor."<<std::endl;
    LOG(Log::INFO)<<"Juego Finalizado. Cerrando el servidor."<<std::endl;
    juego_iniciado = false;
    LOG(Log::DEBUG) << "Finalizando juego" << std::endl;
    LOG(Log::DEBUG)<<"Eliminando Camara"<<std::endl;
    if (camara != nullptr)
        delete(camara);
    LOG(Log::DEBUG)<<"Eliminando Colisionador"<<std::endl;
    if(colisionador != nullptr) {
        colisionador->clear_entidades();
        delete (colisionador);
    }
    LOG(Log::DEBUG)<<"Eliminando Lector"<<std::endl;
    if (lectorXml != nullptr)
        delete(lectorXml);
    LOG(Log::DEBUG)<<"Eliminando Jugadores"<<std::endl;
    for (auto & jugador : jugadores)
        delete(jugador);
    LOG(Log::DEBUG)<<"Eliminando Background"<<std::endl;
    if (background != nullptr)
        delete(background);
    LOG(Log::DEBUG)<<"Eliminando Temporizador"<<std::endl;
    if(temporizador != nullptr)
        delete(temporizador);
    LOG(Log::DEBUG)<<"Eliminando Enemigos"<<std::endl;
    for (size_t i=0; i<enemigos.size(); i++){
        delete(enemigos.at(i));
    }
    if (!enemigos.empty())
        enemigos.clear();
    LOG(Log::DEBUG)<<"Eliminando Escenarios"<<std::endl;
    for (size_t i=0; i<escenarios.size(); i++){
        delete(escenarios.at(i));
    }
    if (!escenarios.empty())
        escenarios.clear();
    LOG (Log::DEBUG)<<"Eliminando Monedas."<<std::endl;
    for (size_t i=0; i<monedas.size(); i++){
        delete(monedas.at(i));
    }
    if (!monedas.empty())
        monedas.clear();
    if (!hongos.empty()) {
        for (auto& hongo:hongos)
            delete (hongo);
        hongos.clear();
    }
}

bool Servidor::game_over(){
    bool game_over = true;
    for (auto& jugador: jugadores){
        if(!jugador->esta_muerto()) {
            game_over = false;
            break;
        }
    }
    if(temporizador != nullptr) {
        if (!game_over && temporizador->get_tiempo_restante() <= 0) {
            game_over = true;
        }
    }
    return game_over;
}

void Servidor::game_loop() {
    if (estado_error == ERROR_JUEGO)
        quit = true;

    while (!quit && (!game_over() && !fin_juego)){
        pthread_mutex_lock(&mutex_render);
        camara->stop_scrolling();
        for (auto & jugador : jugadores)
            jugador->reset_posicion();
        int tiempo = time(nullptr);
        int aux;
        if (nivel_actual == CAMBIANDO_NIVEL)
            aux = 0;
        else
            aux = nivel_actual;
        if(aux != FIN_JUEGO)
            nivel_actual = CAMBIANDO_NIVEL;
        pthread_mutex_unlock(&mutex_render);
        pthread_mutex_lock(&mutex_render);
        enviar_pantalla_entre_nivel();
        pthread_mutex_unlock(&mutex_render);
        while(time(nullptr) - tiempo <= TIEMPO_ENTRE_NIVEL){
        }
        int estado_xml;
        if(!quit && !fin_juego) {
            pthread_mutex_lock(&mutex_render);
            nivel_actual = aux+1;

            std::string nivel_str = (std::string("nivel")+std::to_string(nivel_actual));
            estado_xml = lectorXml->generar_nivel(&enemigos, &monedas, &escenarios, &background, &temporizador, nivel_str);
            if ( estado_xml == QUIT) {
                fin_juego = true;
                nivel_actual = FIN_JUEGO;
            }
            if(estado_xml == ERROR_XML) {
                lectorXml->set_default();
                if(lectorXml->generar_nivel(&enemigos, &monedas, &escenarios, &background, &temporizador, nivel_str) == QUIT){
                    fin_juego = true;
                    nivel_actual = FIN_JUEGO;
                }
            }
            pthread_mutex_unlock(&mutex_render);
        }
        if(!fin_juego){
            camara->set_fin_nivel(background->es_fin_nivel());
            while (camara->check_movimiento(jugadores) != FIN_NIVEL && !quit && !game_over() && !fin_juego) {
                //LOS THREADS YA ESTAN RECIBIENDO Y ENVIANDO POR CADA CLIENTE
                update();
                if(cant_clientes_exit == jugadores.size())
                    quit = true;
                camara->set_fin_nivel(background->es_fin_nivel());
            }
            if(!lectorXml->tiene_siguiente_nivel("nivel"+std::to_string(nivel_actual+1))){
                fin_juego = true;
                nivel_actual = FIN_JUEGO;
            }
            for(auto & jugador: jugadores){
                if(jugador->finalizo_nivel() && !contiene_jugador(jugador))
                    jugadores_fin.push_back(jugador);
            }
            for(int i = 0 ; i< jugadores_fin.size(); i ++){
                jugadores_fin.at(i)->sumar_puntos_fin_nivel(2000 - 500 *i);
                jugadores_fin.at(i)->finalizar_nivel(false);
            }
            jugadores_fin.clear();
        }
    }
    pthread_mutex_lock(&mutex_render);
    enviar_pantalla_entre_nivel();
    if (fin_juego)
        nivel_actual = FIN_JUEGO;
    pthread_mutex_unlock(&mutex_render);
    while(!quit){
        if(cant_clientes_exit == jugadores.size())
            quit = true;
    }
    finalizar_juego();
}

Servidor::~Servidor() {
    LOG(Log::INFO)<<"Destruyendo servidor. Procediendo con el shutdown..."<<std::endl;
    shutdown(socket_svr, STOP_RECEPTION_AND_TRANSMISSION);
    close(socket_svr);
    LOG(Log::INFO)<<"Destruyendo thread de aceptación de conexiones..."<<std::endl;
    if (aceptando_conexiones)
        pthread_cancel(thread_conexiones);
    pthread_join(thread_conexiones, nullptr);
    LOG(Log::INFO)<<"Destruyendo threads de clientes..."<<std::endl;
    for(auto & thread:threads){
        pthread_cancel(*thread);
        pthread_join(*thread,nullptr);
        free(thread);
    }
}

in_addr_t Servidor::get_ip(){
    return svr_address.sin_addr.s_addr;
}

//PARA FINES DE TESTING. NO UTILIZAR.
void Servidor::set_juego_iniciado(){
    juego_iniciado = true;
}

int Servidor::get_socket(){
    return socket_svr;
}

int Servidor::get_cantidad_jugadores(){
    return lectorXml->get_cantidad_jugadores();
}

