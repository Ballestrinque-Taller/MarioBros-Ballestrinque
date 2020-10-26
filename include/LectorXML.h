#ifndef MARIOBROS_BALLESTRINQUE_XMLREADER_H
#define MARIOBROS_BALLESTRINQUE_XMLREADER_H

#include "XML/rapidxml.hpp"
//#include "XML/rapidxml_iterators.hpp"
#include "XML/rapidxml_print.hpp"
#include "XML/rapidxml_utils.hpp"
#include "Enemigo.h"
#include "Goomba.h"
#include "Tortuga.h"
#include "Escenario.h"
#include <vector>

using namespace rapidxml;

class LectorXML{
    public:
        LectorXML(SDL_Renderer* renderer);
        void generar_nivel(std::vector<Enemigo*>* enemigos, std::vector<Escenario*> escenarios, std::string nivel);
        void free_archivo();
   private:
        xml_document<> documento;
        char* archivo_data;
        SDL_Renderer* renderer;
        void generar_enemigos(xml_node<>* nivel, std::vector<Enemigo*>* enemigos);
        void generar_enemigos_particulares(std::string tipo_enemigo, std::string path_a_imagen, int cantidad, std::vector<Enemigo*>* enemigos);

};

#endif //MARIOBROS_BALLESTRINQUE_XMLREADER_H
