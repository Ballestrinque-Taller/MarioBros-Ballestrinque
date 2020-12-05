#include "gtest/gtest.h"
#include "Cliente.h"
#include "Servidor.h"
#include "Mensajes.h"
#include <arpa/inet.h>

#define SERVIDOR_LOCAL "127.0.0.1"
#define STOP_RECEPTION_AND_TRANSMISSION 2

TEST(ClienteTest, test01AlRecibirUnMensajeDelServidorEsteSeRecibeBien){
    SDL_Rect rect;
    rect.h = 2; rect.w=3; rect.x = 4; rect.y = 5;
    mensaje_servidor_a_cliente_t mensaje;
    mensaje.cantidad_entidades = 1;
    mensaje.entidad.src_rect = rect;
    mensaje.entidad.dest_rect = rect;
    mensaje.num_nivel = 1;
    strcpy(mensaje.entidad.path_textura, "Path_textura");
    strcpy(mensaje.entidad.default_path, "Default_path");
    mensaje.entidad.flip = SDL_FLIP_NONE;

    Servidor* servidor = new Servidor(SERVIDOR_LOCAL, 5000, "");
    Cliente* cliente = new Cliente(SERVIDOR_LOCAL, 5000);
    sleep(1);
    servidor->set_aceptando_conexiones_false();
    servidor->bucle_send(&mensaje, 0);
    cliente->recibir_renders_del_servidor();
    std::vector<entidad_t> entidades = cliente->get_entidades();
    entidad_t entidad = entidades.at(0);
    servidor->set_juego_iniciado();
    delete(servidor);
    delete(cliente);

    EXPECT_EQ(strcmp(entidad.path_textura, "Path_textura"), 0);
    EXPECT_EQ(strcmp(entidad.default_path, "Default_path"), 0);
    EXPECT_EQ(entidad.flip, SDL_FLIP_NONE);
    EXPECT_EQ(entidad.src_rect.h, 2); EXPECT_EQ(entidad.src_rect.w, 3);
    EXPECT_EQ(entidad.src_rect.x, 4); EXPECT_EQ(entidad.src_rect.y, 5);
    EXPECT_EQ(entidad.dest_rect.h, 2); EXPECT_EQ(entidad.dest_rect.w, 3);
    EXPECT_EQ(entidad.dest_rect.x, 4); EXPECT_EQ(entidad.dest_rect.y, 5);

}

