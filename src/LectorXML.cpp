#include "LectorXML.h"
#include <time.h>

#define CAMPO_ENEMIGOS "enemigos"
#define CAMPO_BLOQUES "bloques"
#define CAMPO_MONEDAS "monedas"
#define CAMPO_BACKGROUND "background"
#define WIDTH 800
//#define RATIO_ASPECTO WIDTH_SRC/WIDTH
//#define ANCHO_IMAGEN 3396
//#define ANCHO_AJUSTADO ANCHO_IMAGEN*WIDTH/WIDTH_SRC

using namespace rapidxml;

// FALTA IMPLEMENTAR EN CASO DE ERRORES

LectorXML::LectorXML(SDL_Renderer* renderer){
    srand(time(NULL));
    rapidxml::file<> xmlFile("./res/config.xml");
    archivo_data = xmlFile.data();
    documento.parse<0>((char*)archivo_data.c_str());
    this->renderer = renderer;
}

//genera todos los enemigos del nivel.
void LectorXML::generar_enemigos(xml_node<>* nivel, std::vector<Enemigo*>* enemigos){
    xml_node<>* nodo_de_enemigos = nivel->first_node(CAMPO_ENEMIGOS);

    //dentro del campo enemigos en el xml, recorro cada enemigo (goomba, troopa, etc)
    xml_node<>* enemigo = nodo_de_enemigos->first_node();

   while(enemigo != nullptr){
        std::string tipo_enemigo = enemigo->first_attribute("tipo")->value();
        std::string path_a_imagen = (enemigo->first_attribute("imagen")->value());
        int cantidad = std::stoi(enemigo->first_attribute("cantidad")->value());

        generar_enemigos_particulares(tipo_enemigo,path_a_imagen,cantidad,enemigos);
        enemigo = enemigo->next_sibling();
   }

}

//genera todos los enemigos de UN SOLO tipo en particular. Ejemplo: genera TODOS los troopas.
void LectorXML::generar_enemigos_particulares(std::string tipo_enemigo, std::string path_to_image, int cantidad, std::vector<Enemigo*>* enemigos){
    if(tipo_enemigo.compare("troopa") == 0){
        for(int i=0;i<cantidad;i++){
            Enemigo* enemigo = new Tortuga(renderer,rand() % (ancho_ajustado) ,0,path_to_image);
            enemigos->push_back(enemigo);
        }
    }
    else if(tipo_enemigo.compare("goomba") == 0){
        for(int i=0;i<cantidad;i++){
            Enemigo* enemigo = new Goomba(renderer,rand() % (ancho_ajustado) ,0,path_to_image);
            enemigos->push_back(enemigo);
        }
    }
}

void LectorXML::generar_escenario(std::vector<Escenario*>* escenarios, xml_node<>* nivel){
    xml_node<>* nodo_de_bloques = nivel->first_node(CAMPO_BLOQUES);
    xml_node<>* bloque = nodo_de_bloques->first_node();

    while(bloque != nullptr){
        int pos_x = std::stoi(bloque->first_attribute("x")->value());
        int pos_y = std::stoi(bloque->first_attribute("y")->value());
        int cantidad = std::stoi(bloque->first_attribute("cantidad")->value());
        std::string tipo = bloque->first_attribute("tipo")->value();
        std::string path = bloque->first_attribute("imagen")->value();

        generar_bloques_particulares(tipo, cantidad, pos_x, pos_y, path, escenarios);
        bloque = bloque->next_sibling();
    }
}

void LectorXML::generar_bloques_particulares(std::string tipo, int cantidad, int x, int y, std::string path, std::vector<Escenario*>* escenarios){
    if (tipo.compare("Ladrillo") == 0){
        for (int i=0; i<cantidad; i++){
            escenarios->push_back(new Ladrillo(renderer,x+i*ANCHO_LADRILLO_PANTALLA,y, path));
        }
    }
    else if(tipo.compare("Sorpresa") == 0){
        for (int i=0; i<cantidad; i++){
            escenarios->push_back(new Sorpresa(renderer,x+i*ANCHO_SORPRESA_PANTALLA,y, path));
        }
    }
}

void LectorXML::generar_monedas(xml_node<>* nivel, std::vector<Escenario*>* escenarios){
    xml_node<>* nodo_de_monedas = nivel->first_node(CAMPO_MONEDAS);
    int cantidad = std::stoi(nodo_de_monedas->first_attribute("cantidad")->value());
    std::string path = nodo_de_monedas->first_attribute("imagen")->value();
    for (int i=0; i<cantidad; i++){
        escenarios->push_back(new Moneda(renderer,rand() % (ancho_ajustado),350, path));
    }
}

void LectorXML::generar_background(xml_node<>* nivel, Background** background){
    xml_node<>* nodo_de_background = nivel->first_node(CAMPO_BACKGROUND);
    std::string path = nodo_de_background->first_attribute("imagen")->value();
    int ancho = std::stoi (nodo_de_background->first_attribute("ancho")->value());
    int alto = std::stoi (nodo_de_background->first_attribute("alto")->value());
    ancho_ajustado = ancho*WIDTH/(alto*RATIO_ASPECTO);
    (*background) = new Background(renderer, path, ancho, alto);
}

//void LectorXML::generar_jugador(nivel, Jugador* jugador){}

void LectorXML::generar_nivel(std::vector<Enemigo*>* enemigos, std::vector<Escenario*>* escenarios, Background** background, std::string nivel){

    for (size_t i=0; i<enemigos->size(); i++){ //Recorro el vector y deleteo cada enemigo
        delete(enemigos->at(i));
    }
    for (size_t i=0; i<escenarios->size(); i++){
        delete(escenarios->at(i));
    };
    enemigos->clear();
    escenarios->clear();
    delete(*background);
    xml_node<>* nodo_del_nivel = documento.first_node()->first_node(nivel.c_str());
    generar_background(nodo_del_nivel, background);
    generar_escenario(escenarios, nodo_del_nivel);
    generar_enemigos(nodo_del_nivel,enemigos);
    generar_monedas(nodo_del_nivel, escenarios);
    //generar_timer(nodo_del_nivel, timer);
}