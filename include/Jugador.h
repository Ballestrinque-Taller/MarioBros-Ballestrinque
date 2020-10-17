#ifndef MARIOBROS_BALLESTRINQUE_JUGADOR_H
#define MARIOBROS_BALLESTRINQUE_JUGADOR_H

#include "Renderer.h"

typedef struct texturas{
    SDL_Texture* textura;
    SDL_Texture* textura_agachado;
    SDL_Texture* textura_salto;
    SDL_RendererFlip flip;
}texturas_t;

class Jugador: public Renderer{
    public:
        Jugador(SDL_Renderer* renderer);
        void acelerar_x(int direccion);
        void saltar();
        void desplazar();
        void renderizar(SDL_Renderer* renderer);
        void cambiar_frame(SDL_Renderer* renderer);
        void recibir_evento(SDL_Event evento);
        void agacharse();

    private:
        SDL_Texture* textura_actual;
        texturas_t texturas;

        bool acelerando;
        bool en_aire;
        bool agachado;

        int aceleracion_x;
        int aceleracion_y;
        int frame_actual;
        int tick_actual;

        void aceleracion_gravitatoria();
        void rozamiento();
};


#endif //MARIOBROS_BALLESTRINQUE_JUGADOR_H
