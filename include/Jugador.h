#ifndef MARIOBROS_BALLESTRINQUE_JUGADOR_H
#define MARIOBROS_BALLESTRINQUE_JUGADOR_H

#include "../include/Renderer.h"


class Jugador: public Renderer{
    public:
        Jugador();
        /* Si nos remontamos al juego posta, vemos que mario no se mueve linealmente
         * Va acelerando con el tiempo.
         * para eso deberiamos usar x ejemplo los metodos comentados.
         *
         * */
        // void acelerar();void desplazar();

        void mover_pos_y(int y);//Son temporales, solo para chequear que funcione.
        void mover_pos_x(int x);
};


#endif //MARIOBROS_BALLESTRINQUE_JUGADOR_H
