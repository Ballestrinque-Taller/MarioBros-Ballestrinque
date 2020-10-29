#ifndef MARIOBROS_BALLESTRINQUE_TEMPORIZADOR_H
#define MARIOBROS_BALLESTRINQUE_TEMPORIZADOR_H


#include <time.h>
#include "Renderer.h"
#include "TextWriter.h"

class Temporizador: public TextWriter{

public:
    Temporizador(size_t tiempo_duracion);
    ~Temporizador();
    void reiniciar(size_t tiempo);
    void update();
    void render(SDL_Renderer* renderer);


private:
    size_t tiempo_parametro;
    size_t tiempo_restante;
    size_t tiempo_inicial;
};


#endif //MARIOBROS_BALLESTRINQUE_TEMPORIZADOR_H
