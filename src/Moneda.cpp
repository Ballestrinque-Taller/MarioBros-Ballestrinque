#include "Moneda.h"
#include "Escenario.h"
#include "Camara.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define MAX_FRAME 3
#define MAXIMO_TICKS 15
#define FRAME_USADA 4

#define ANCHO_FRAME 16
#define ALTO_FRAME 15

#define POS_INICIAL_X_FRAME 0
#define POS_INICIAL_Y_FRAME 0
#define ALTO_MONEDA_PANTALLA 40
#define ANCHO_MONEDA_PANTALLA 40

Moneda::Moneda(SDL_Renderer* renderer, int pos_x, int pos_y, std::string path): Escenario(){
    set_dest_rect(pos_x, pos_y, ALTO_MONEDA_PANTALLA, ANCHO_MONEDA_PANTALLA);
    set_src_rect(POS_INICIAL_X_FRAME, POS_INICIAL_Y_FRAME, ALTO_FRAME, ANCHO_FRAME);
    path_to_image = path;
    default_path = ("./res/MONEDA.png");
    renderizar(renderer);
}

void Moneda::cambiar_frame(SDL_Renderer* renderer, Camara* camara){
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