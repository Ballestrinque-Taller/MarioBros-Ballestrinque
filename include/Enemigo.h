#ifndef MARIOBROS_BALLESTRINQUE_ENEMIGO_H
#define MARIOBROS_BALLESTRINQUE_ENEMIGO_H

#include <SDL2/SDL.h>
#include "Renderer.h"

class Enemigo: public Renderer{
    public:
        Enemigo(SDL_Renderer* renderer, int pos_x, int pos_y);
        void cambiar_frame(SDL_Renderer* renderer);

    private:
        int aceleracion_x;
};

#endif /*MARIOBROS_BALLESTRINQUE_ENEMIGO_H*/