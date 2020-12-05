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
    Servidor* servidor = new Servidor(SERVIDOR_LOCAL,5000, "");
    addr.sin_addr.s_addr = servidor->get_ip();
    addr.sin_port = 5000;
    addr.sin_family = AF_INET;
    servidor->set_aceptando_conexiones_false();
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
    Servidor* servidor = new Servidor(SERVIDOR_LOCAL,5001, "");
    addr.sin_addr.s_addr = servidor->get_ip();
    addr.sin_port = 5001;
    addr.sin_family = AF_INET;
    int estadoConexion = connect(sock,(struct sockaddr *)&addr , sizeof(addr));
    //int conexion_gen = servidor->aceptar_conexion();
    sleep(1);
    servidor->set_aceptando_conexiones_false();
    send(sock, &mensaje, sizeof(mensaje), 0);
    sleep(1);
    int recibiendo_mensajes = servidor->recibir_mensaje(0);
    servidor->set_juego_iniciado();
    delete(servidor);
    shutdown(sock, STOP_RECEPTION_AND_TRANSMISSION);
    close(sock);
    EXPECT_EQ(recibiendo_mensajes, RECIBIENDO_MENSAJES);
}

/*
TEST(ServidorTest,test03IntentoRecibirUnMensajeEnServidorQueNoHayYDevuelveNO_RECIBIENDO_MENSAJES){
    int sock = socket(AF_INET,SOCK_STREAM,0);
    sockaddr_in addr;
    Servidor* servidor = new Servidor(SERVIDOR_LOCAL,5002, "");
    addr.sin_addr.s_addr = servidor->get_ip();
    addr.sin_port = 5000;
    addr.sin_family = AF_INET;
    int estadoConexion = connect(sock,(struct sockaddr *)&addr , sizeof(addr));
    //int conexion_gen = servidor->aceptar_conexion();
    sleep(1);
    servidor->set_aceptando_conexiones_false();
    int recibiendo_mensajes = servidor->recibir_mensaje(0);
    servidor->set_juego_iniciado();
    delete(servidor);
    shutdown(sock, STOP_RECEPTION_AND_TRANSMISSION);
    close(sock);
    EXPECT_EQ(recibiendo_mensajes, NO_RECIBIENDO_MENSAJES);
}*/

