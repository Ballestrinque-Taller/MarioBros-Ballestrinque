#ifndef MARIOBROS_BALLESTRINQUE_JUGADOR_H
#define MARIOBROS_BALLESTRINQUE_JUGADOR_H

#include "Renderer.h"

typedef struct texturas{
    //0 Derecha && 1 Izquierda
    SDL_Texture* texturas[2];
    SDL_Texture* textura_agachado[2];
    SDL_Texture* textura_salto[2];
}texturas_t;

class Jugador: public Renderer{
    public:
        Jugador(SDL_Renderer* renderer);
        void acelerar_x(int direccion);
        void saltar();
        void desplazar();
        void renderizar(SDL_Renderer* renderer);
        void cambiar_frame(SDL_Renderer* renderer);

    private:
        SDL_Texture* textura_actual;
        texturas_t texturas;
        bool acelerando;
        int aceleracion_x;
        int aceleracion_y;
        bool en_aire;
        void aceleracion_gravitatoria();
};


#endif //MARIOBROS_BALLESTRINQUE_JUGADOR_H
