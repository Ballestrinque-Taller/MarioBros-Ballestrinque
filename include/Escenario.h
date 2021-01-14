#ifndef MARIOBROS_BALLESTRINQUE_ESCENARIO_H
#define MARIOBROS_BALLESTRINQUE_ESCENARIO_H

#include <SDL2/SDL.h>
#include "Renderer.h"
#include "Camara.h"

class Escenario: public Renderer{
    public:
        Escenario();
        virtual void cambiar_frame(Camara* camara){};
        bool es_sorpresa();

    protected:
        int frame_actual;
        bool bloque_sorpresa = false;
        bool usado = false;
};

#endif //MARIOBROS_BALLESTRINQUE_ESCENARIO_H
