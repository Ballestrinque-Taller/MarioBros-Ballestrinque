#ifndef MARIOBROS_BALLESTRINQUE_ENEMIGO_H
#define MARIOBROS_BALLESTRINQUE_ENEMIGO_H

#include <SDL2/SDL.h>
#include "Renderer.h"
#include "Camara.h"

#define VELOCIDAD_ENEMIGOS 2

class Enemigo: public Renderer{
    public:
        Enemigo();
        virtual ~Enemigo(){};
        virtual void cambiar_frame(Camara* camara){};
        void desplazar();
        void cambiar_direccion();

    protected:
        bool en_aire;
        int velocidad_x;
        int velocidad_y;
        int tick_actual;
        int frame_actual;
        bool muerto;
        void aceleracion_gravitatoria();
};

#endif /*MARIOBROS_BALLESTRINQUE_ENEMIGO_H*/