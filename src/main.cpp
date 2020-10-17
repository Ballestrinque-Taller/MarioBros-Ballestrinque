#include <string>

#include "Juego.h"

std::string calcular_path(char* rawPath);

int main(int args, char** argc){
    
    //esta constante contiene el path de la carpeta que contiene a bin/
    const std::string path_to_project = calcular_path(argc[0]);

    Juego* juego = new Juego();
    juego->game_loop();
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



