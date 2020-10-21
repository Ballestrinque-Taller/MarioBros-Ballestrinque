#ifndef MARIOBROS_BALLESTRINQUE_LADRILLO_H
#define MARIOBROS_BALLESTRINQUE_LADRILLO_H

#include "Escenario.h"
#include "Camara.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Ladrillo: public Escenario{
    public:
        Ladrillo(SDL_Renderer* renderer, int pos_x, int pos_y);
        void cambiar_frame(SDL_Renderer* renderer, Camara* camara) override;
};

#endif //MARIOBROS_BALLESTRINQUE_LADRILLO_H
