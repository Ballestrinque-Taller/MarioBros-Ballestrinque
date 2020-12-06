#ifndef MARIOBROS_BALLESTRINQUE_CAMARA_H
#define MARIOBROS_BALLESTRINQUE_CAMARA_H

#include "Renderer.h"
#include "Background.h"
#include "Jugador.h"

#include <vector>

#define MARGEN_CAMARA 600*5/6

class Jugador;

class Camara {
    private:
        int velocidad_camara;
        bool debo_mover_camara;
        bool puedo_mover_camara;

    public:
        Camara();
        //void check_movimiento(Jugador* jugador, int aceleracion_jugador);
        void check_movimiento(std::vector<Jugador*>jugadores);
        void acomodar_a_imagen(Renderer* renderizado);
        void scroll_background(Background* background);
        void stop_scrolling();
};

#endif //MARIOBROS_BALLESTRINQUE_CAMARA_H
