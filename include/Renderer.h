#ifndef MARIOBROS_BALLESTRINQUE_RENDERER_H
#define MARIOBROS_BALLESTRINQUE_RENDERER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include "Texturas.h"

class Camara;

typedef struct frames{
    SDL_Rect src_rect;
    SDL_Rect dest_rect;
}frames_t;

class Renderer {
    public:
        virtual ~Renderer();
        void renderizar(SDL_Renderer* renderer);
        virtual void cambiar_frame(SDL_Renderer* renderer, Camara* camara){};
        void set_dest_rect_x(int x);
        int get_dest_rect_x();

    protected:
        bool renderizado = false;
        std::string default_path;
        std::string path_to_image;
        void set_dest_rect(int x,int y,int height,int width);
        void set_src_rect(int x,int y,int height,int width);
        texturas_t texturas;
        frames_t frames_render;
};


#endif //MARIOBROS_BALLESTRINQUE_RENDERER_H
