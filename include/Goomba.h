#ifndef MARIOBROS_BALLESTRINQUE_KOOPA_H
#define MARIOBROS_BALLESTRINQUE_KOOPA_H

#include "Enemigo.h"
#include "Camara.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


class Goomba: public Enemigo{
    public:
        Goomba(int pos_x, int pos_y,std::string path_to_image);
        void cambiar_frame(Camara* camara) override;
};

#endif //MARIOBROS_BALLESTRINQUE_KOOPA_H
