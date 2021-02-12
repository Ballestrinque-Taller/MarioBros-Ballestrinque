#ifndef MARIOBROS_BALLESTRINQUE_HONGO_H
#define MARIOBROS_BALLESTRINQUE_HONGO_H

#include "Hongo.h"
#include "Renderer.h"

#define VELOCIDAD_HONGO 3
#define ANCHO_SOURCE 16
#define ALTO_SOURCE 16
#define ANCHO_HONGO 40
#define ALTO_HONGO 40

class Hongo: public Renderer{
    public:
        Hongo(int x, int y);
        void desplazar();
        void cambiar_direccion();
        void set_hongo_en_aire(bool esta_en_aire);

    private:
        float velocidad_x;
        float velocidad_y;
        bool en_aire = false;
        int animacion = 0;
        void aceleracion_gravitatoria();
        void animacion_emerger();



};

#endif //MARIOBROS_BALLESTRINQUE_HONGO_H
