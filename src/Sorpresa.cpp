#include "Sorpresa.h"
#include "Escenario.h"
#include "Camara.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define ANCHO_FRAME 16
#define ALTO_FRAME 16
#define MAX_FRAME 2
#define MAXIMO_TICKS 15
#define FRAME_USADA 3

#define POS_INICIAL_X_FRAME 0
#define POS_INICIAL_Y_FRAME 0


Sorpresa::Sorpresa(int pos_x, int pos_y, std::string path): Escenario(){
    set_dest_rect(pos_x, pos_y, ALTO_SORPRESA_PANTALLA, ANCHO_SORPRESA_PANTALLA);
    set_src_rect(POS_INICIAL_X_FRAME, POS_INICIAL_Y_FRAME, ALTO_FRAME, ANCHO_FRAME);
    path_to_image = path;
    default_path = ("./res/Sorpresa_default.png");
}

void Sorpresa::cambiar_frame(SDL_Renderer* renderer, Camara* camara){
    if (!renderizado){
        renderizar(renderer);
    }
    tick_actual++;
    if(tick_actual>MAXIMO_TICKS){
        tick_actual = 0;
        frame_actual++;
        if (frame_actual > MAX_FRAME && !usado)
            frame_actual = 0;
    }
    else if (usado){
        frame_actual = FRAME_USADA;
    }
    set_src_rect(frame_actual*ANCHO_FRAME,0,ALTO_FRAME,ANCHO_FRAME);
    camara->acomodar_a_imagen(this);
    SDL_RenderCopyEx(renderer, texturas.textura, &(frames_render.src_rect), &(frames_render.dest_rect), 0, NULL, texturas.flip);
}