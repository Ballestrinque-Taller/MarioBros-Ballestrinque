#ifndef MARIOBROS_BALLESTRINQUE_XMLREADER_H
#define MARIOBROS_BALLESTRINQUE_XMLREADER_H

#include "XML/rapidxml.hpp"
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
#include "Jugador.h"
#include "Mensajes.h"
#include "Moneda.h"
#include "Tuberia.h"
#include <vector>

#define ERROR_XML -1
#define PASO 0
#define QUIT 1

using namespace rapidxml;

class LectorXML{

    public:
        LectorXML(std::string path_to_xml);
        int generar_nivel(std::vector<Enemigo*>* enemigos, std::vector<Moneda*>*monedas, std::vector<Escenario*>* escenarios, Background** background, Temporizador** temporizador, std::string nivel);
        void free_archivo();
        bool generar_jugador(std::vector<Jugador*>* jugadores);
        int get_cantidad_jugadores();
        void set_default();
        bool posee_credenciales(credenciales_t credenciales);
        bool tiene_siguiente_nivel(std::string nivel);

   private:
        int cant_jugadores = 0;
        xml_document<> documento;
        int ancho_ajustado;
        std::string archivo_data;
        bool generar_timer(xml_node<>* nivel, Temporizador** timer);
        bool generar_background(xml_node<>* nivel, Background** background);
        bool generar_monedas(xml_node<>* nivel, std::vector<Moneda*>* monedas, std::vector<Escenario*> escenarios);
        bool moneda_colisiona_con_algun_bloque(int pos_x_moneda, int pos_y_moneda, std::vector<Escenario*> escenarios);
        bool generar_enemigos(xml_node<>* nivel, std::vector<Enemigo*>* enemigos);
        bool generar_escenario(std::vector<Escenario*>* escenarios, xml_node<>* nivel);
        void generar_enemigos_particulares(std::string tipo_enemigo, std::string path_a_imagen, int cantidad, std::vector<Enemigo*>* enemigos);
        void generar_bloques_particulares(std::string tipo, int cantidad, int x, int y, std::string path,std::vector<Escenario*>* escenarios);

};

#endif //MARIOBROS_BALLESTRINQUE_XMLREADER_H
