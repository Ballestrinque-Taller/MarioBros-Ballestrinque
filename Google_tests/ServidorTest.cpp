#include "gtest/gtest.h"
#include "Servidor.h"
#include "Mensajes.h"
#include <arpa/inet.h>

#define SERVIDOR_LOCAL "127.0.0.1"
#define STOP_RECEPTION_AND_TRANSMISSION 2

TEST(ServidorTest,test01GenerarConexionConElServidor){
    //creamos cliente
    int sock = socket(AF_INET,SOCK_STREAM,0);

    sockaddr_in addr;
    Servidor* servidor = new Servidor(SERVIDOR_LOCAL,5000);
    addr.sin_addr.s_addr = servidor->get_ip();
    addr.sin_port = 5000;
    addr.sin_family = AF_INET;
    int estadoConexion = connect(sock,(struct sockaddr *)&addr , sizeof(addr));
    int conexion_gen = servidor->aceptar_conexion();
    servidor->set_juego_iniciado();
    delete(servidor);
    shutdown(sock, STOP_RECEPTION_AND_TRANSMISSION);
    close(sock);
    EXPECT_EQ(conexion_gen,0);
}

TEST(ServidorTest,test02RecibirUnEventoEnElServidorYSeRecibeLaTotalidadDeBytes){
    mensaje_cliente_a_servidor_t mensaje;
    SDL_Event evento;
    mensaje.evento = evento;

    int sock = socket(AF_INET,SOCK_STREAM,0);

    sockaddr_in addr;
    Servidor* servidor = new Servidor(SERVIDOR_LOCAL,5000);
    addr.sin_addr.s_addr = servidor->get_ip();
    addr.sin_port = 5000;
    addr.sin_family = AF_INET;
    int estadoConexion = connect(sock,(struct sockaddr *)&addr , sizeof(addr));
    int conexion_gen = servidor->aceptar_conexion();
    send(sock, &mensaje, sizeof(mensaje), 0);
    int recibiendo_mensajes = servidor->recibir_mensaje(0);
    servidor->set_juego_iniciado();
    delete(servidor);
    shutdown(sock, STOP_RECEPTION_AND_TRANSMISSION);
    close(sock);
    EXPECT_EQ(recibiendo_mensajes, RECIBIENDO_MENSAJES);
}

TEST(ServidorTest,test03IntentoRecibirUnMensajeEnServidorQueNoHayYDevuelveNO_RECIBIENDO_MENSAJES){
    int sock = socket(AF_INET,SOCK_STREAM,0);
    sockaddr_in addr;
    Servidor* servidor = new Servidor(SERVIDOR_LOCAL,5000);
    addr.sin_addr.s_addr = servidor->get_ip();
    addr.sin_port = 5000;
    addr.sin_family = AF_INET;
    int estadoConexion = connect(sock,(struct sockaddr *)&addr , sizeof(addr));
    int conexion_gen = servidor->aceptar_conexion();
    int recibiendo_mensajes = servidor->recibir_mensaje(0);
    servidor->set_juego_iniciado();
    delete(servidor);
    shutdown(sock, STOP_RECEPTION_AND_TRANSMISSION);
    close(sock);
    EXPECT_EQ(recibiendo_mensajes, NO_RECIBIENDO_MENSAJES);
}

TEST(ServidorTest,test04IntentoRecibirUnMensajeEnServidorConUnSocketCerradoYDevuelveNO_RECIBIENDO_MENSAJES){
    int sock = socket(AF_INET,SOCK_STREAM,0);
    sockaddr_in addr;
    Servidor* servidor = new Servidor(SERVIDOR_LOCAL,5000);
    addr.sin_addr.s_addr = servidor->get_ip();
    addr.sin_port = 5000;
    addr.sin_family = AF_INET;
    int estadoConexion = connect(sock,(struct sockaddr *)&addr , sizeof(addr));
    int conexion_gen = servidor->aceptar_conexion();
    close(sock);
    int recibiendo_mensajes = servidor->recibir_mensaje(0);
    servidor->set_juego_iniciado();
    delete(servidor);
    shutdown(sock, STOP_RECEPTION_AND_TRANSMISSION);
    close(sock);
    EXPECT_EQ(recibiendo_mensajes, NO_RECIBIENDO_MENSAJES);
}

