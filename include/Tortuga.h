#ifndef MARIOBROS_BALLESTRINQUE_TORTUGA_H
#define MARIOBROS_BALLESTRINQUE_TORTUGA_H

#include "Enemigo.h"
#include "Camara.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Tortuga: public Enemigo{
    public:
        Tortuga(SDL_Renderer* renderer, int pos_x, int pos_y);
        void cambiar_frame(SDL_Renderer *renderer, Camara* camara) override;

    private:
        bool encaparazonado;
};

#endif //MARIOBROS_BALLESTRINQUE_TORTUGA_H
