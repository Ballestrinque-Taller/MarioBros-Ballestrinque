#include "Servidor.h"
#include "Log.h"
#define ERROR_SVR -1
#define MAX_CONEXIONES 10

Servidor::Servidor(){
    SET_LOGGING_LEVEL(Log::DEBUG);
    socket_svr = socket(AF_INET,SOCK_STREAM,0);
    if (socket_svr == ERROR_SVR){
        LOG(Log::ERROR)<<"No se pudo crear el socket."<<std::endl;
        exit(ERROR_SVR);
    }
    svr_address.sin_family = AF_INET;
    svr_address.sin_addr.s_addr = INADDR_ANY;
    svr_address.sin_port = 0;
    if(bind(socket_svr, (sockaddr*)&svr_address, sizeof(struct sockaddr)) == ERROR_SVR){
        LOG(Log::ERROR)<<"No se pudo bindear el socket."<<std::endl;
        exit(ERROR_SVR);
    }
    if(listen(socket_svr, MAX_CONEXIONES) == ERROR_SVR){
        LOG(Log::ERROR)<<"No se pudo poner a escuchar al socket."<<std::endl;
        exit(ERROR_SVR);
    }
}

int Servidor::generar_conexion(sockaddr* client_address){
    conexiones.push_back(accept(socket_svr, client_address, (socklen_t*)(sizeof(struct sockaddr))) == ERROR_SVR);
    if(conexiones.back()){
        LOG(Log::ERROR)<<"No se pudo aceptar la conexión del client_adress"<<std::endl;
        conexiones.pop_back();
        return ERROR_SVR;
    }
    return 0;
}

void Servidor::enviar_mensaje(void* mensaje, int size_mensaje, int sock_cliente){
    if(send(sock_cliente, mensaje, size_mensaje, 0)==ERROR_SVR){
        LOG(Log::ERROR)<<"No se pudo enviar el mensaje al socket de cliente: "<<sock_cliente<<std::endl;
    }
}

int Servidor::recibir_mensaje(int sock_cliente, int* comando){
    int total_bytes_recibidos = 0;
    int bytes_recibidos = 0;
    int bytes_struct = sizeof(int);
    bool recibiendo = true;
    while((bytes_struct>bytes_recibidos) &&(recibiendo)) {
        bytes_recibidos = recv(sock_cliente, (comando + total_bytes_recibidos),
                                   (bytes_struct - total_bytes_recibidos), MSG_NOSIGNAL);
        if (bytes_recibidos < 0) {
            LOG(Log::ERROR) << "No se pudo recibir el mensaje." << std::endl;
            return bytes_recibidos;
        } else if (bytes_recibidos == 0) {
            recibiendo = false;
        } else {
            total_bytes_recibidos += bytes_recibidos;
        }
    }
}