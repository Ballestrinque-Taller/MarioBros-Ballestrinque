#ifndef MARIOBROS_BALLESTRINQUE_COLISIONADOR_H
#define MARIOBROS_BALLESTRINQUE_COLISIONADOR_H

#include "Enemigo.h"
#include "Escenario.h"
#include "Moneda.h"
#include "Renderer.h"

#define COLISION_SUPERIOR 0
#define COLISION_INFERIOR 1
#define COLISION_IZQUIERDA 2
#define COLISION_DERECHA 3
#define NO_COLISIONA -1

class Colisionador{

    public:
        //Colisionador();
        void agregar_enemigos(std::vector<Enemigo*>enemigos_recibidos);
        void agregar_bloques(std::vector<Escenario*> escenarios);
        void agregar_monedas(std::vector<Moneda*> monedas_recibidas);
        void clear_entidades();
        void jugador_colisionar(Jugador* jugador);
        void enemigo_colisionar(Enemigo* enemigo);

    private:
        std::vector<Enemigo*> enemigos;
        std::vector<Escenario*> bloques;
        std::vector<Moneda*> monedas;
        void jugador_colisiona_con_enemigo(Jugador* entidad);
        void jugador_colisiona_con_bloque(Jugador* entidad);
        void jugador_colisiona_con_moneda(Jugador* entidad);
};

#endif //MARIOBROS_BALLESTRINQUE_COLISIONADOR_H
