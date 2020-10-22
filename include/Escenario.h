#ifndef MARIOBROS_BALLESTRINQUE_ESCENARIO_H
#define MARIOBROS_BALLESTRINQUE_ESCENARIO_H

#include <SDL2/SDL.h>
#include "Renderer.h"
#include "Camara.h"

class Escenario: public Renderer{
    public:
        Escenario();
        virtual void cambiar_frame(SDL_Renderer* renderer, Camara* camara){};

    protected:
        int frame_actual;
        bool usado;
};

#endif //MARIOBROS_BALLESTRINQUE_ESCENARIO_H
