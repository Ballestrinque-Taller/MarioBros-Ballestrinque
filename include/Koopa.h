#ifndef MARIOBROS_BALLESTRINQUE_KOOPA_H
#define MARIOBROS_BALLESTRINQUE_KOOPA_H

#include "Enemigo.h"
#include "Camara.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


class Koopa: public Enemigo{
    public:
        Koopa(SDL_Renderer* renderer, int pos_x, int pos_y);
        void cambiar_frame(SDL_Renderer* renderer, Camara* camara) override;
};

#endif //MARIOBROS_BALLESTRINQUE_KOOPA_H