TEST(ServidorTest,test04IntentoRecibirUnMensajeEnServidorConUnSocketCerradoYDevuelveNO_RECIBIENDO_MENSAJES){
    int sock = socket(AF_INET,SOCK_STREAM,0);
    sockaddr_in addr;
    Servidor* servidor = new Servidor(SERVIDOR_LOCAL,5003, "");
    addr.sin_addr.s_addr = servidor->get_ip();
    addr.sin_port = 5003;
    addr.sin_family = AF_INET;
    int estadoConexion = connect(sock,(struct sockaddr *)&addr , sizeof(addr));
    //int conexion_gen = servidor->aceptar_conexion();
    sleep(1);
    close(sock);
    servidor->set_aceptando_conexiones_false();
    int recibiendo_mensajes = servidor->recibir_mensaje(0);
    servidor->set_juego_iniciado();
    delete(servidor);
    shutdown(sock, STOP_RECEPTION_AND_TRANSMISSION);
    close(sock);
    EXPECT_EQ(recibiendo_mensajes, NO_RECIBIENDO_MENSAJES);
}
/*
TEST(ServidorTest, test05RealizamosMultiplesConexionesPermitidasYEstasSeConectan) {
    sockaddr_in addr[lectorXml->get_cantidad_jugadores];
    Servidor *servidor = new Servidor(SERVIDOR_LOCAL, 5004, "");

    int sock[lectorXml->get_cantidad_jugadores()];
    for (int i = 0; i < lectorXml->get_cantidad_jugadores; i++) {
        addr[i].sin_addr.s_addr = servidor->get_ip();
        addr[i].sin_port = 5004;
        addr[i].sin_family = AF_INET;
        sock[i] = socket(AF_INET, SOCK_STREAM, 0);
        int estadoConexion = connect(sock[i], (struct sockaddr *) &(addr[i]), sizeof(addr));
        //int conexion_gen = servidor->aceptar_conexion();
    }
    sleep(2);
    servidor->set_aceptando_conexiones_false();
    int cant_conex = servidor->get_cantidad_de_conexiones();
    servidor->set_juego_iniciado();
    delete (servidor);
    for (int i = 0; i < lectorXmlget_cantidad_jugadores; i++){
        shutdown(sock[i], STOP_RECEPTION_AND_TRANSMISSION);
        close(sock[i]);
    }
    EXPECT_EQ(cant_conex, lectorXmlget_cantidad_jugadores);
}*/
/*
TEST(ServidorTest, test06RealizamosMasDeLasConexionesPermitidasYLaNoPermitidaRetornaJuegoIniciado){
    sockaddr_in addr[lectorXmlget_cantidad_jugadores + 1];
    Servidor* servidor = new Servidor(SERVIDOR_LOCAL,5005, "");

    int sock[lectorXmlget_cantidad_jugadores + 1];
    for (int i=0; i < lectorXmlget_cantidad_jugadores; i++){
        addr[i].sin_addr.s_addr = servidor->get_ip();
        addr[i].sin_port = 5005;
        addr[i].sin_family = AF_INET;
        sock[i] = socket(AF_INET,SOCK_STREAM,0);
        int estadoConexion = connect(sock[i],(struct sockaddr *)&(addr[i]) , sizeof(addr));
        sleep(1);
    }
    addr[lectorXmlget_cantidad_jugadores].sin_addr.s_addr = servidor->get_ip();
    addr[lectorXmlget_cantidad_jugadores].sin_port = 5005;
    addr[lectorXmlget_cantidad_jugadores].sin_family = AF_INET;
    sock[lectorXmlget_cantidad_jugadores] = socket(AF_INET, SOCK_STREAM, 0);
    servidor->set_aceptando_conexiones_false();
    int estadoConexion = connect(sock[lectorXmlget_cantidad_jugadores], (struct sockaddr *)&(addr[lectorXmlget_cantidad_jugadores]) , sizeof(addr));
    sleep(1);
    int conexion_gen = servidor->aceptar_conexion();
    servidor->set_juego_iniciado();
    delete(servidor);
    for (int i = 0; i < lectorXmlget_cantidad_jugadores + 1; i++){
        shutdown(sock[i], STOP_RECEPTION_AND_TRANSMISSION);
        close(sock[i]);
    }
    EXPECT_EQ(conexion_gen, JUEGO_INICIADO);
}*/
/*
TEST(ServidorTest, test07RealizandoMasConexionesQueLasPermitidasSoloQuedan4Conectadas){
    sockaddr_in addr[lectorXmlget_cantidad_jugadores + 1];
    Servidor* servidor = new Servidor(SERVIDOR_LOCAL,5006, "");

    int sock[lectorXmlget_cantidad_jugadores + 1];
    for (int i=0; i < lectorXmlget_cantidad_jugadores; i++){
        addr[i].sin_addr.s_addr = servidor->get_ip();
        addr[i].sin_port = 5006;
        addr[i].sin_family = AF_INET;
        sock[i] = socket(AF_INET,SOCK_STREAM,0);
        int estadoConexion = connect(sock[i],(struct sockaddr *)&(addr[i]) , sizeof(addr));
        //int conexion_gen = servidor->aceptar_conexion();
        //EXPECT_EQ(conexion_gen, 0);
        sleep(1);
    }
    addr[lectorXmlget_cantidad_jugadores + 1].sin_addr.s_addr = servidor->get_ip();
    addr[lectorXmlget_cantidad_jugadores + 1].sin_port = 5006;
    addr[lectorXmlget_cantidad_jugadores + 1].sin_family = AF_INET;
    sock[lectorXmlget_cantidad_jugadores + 1] = socket(AF_INET, SOCK_STREAM, 0);
    int estadoConexion = connect(sock[lectorXmlget_cantidad_jugadores], (struct sockaddr *)&(addr[lectorXmlget_cantidad_jugadores]) , sizeof(addr));
    sleep(1);
    int conexion_gen = servidor->aceptar_conexion();
    int cantidad_conexiones = servidor->get_cantidad_de_conexiones();
    servidor->set_juego_iniciado();
    delete(servidor);
    for (int i = 0; i < lectorXmlget_cantidad_jugadores + 1; i++){
        shutdown(sock[i], STOP_RECEPTION_AND_TRANSMISSION);
        close(sock[i]);
    }
    EXPECT_EQ(cantidad_conexiones, lectorXmlget_cantidad_jugadores);
}*/

