#ifndef MARIOBROS_BALLESTRINQUE_BACKGROUND_H
#define MARIOBROS_BALLESTRINQUE_BACKGROUND_H

#include "Renderer.h"

class Background:public Renderer{
    public:
        Background(SDL_Renderer* renderer);
        void scroll(int velocidad_scroll, SDL_Renderer* renderer);

    private:
        float pos_x_actual;
};

#endif //MARIOBROS_BALLESTRINQUE_BACKGROUND_H
