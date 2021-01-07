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
        virtual void cambiar_frame(Camara* camara){};
        void set_dest_rect_x(int x);
        void set_dest_rect_y(int y);
        int get_dest_rect_x();
        SDL_Rect get_src_rect();
        SDL_Rect get_dest_rect();
        std::string get_path_img();
        std::string get_def_path();
        SDL_RendererFlip get_flip();

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
