#ifndef MARIOBROS_BALLESTRINQUE_SORPRESA_H
#define MARIOBROS_BALLESTRINQUE_SORPRESA_H

#include "Escenario.h"
#include "Camara.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Sorpresa: public Escenario{
    public:
        Sorpresa(SDL_Renderer* renderer, int pos_x, int pos_y);
        void cambiar_frame(SDL_Renderer* renderer, Camara* camara) override;
    private:
        int tick_actual = 0;
};

#endif //MARIOBROS_BALLESTRINQUE_SORPRESA_H
