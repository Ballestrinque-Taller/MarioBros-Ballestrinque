//
// Created by ramiro on 2/2/21.
//

#ifndef MARIO_TUBERIA_H
#define MARIO_TUBERIA_H

#define ALTO_TUBERIA_PANTALLA 40
#define ANCHO_TUBERIA_PANTALLA 80

#define ANCHO_TUBERIA_FRAME 80
#define ALTO_TUBERIA_FRAME 40

#include "Escenario.h"
#include "Camara.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


class Tuberia : public Escenario{
    public:
    Tuberia(int pos_x, int pos_y, std::string path);
    void cambiar_frame(Camara* camara) override;
};


#endif //MARIO_TUBERIA_H
