#ifndef MARIOBROS_BALLESTRINQUE_BACKGROUND_H
#define MARIOBROS_BALLESTRINQUE_BACKGROUND_H

#include "Renderer.h"
#define WIDTH 800
#define HEIGHT 600
#define RATIO_ASPECTO WIDTH/HEIGHT

class Background:public Renderer{
    public:
        Background(SDL_Renderer* renderer, std::string path, int ancho, int alto);
        void scroll(int velocidad_scroll, SDL_Renderer* renderer);

    private:
        float pos_x_actual;
        int alto_imagen;
        int ancho_imagen;
};

#endif //MARIOBROS_BALLESTRINQUE_BACKGROUND_H
