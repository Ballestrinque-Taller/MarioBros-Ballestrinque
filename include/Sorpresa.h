#ifndef MARIOBROS_BALLESTRINQUE_SORPRESA_H
#define MARIOBROS_BALLESTRINQUE_SORPRESA_H

#define ALTO_SORPRESA_PANTALLA 40
#define ANCHO_SORPRESA_PANTALLA 40

#define SORPRESA_HONGO 1
#define SORPRESA_MONEDA 0

#include "Escenario.h"
#include "Camara.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Sorpresa: public Escenario{
    public:
        Sorpresa(int pos_x, int pos_y, std::string path);
        void cambiar_frame(Camara* camara) override;
        int get_tipo_premio();
        void consumir_sorpresa();
        bool sorpresa_consumida();
    private:
        int tick_actual = 0;
        int tipo_sorpresa;
        bool tiene_sorpresa;
};

#endif //MARIOBROS_BALLESTRINQUE_SORPRESA_H
