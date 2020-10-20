#ifndef MARIOBROS_BALLESTRINQUE_CAMARA_H
#define MARIOBROS_BALLESTRINQUE_CAMARA_H

#include "Renderer.h"

class Camara {
    private:
        int velocidad_camara;
        bool debo_mover_camara = false;

    public:
        Camara();
        void check_movimiento(Renderer* renderizado, int aceleracion_jugador);
        void acomodar_a_imagen(Renderer* renderizado);
};

#endif //MARIOBROS_BALLESTRINQUE_CAMARA_H
