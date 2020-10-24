#include "LectorXML.h"
#include <time.h>

#define CAMPO_ENEMIGOS "enemigos"
#define WIDTH 800
#define WIDTH_SRC 304
#define RATIO_ASPECTO WIDTH_SRC/WIDTH
#define ANCHO_IMAGEN 3396

using namespace rapidxml;

// FALTA IMPLEMENTAR EN CASO DE ERRORES

LectorXML::LectorXML(SDL_Renderer* renderer){
    srand(time(NULL));
    rapidxml::file<> xmlFile("../res/config.xml");
    documento.parse<0>(xmlFile.data());
    this->renderer = renderer;
}

//genera todos los enemigos del nivel.
void LectorXML::generar_enemigos(xml_node<>* nivel, std::vector<Enemigo*> enemigos){
    xml_node<>* nodo_de_enemigos = nivel->first_node(CAMPO_ENEMIGOS);

    //dentro del campo enemigos en el xml, recorro cada enemigo (goomba, troopa, etc)
    for (xml_node<> *enemigo = nodo_de_enemigos->first_node();enemigo; enemigo = nodo_de_enemigos->next_sibling()){
        std::string tipo_enemigo = enemigo->first_attribute("tipo")->value();
        std::string path_a_imagen = enemigo->first_attribute("imagen")->value();
        int cantidad = std::stoi(enemigo->first_attribute("cantidad")->value());

        generar_enemigos_particulares(tipo_enemigo,path_a_imagen,cantidad,enemigos);
    }

}

//genera todos los enemigos de UN SOLO tipo en particular. Ejemplo: genera TODOS los troopas.
void LectorXML::generar_enemigos_particulares(std::string tipo_enemigo, std::string path_to_image, int cantidad, std::vector<Enemigo*> enemigos){
    if(tipo_enemigo.compare("troopa") == 0){
        for(int i=0;i<cantidad;i++){
            enemigos.push_back(new Tortuga(renderer,rand() % (ANCHO_IMAGEN * RATIO_ASPECTO) ,0,path_to_image));
        }
    }
    else if(tipo_enemigo.compare("goomba") == 0){
        for(int i=0;i<cantidad;i++){
            enemigos.push_back(new Goomba(renderer,rand() % (ANCHO_IMAGEN * RATIO_ASPECTO) ,0,path_to_image));
        }
    }
}

void LectorXML::generar_nivel(std::vector<Enemigo*> enemigos, std::vector<Escenario*> escenarios, std::string nivel){
    enemigos.clear();
    escenarios.clear();
    xml_node<>* nodo_del_nivel = documento.first_node(nivel.c_str());
    generar_enemigos(nodo_del_nivel,enemigos);
}