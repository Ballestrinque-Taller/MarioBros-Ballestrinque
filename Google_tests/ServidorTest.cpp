#include "gtest/gtest.h"
#include "Servidor.h"
#include "Mensajes.h"
#include <arpa/inet.h>

TEST(ServidorTest,test01GenerarConexionConElServidor){
    //creamos cliente
    int sock = socket(AF_INET,SOCK_STREAM,0);

    sockaddr_in addr;
    Servidor* servidor = new Servidor("169.254.255.0",5002);
    addr.sin_addr.s_addr = servidor->get_ip();
    addr.sin_port = 5002;
    addr.sin_family = AF_INET;
    int estadoConexion = connect(sock,(struct sockaddr *)&addr , sizeof(addr));
    int conexion_gen = servidor->generar_conexion();
    delete(servidor);
    EXPECT_EQ(conexion_gen,0);
}

TEST(ServidorTest,test02RecibirUnEventoEnElServidor){
    mensaje_cliente_a_servidor_t mensaje;
    SDL_Event evento;
    mensaje.evento = evento;

    int sock = socket(AF_INET,SOCK_STREAM,0);

    sockaddr_in addr;
    Servidor* servidor = new Servidor("169.254.255.0",5002);
    addr.sin_addr.s_addr = servidor->get_ip();
    addr.sin_port = 5002;
    addr.sin_family = AF_INET;
    int estadoConexion = connect(sock,(struct sockaddr *)&addr , sizeof(addr));
    int conexion_gen = servidor->generar_conexion();
    send(sock, &mensaje, sizeof(mensaje), 0);
    int recibiendo_mensajes = servidor->recibir_mensaje(0);
    EXPECT_EQ(recibiendo_mensajes, 0);
}