TEST(ServidorTest, test05RealizamosMultiplesConexionesPermitidasYEstasSeConectan) {
    sockaddr_in addr[4];
    Servidor *servidor = new Servidor(SERVIDOR_LOCAL, 5000);

    int sock[4];
    for (int i = 0; i < 4; i++) {
        addr[i].sin_addr.s_addr = servidor->get_ip();
        addr[i].sin_port = 5000;
        addr[i].sin_family = AF_INET;
        sock[i] = socket(AF_INET, SOCK_STREAM, 0);
        int estadoConexion = connect(sock[i], (struct sockaddr *) &(addr[i]), sizeof(addr));
        int conexion_gen = servidor->aceptar_conexion();
    }
    int cant_conex = servidor->get_cantidad_de_conexiones();
    servidor->set_juego_iniciado();
    delete (servidor);
    for (int i = 0; i < 4; i++){
        shutdown(sock[i], STOP_RECEPTION_AND_TRANSMISSION);
        close(sock[i]);
    }
    EXPECT_EQ(cant_conex,4);
}

TEST(ServidorTest, test06RealizamosMasDeLasConexionesPermitidasYLaNoPermitidaRetornaError){
    sockaddr_in addr[5];
    Servidor* servidor = new Servidor(SERVIDOR_LOCAL,5000);

    int sock[5];
    for (int i=0; i<4; i++){
        addr[i].sin_addr.s_addr = servidor->get_ip();
        addr[i].sin_port = 5000;
        addr[i].sin_family = AF_INET;
        sock[i] = socket(AF_INET,SOCK_STREAM,0);
        int estadoConexion = connect(sock[i],(struct sockaddr *)&(addr[i]) , sizeof(addr));
        int conexion_gen = servidor->aceptar_conexion();
        EXPECT_EQ(conexion_gen, 0);
    }
    addr[4].sin_addr.s_addr = servidor->get_ip();
    addr[4].sin_port = 5000;
    addr[4].sin_family = AF_INET;
    sock[4] = socket(AF_INET,SOCK_STREAM,0);
    int estadoConexion = connect(sock[4],(struct sockaddr *)&(addr[4]) , sizeof(addr));
    int conexion_gen = servidor->aceptar_conexion();
    servidor->set_juego_iniciado();
    delete(servidor);
    for (int i = 0; i < 5; i++){
        shutdown(sock[i], STOP_RECEPTION_AND_TRANSMISSION);
        close(sock[i]);
    }
    EXPECT_EQ(conexion_gen, ERROR_SVR);
}

TEST(ServidorTest, test07RealizandoMasConexionesQueLasPermitidasSoloQuedan4Conectadas){
    sockaddr_in addr[5];
    Servidor* servidor = new Servidor(SERVIDOR_LOCAL,5000);

    int sock[5];
    for (int i=0; i<4; i++){
        addr[i].sin_addr.s_addr = servidor->get_ip();
        addr[i].sin_port = 5000;
        addr[i].sin_family = AF_INET;
        sock[i] = socket(AF_INET,SOCK_STREAM,0);
        int estadoConexion = connect(sock[i],(struct sockaddr *)&(addr[i]) , sizeof(addr));
        int conexion_gen = servidor->aceptar_conexion();
        EXPECT_EQ(conexion_gen, 0);
    }
    addr[4].sin_addr.s_addr = servidor->get_ip();
    addr[4].sin_port = 5000;
    addr[4].sin_family = AF_INET;
    sock[4] = socket(AF_INET,SOCK_STREAM,0);
    int estadoConexion = connect(sock[4],(struct sockaddr *)&(addr[4]) , sizeof(addr));
    int conexion_gen = servidor->aceptar_conexion();
    int cantidad_conexiones = servidor->get_cantidad_de_conexiones();
    servidor->set_juego_iniciado();
    delete(servidor);
    for (int i = 0; i < 5; i++){
        shutdown(sock[i], STOP_RECEPTION_AND_TRANSMISSION);
        close(sock[i]);
    }
    EXPECT_EQ(cantidad_conexiones, MAX_CONEXIONES);
}

