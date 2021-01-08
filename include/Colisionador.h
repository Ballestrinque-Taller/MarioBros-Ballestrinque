#ifndef MARIOBROS_BALLESTRINQUE_COLISIONADOR_H
#define MARIOBROS_BALLESTRINQUE_COLISIONADOR_H

#include "Enemigo.h"
#include "Escenario.h"
#include "Moneda.h"
#include "Servidor.h"
#include "Renderer.h"

#define COLISION_SUPERIOR 0
#define COLISION_INFERIOR 1
#define COLISION_IZQUIERDA 2
#define COLISION_DERECHA 3
#define NO_COLISIONA -1

class Servidor;

class Colisionador{

    public:
        Colisionador(Servidor* servidor_recibido);
        void agregar_enemigos(std::vector<Enemigo*>enemigos_recibidos);
        void agregar_bloques(std::vector<Escenario*> escenarios);
        void agregar_monedas(std::vector<Moneda*> monedas_recibidas);
        void clear_entidades();
        void jugador_colisionar(Jugador* jugador);
        void enemigo_colisionar(Enemigo* enemigo);

    private:
        Servidor* servidor;
        std::vector<Enemigo*> enemigos;
        std::vector<Escenario*> bloques;
        std::vector<Moneda*> monedas;
        void jugador_colisiona_con_enemigo(Jugador* jugador);
        void jugador_colisiona_con_bloque(Jugador* jugador);
        void jugador_colisiona_con_moneda(Jugador* jugador);
        void enemigo_colisiona_con_enemigo(Enemigo* enemigo);
        void enemigo_colisiona_con_bloque(Enemigo* enemigo);

};

#endif //MARIOBROS_BALLESTRINQUE_COLISIONADOR_H
