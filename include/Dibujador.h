#ifndef MARIOBROS_BALLESTRINQUE_DIBUJADOR_H
#define MARIOBROS_BALLESTRINQUE_DIBUJADOR_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <vector>
#include "Mensajes.h"

class Dibujador{
    public:
        void dibujar(std::vector<entidad_t> entidades_a_dibujar, TextWriter* nivel_label, int nivel_actual, TextWriter* temporizador_label, int tiempo_restante, SDL_Renderer* renderer);
        void crear_texturas(std::vector<entidad_t> entidades_a_texturizar, SDL_Renderer* renderer);
        ~Dibujador();

    private:
        std::vector<SDL_Texture*> texturas;
        SDL_Texture* crear_textura(entidad_t entidad, SDL_Renderer* renderer);
};

#endif //MARIOBROS_BALLESTRINQUE_DIBUJADOR_H