mensaje_servidor_a_cliente_t recibir_mensaje(int socket){
    mensaje_servidor_a_cliente_t mensaje_recibido;
    int total_bytes_recibidos = 0;
    int bytes_struct = sizeof(mensaje_servidor_a_cliente_t);
    char *buffer = (char *) malloc(bytes_struct);
    while ((bytes_struct > total_bytes_recibidos)) {
        total_bytes_recibidos += recv(socket, (buffer + total_bytes_recibidos), (bytes_struct - total_bytes_recibidos), 0);
    }
    //PROCESAMIENTO DEL MENSAJE
    if (total_bytes_recibidos == bytes_struct) {
        mensaje_recibido.entidad = ((mensaje_servidor_a_cliente_t *) buffer)->entidad;
        mensaje_recibido.cantidad_entidades = ((mensaje_servidor_a_cliente_t *)buffer)->cantidad_entidades;
        mensaje_recibido.num_nivel = ((mensaje_servidor_a_cliente_t *)buffer)->num_nivel;
    }
    free(buffer);
}

TEST(ServidorTest, test08AlEnviarUnMensajeSeEnviaCorrectamenteAlCliente){
    SDL_Rect rect;
    rect.h = 2; rect.w=3; rect.x = 4; rect.y = 5;
    mensaje_servidor_a_cliente_t mensaje;
    mensaje_servidor_a_cliente_t mensaje_recibido;
    mensaje.cantidad_entidades = 1;
    mensaje.entidad.src_rect = rect;
    mensaje.entidad.dest_rect = rect;
    mensaje.num_nivel = 1;
    mensaje.entidad.path_textura = "Path_textura";
    mensaje.entidad.default_path = "Default_path";
    mensaje.entidad.flip = SDL_FLIP_NONE;

    int sock = socket(AF_INET,SOCK_STREAM,0);

    sockaddr_in addr;
    Servidor* servidor = new Servidor(SERVIDOR_LOCAL,5001);
    addr.sin_addr.s_addr = servidor->get_ip();
    addr.sin_port = 5001;
    addr.sin_family = AF_INET;
    int estadoConexion = connect(sock,(struct sockaddr *)&addr , sizeof(addr));
    int conexion_gen = servidor->aceptar_conexion();

    int envio_bien = servidor->bucle_send(&mensaje,0);

    EXPECT_EQ(envio_bien, 0);

    mensaje_recibido = recibir_mensaje(sock);

    servidor->set_juego_iniciado();
    shutdown(sock, STOP_RECEPTION_AND_TRANSMISSION);
    close(sock);
    delete(servidor);

    EXPECT_EQ(mensaje_recibido.num_nivel, 1);
    EXPECT_EQ(mensaje_recibido.cantidad_entidades, 1);
    EXPECT_EQ(mensaje_recibido.entidad.src_rect.x, 4);
    EXPECT_EQ(mensaje_recibido.entidad.src_rect.y, 5);
    EXPECT_EQ(mensaje_recibido.entidad.src_rect.w, 3);
    EXPECT_EQ(mensaje_recibido.entidad.src_rect.h, 2);
    EXPECT_EQ(mensaje_recibido.entidad.dest_rect.x, 4);
    EXPECT_EQ(mensaje_recibido.entidad.dest_rect.y, 5);
    EXPECT_EQ(mensaje_recibido.entidad.dest_rect.w, 3);
    EXPECT_EQ(mensaje_recibido.entidad.dest_rect.h, 2);
    EXPECT_EQ(mensaje_recibido.entidad.flip, SDL_FLIP_NONE);
    EXPECT_EQ(mensaje_recibido.entidad.default_path, "Default_path");
    EXPECT_EQ(mensaje_recibido.entidad.path_textura, "Path_textura");
}

