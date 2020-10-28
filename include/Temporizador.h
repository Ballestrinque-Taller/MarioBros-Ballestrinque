#ifndef MARIOBROS_BALLESTRINQUE_TEMPORIZADOR_H
#define MARIOBROS_BALLESTRINQUE_TEMPORIZADOR_H


#include <time.h>
#include "Renderer.h"

class Temporizador: public Renderer{

public:

    Temporizador(int tiempo_duracion);
    ~Temporizador(){}

    void update();


private:
    time_t temp;
    time_t initial_time;
};


#endif //MARIOBROS_BALLESTRINQUE_TEMPORIZADOR_H
