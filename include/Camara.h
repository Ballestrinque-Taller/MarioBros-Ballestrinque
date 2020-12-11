#ifndef MARIOBROS_BALLESTRINQUE_CAMARA_H
#define MARIOBROS_BALLESTRINQUE_CAMARA_H

#include "Renderer.h"
#include "Background.h"
#include "Jugador.h"

#include <vector>

#define MARGEN_CAMARA 800*4/6
#define FIN_NIVEL 1
#define NIVEL_EN_CURSO 0

class Jugador;

class Camara {
    private:
        int velocidad_camara = 0;
        bool debo_mover_camara = false;
        bool puedo_mover_camara = true;
        bool fin_nivel = false;

    public:
        Camara();
        //void check_movimiento(Jugador* jugador, int aceleracion_jugador);
        void set_fin_nivel(bool es_fin_nivel);
        int check_movimiento(std::vector<Jugador*>jugadores);
        void acomodar_a_imagen(Renderer* renderizado);
        void scroll_background(Background* background);
        void stop_scrolling();
};

#endif //MARIOBROS_BALLESTRINQUE_CAMARA_H
