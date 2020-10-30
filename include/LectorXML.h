#ifndef MARIOBROS_BALLESTRINQUE_XMLREADER_H
#define MARIOBROS_BALLESTRINQUE_XMLREADER_H

#include "XML/rapidxml.hpp"
//#include "XML/rapidxml_iterators.hpp"
#include "XML/rapidxml_print.hpp"
#include "XML/rapidxml_utils.hpp"
#include "Enemigo.h"
#include "Goomba.h"
#include "Tortuga.h"
#include "Sorpresa.h"
#include "Ladrillo.h"
#include "Escenario.h"
#include "Moneda.h"
#include "Temporizador.h"
#include <vector>

using namespace rapidxml;

class LectorXML{

    public:
        LectorXML(SDL_Renderer* renderer);
        bool generar_nivel(std::vector<Enemigo*>* enemigos, std::vector<Escenario*>* escenarios, Background** background, Temporizador** temporizador, std::string nivel);
        void free_archivo();

   private:
        xml_document<> documento;
        int ancho_ajustado;
        std::string archivo_data;
        SDL_Renderer* renderer;
        void generar_timer(xml_node<>* nivel, Temporizador** timer);
        void generar_background(xml_node<>* nivel, Background** background);
        void generar_monedas(xml_node<>* nivel, std::vector<Escenario*>* escenarios);
        void generar_enemigos(xml_node<>* nivel, std::vector<Enemigo*>* enemigos);
        void generar_escenario(std::vector<Escenario*>* escenarios, xml_node<>* nivel);
        void generar_enemigos_particulares(std::string tipo_enemigo, std::string path_a_imagen, int cantidad, std::vector<Enemigo*>* enemigos);
        void generar_bloques_particulares(std::string tipo, int cantidad, int x, int y, std::string path,std::vector<Escenario*>* escenarios);

};

#endif //MARIOBROS_BALLESTRINQUE_XMLREADER_H
