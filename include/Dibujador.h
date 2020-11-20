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
        Dibujador(SDL_Renderer* renderer_sdl);
        void dibujar(std::vector<entidad_t> entidades_a_dibujar);
        void crear_texturas(std::vector<entidad_t> entidades_a_texturizar);

    private:
        SDL_Renderer* renderer;
        std::vector<SDL_Texture*> texturas;
};

#endif //MARIOBROS_BALLESTRINQUE_DIBUJADOR_H
