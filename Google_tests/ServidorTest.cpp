#include "gtest/gtest.h"
#include "Servidor.h"
#include "Mensajes.h"
#include <arpa/inet.h>

TEST(ServidorTest,test01){
    //creamos cliente
    int sock = socket(AF_INET,SOCK_STREAM,0);

    sockaddr_in addr;
    Servidor* servidor = new Servidor("169.254.255.0",5002);
    addr.sin_addr.s_addr = servidor->get_ip();
    addr.sin_port = 5002;
    addr.sin_family = AF_INET;
    int estadoConexion = connect(sock,(struct sockaddr *)&addr , sizeof(addr));
    EXPECT_EQ(servidor->generar_conexion(),0);
    delete(servidor);
}