#ifndef MARIOBROS_BALLESTRINQUE_CAMARA_H
#define MARIOBROS_BALLESTRINQUE_CAMARA_H

#include "Renderer.h"
#include "Background.h"

class Camara {
    private:
        int velocidad_camara;
        bool debo_mover_camara;

    public:
        Camara();
        void check_movimiento(Renderer* renderizado, int aceleracion_jugador);
        void acomodar_a_imagen(Renderer* renderizado);
        void scroll_background(Background* background);
        void stop_scrolling();
};

#endif //MARIOBROS_BALLESTRINQUE_CAMARA_H
