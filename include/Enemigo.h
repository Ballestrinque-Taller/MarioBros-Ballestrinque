#ifndef MARIOBROS_BALLESTRINQUE_ENEMIGO_H
#define MARIOBROS_BALLESTRINQUE_ENEMIGO_H

#include <SDL2/SDL.h>
#include "Renderer.h"
#include "Camara.h"

class Enemigo: public Renderer{
    public:
        Enemigo();
        virtual void cambiar_frame(SDL_Renderer* renderer, Camara* camara){};

    protected:
        int aceleracion_x;
        int tick_actual;
        int frame_actual;
        bool muerto;
};

#endif /*MARIOBROS_BALLESTRINQUE_ENEMIGO_H*/