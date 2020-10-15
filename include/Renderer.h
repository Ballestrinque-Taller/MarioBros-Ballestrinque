#ifndef MARIOBROS_BALLESTRINQUE_RENDERER_H
#define MARIOBROS_BALLESTRINQUE_RENDERER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

typedef struct frames{
    SDL_Texture* textura[20];
    int frame_actual;
    int tope_frames;
    SDL_Rect src_rect;
    SDL_Rect dest_rect;
}frames_t;

//HACER VIRTUAL
class Renderer {
    public:
        void renderizar(SDL_Renderer* renderer);
        void cambiar_frame();

    protected:
        std::string path_to_image;
        void set_dest_rect(int x,int y,int height,int width);
        void set_src_rect(int x,int y,int height,int width);
        frames_t frames_render;
    private:

};


#endif //MARIOBROS_BALLESTRINQUE_RENDERER_H
