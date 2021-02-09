#ifndef MARIOBROS_BALLESTRINQUE_MONEDA_H
#define MARIOBROS_BALLESTRINQUE_MONEDA_H

#include "Escenario.h"
#include "Camara.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Moneda: public Escenario{
    public:
        Moneda(int pos_x, int pos_y, std::string path);
        void cambiar_frame(Camara* camara) override;
        void animar();
        void desplazar();
    private:
        void aceleracion_gravitatoria();

        int tick_actual = 0;
        bool se_mueve = false;
        int velocidad_x = 0;
        int velocidad_y = 0;
};

#endif //MARIOBROS_BALLESTRINQUE_MONEDA_H