TEST(ServidorTest, test08AlEnviarUnMensajeSeEnviaCorrectamenteAlCliente){
    SDL_Rect rect;
    rect.h = 2; rect.w=3; rect.x = 4; rect.y = 5;
    mensaje_servidor_a_cliente_t mensaje;
    mensaje_servidor_a_cliente_t mensaje_recibido;
    mensaje.cantidad_entidades = 1;
    mensaje.entidad.src_rect = rect;
    mensaje.entidad.dest_rect = rect;
    mensaje.num_nivel = 1;
    strcpy(mensaje.entidad.path_textura, "Path_textura");
    strcpy(mensaje.entidad.default_path, "Default_path");
    mensaje.entidad.flip = SDL_FLIP_NONE;
    mensaje.tiempo_restante = 10;

    int sock = socket(AF_INET,SOCK_STREAM,0);

    sockaddr_in addr;
    Servidor* servidor = new Servidor(SERVIDOR_LOCAL,5007, "");
    addr.sin_addr.s_addr = servidor->get_ip();
    addr.sin_port = 5007;
    addr.sin_family = AF_INET;
    int estadoConexion = connect(sock,(struct sockaddr *)&addr , sizeof(addr));
    //int conexion_gen = servidor->aceptar_conexion();
    sleep(1);
    servidor->set_aceptando_conexiones_false();

    int envio_bien = servidor->bucle_send(&mensaje,0);

    EXPECT_EQ(envio_bien, 0);

    //RECIBIMOS EL MENSAJE
    int total_bytes_recibidos = 0;
    int bytes_struct = sizeof(mensaje_servidor_a_cliente_t);
    char *buffer = (char *) malloc(bytes_struct);
    sleep(1);
    while ((bytes_struct > total_bytes_recibidos)) {
        total_bytes_recibidos += recv(sock, (buffer + total_bytes_recibidos), (bytes_struct - total_bytes_recibidos), MSG_NOSIGNAL);
    }
    EXPECT_EQ(total_bytes_recibidos, sizeof(mensaje_servidor_a_cliente_t));
    //PROCESAMIENTO DEL MENSAJE
    if (total_bytes_recibidos == bytes_struct) {
        mensaje_recibido.entidad = ((mensaje_servidor_a_cliente_t *) buffer)->entidad;
        mensaje_recibido.cantidad_entidades = ((mensaje_servidor_a_cliente_t *)buffer)->cantidad_entidades;
        mensaje_recibido.num_nivel = ((mensaje_servidor_a_cliente_t *)buffer)->num_nivel;
        mensaje_recibido.tiempo_restante =((mensaje_servidor_a_cliente_t*)buffer)->tiempo_restante;
    }
    free(buffer);

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
    EXPECT_EQ(strcmp(mensaje_recibido.entidad.default_path, "Default_path"), 0);
    EXPECT_EQ(strcmp(mensaje_recibido.entidad.path_textura, "Path_textura"), 0);
    EXPECT_EQ(mensaje_recibido.tiempo_restante, 10);
}

