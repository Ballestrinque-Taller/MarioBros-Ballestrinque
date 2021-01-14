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

#define MAX_SEGS_INMUNE 3


class Jugador: public Renderer{
    public:
        int get_velocidad_x();
        int get_velocidad_y();
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
        bool esta_inmune();
        bool esta_crecido();
        void colisionar_con_bloque(int direccion);
        void colisionar_con_enemigo(int direccion);
        void colisionar_con_moneda();
        void crecer();
        void sumar_puntos(int tipo_enemigo);

    private:
        int puntos = 0;
        int vidas = MAX_VIDAS;
        bool desconectado;
        bool acelerando;
        bool en_aire;
        bool agachado;
        int estado_crecimiento;
        bool inmune = false;
        int tiempo_inmune = 9900;
        int TickDanio = 0;

        int velocidad_x;
        int velocidad_y;
        int max_acel;
        int frame_actual;
        int tick_actual;

        void aceleracion_gravitatoria();
        void rozamiento();
};


#endif //MARIOBROS_BALLESTRINQUE_JUGADOR_H
