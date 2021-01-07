#ifndef MARIOBROS_BALLESTRINQUE_JUGADOR_H
#define MARIOBROS_BALLESTRINQUE_JUGADOR_H

#include "Renderer.h"
#include "Camara.h"

#define MAX_ACELERACION 8

#define COLISION_SUPERIOR 0
#define COLISION_INFERIOR 1
#define COLISION_IZQUIERDA 2
#define COLISION_DERECHA 3
#define NO_COLISIONA -1

#define PUNTOS_MONEDAS 50

#define MAX_VIDAS 3


class Jugador: public Renderer{
    public:
        int get_velocidad_x();
        void grisar();
        void reconectar();
        Jugador(std::string path);
        void acelerar_x(int direccion);
        void saltar();
        void desplazar();
        void cambiar_frame(Camara* camara) override;
        void recibir_evento(SDL_Event evento);
        void agacharse();
        void reset_posicion();
        bool esta_desconectado();
        void colisionar_con_bloque(int direccion);
        void colisionar_con_enemigo(int direccion);
        void colisionar_con_moneda();

    private:
        int puntos = 0;
        int vidas = MAX_VIDAS;
        bool desconectado;
        bool acelerando;
        bool en_aire;
        bool agachado;

        int velocidad_x;
        int velocidad_y;
        int max_acel;
        int frame_actual;
        int tick_actual;

        void aceleracion_gravitatoria();
        void rozamiento();
};


#endif //MARIOBROS_BALLESTRINQUE_JUGADOR_H
