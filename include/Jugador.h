#ifndef MARIOBROS_BALLESTRINQUE_JUGADOR_H
#define MARIOBROS_BALLESTRINQUE_JUGADOR_H

#include "Renderer.h"
#include "Camara.h"


class Jugador: public Renderer{
    public:
        Jugador(SDL_Renderer* renderer);
        void acelerar_x(int direccion);
        void saltar();
        void desplazar();
        void cambiar_frame(SDL_Renderer* renderer, Camara* camara) override;
        void recibir_evento(SDL_Event evento);
        void agacharse();

    private:
        SDL_Texture* textura_actual;

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
