#ifndef MARIOBROS_BALLESTRINQUE_SERVIDOR_H
#define MARIOBROS_BALLESTRINQUE_SERVIDOR_H

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <vector>

struct Mensaje{
    //WEA A IMPLEMENTAR
}mensaje_t;

class Servidor{
    private:
        int socket_svr;
        sockaddr_in svr_address;
        std::vector<int> conexiones;
    public:
        Servidor();
        int generar_conexion(sockaddr* client_address);
        int recibir_mensaje(int sock_cliente, int* comando);
        void enviar_mensaje(void* mensaje, int size_mensaje, int sock_cliente);
};

#endif //MARIOBROS_BALLESTRINQUE_SERVIDOR_H
