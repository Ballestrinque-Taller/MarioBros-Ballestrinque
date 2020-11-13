#ifndef MARIOBROS_BALLESTRINQUE_LADRILLO_H
#define MARIOBROS_BALLESTRINQUE_LADRILLO_H

#define ALTO_LADRILLO_PANTALLA 40
#define ANCHO_LADRILLO_PANTALLA 40

#include "Escenario.h"
#include "Camara.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Ladrillo: public Escenario{
    public:
        Ladrillo(int pos_x, int pos_y, std::string path);
        void cambiar_frame(SDL_Renderer* renderer, Camara* camara) override;
};

#endif //MARIOBROS_BALLESTRINQUE_LADRILLO_H
