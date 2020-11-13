#ifndef MARIOBROS_BALLESTRINQUE_SORPRESA_H
#define MARIOBROS_BALLESTRINQUE_SORPRESA_H

#define ALTO_SORPRESA_PANTALLA 40
#define ANCHO_SORPRESA_PANTALLA 40

#include "Escenario.h"
#include "Camara.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Sorpresa: public Escenario{
    public:
        Sorpresa(int pos_x, int pos_y, std::string path);
        void cambiar_frame(SDL_Renderer* renderer, Camara* camara) override;
    private:
        int tick_actual = 0;
};

#endif //MARIOBROS_BALLESTRINQUE_SORPRESA_H
