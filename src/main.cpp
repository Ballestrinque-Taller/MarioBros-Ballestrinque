#include <string>

#include "Cliente.h"
#include "Servidor.h"
#include "Log.h"


std::string calcular_path(char* rawPath);

int main(int argc, char* argv[]){
    //esta constante contiene el path de la carpeta que contiene a bin/
    //const std::string path_to_project = calcular_path(argv[0]);


    //Cliente/Servidor, IP, Puerto, PathXml <- ARGS
    if(strcmp(argv[1], "c") == 0 && argc==4){
        SET_LOG_FILE(std::string("logfile_cliente.txt"))
        Cliente* cliente = new Cliente(argv[2], atoi(argv[3]));
        //cliente->bucle_juego();
        delete(cliente);
    }
    else if (strcmp(argv[1], "s") == 0 && argc==5){
        SET_LOG_FILE(std::string("logfile_servidor.txt"))
        Servidor* servidor = new Servidor(argv[2], atoi(argv[3]), argv[4]);
        while(servidor->aceptando_conexiones){

        }
        servidor->iniciar_juego();
        delete(servidor);
    }
    else{
        SET_LOG_FILE(std::string("logfile.txt"))
        LOG(Log::ERROR)<<"Argumentos Invalidos para iniciar cliente/servidor. Deben introducirse c/s, IP, Puerto, PathXml(En servidor)."<<std::endl;
        exit(-1);
    }
	return 0;
}

    //la idea es que las clases que cargan imagenes, etc usen la
    //constante para que funcione independientemente de 
    //donde se ejecute la aplicacion.
    //solo que no se donde ponerla o como conviene
    //implementarlo (constante global es bastante feo). 
    //Despues lo discutimos
    //          -Tomy
std::string calcular_path(char* rawPath) {
    //horrible pero soluciona el problema de
    //el programa no encontrando las texturas dependiendo
    //de donde ejecutas el ejecutable
    std::string projectPath = std::string(rawPath);
    size_t ultima_barra = projectPath.find_last_of("\\");
    if (std::string::npos != ultima_barra){
    projectPath.erase(ultima_barra, std::string::npos);
    }
    //esto cambia \ por / de forma fea y lenta pero con pocas lineas
    ultima_barra = projectPath.find('\\');
    while (ultima_barra != std::string::npos) {
        projectPath.replace(ultima_barra,1,"/");
        ultima_barra = projectPath.find('\\');
    }
    projectPath = projectPath + "/../";
    return projectPath;
}

