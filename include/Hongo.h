#ifndef MARIOBROS_BALLESTRINQUE_HONGO_H
#define MARIOBROS_BALLESTRINQUE_HONGO_H

#include "Hongo.h"
#include "Renderer.h"

#define VELOCIDAD_HONGO 3
#define ANCHO_SOURCE 920
#define ALTO_SOURCE 920
#define ANCHO_HONGO 40
#define ALTO_HONGO 40

class Hongo: public Renderer{
    public:
        Hongo(int x, int y);
        void desplazar();
        void cambiar_direccion();
        void set_hongo_en_aire(bool esta_en_aire);

    private:
        int velocidad_x;
        int velocidad_y;
        bool en_aire = false;
        void aceleracion_gravitatoria();



};

#endif //MARIOBROS_BALLESTRINQUE_HONGO_H
