#include "Goomba.h"
#include "Enemigo.h"
#include "Camara.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define MAX_FRAME 1
#define MAXIMO_TICKS 5
#define FRAME_MUERTO 2

#define ANCHO_FRAME 16
#define ALTO_FRAME 16

#define POS_INICIAL_X_FRAME 0
#define POS_INICIAL_Y_FRAME 0
#define ALTO_ENEMIGO_PANTALLA 40
#define ANCHO_ENEMIGO_PANTALLA 40

Goomba::Goomba(int pos_x, int pos_y,std::string path_to_image):Enemigo(){
    set_dest_rect(pos_x, pos_y, ALTO_ENEMIGO_PANTALLA, ANCHO_ENEMIGO_PANTALLA);
    set_src_rect(POS_INICIAL_X_FRAME, POS_INICIAL_Y_FRAME, ALTO_FRAME, ANCHO_FRAME);
    this->path_to_image = path_to_image;
    default_path = ("./res/Goomba_default.png");
    tipo_enemigo = GOOMBA;
}

void Goomba::cambiar_frame(Camara* camara){
    tick_actual++;
    if(tick_actual>MAXIMO_TICKS){
        tick_actual = 0;
        frame_actual++;
        if (frame_actual > MAX_FRAME && !muerto)
            frame_actual = 0;
    }
    else if (muerto){
        frame_actual = FRAME_MUERTO;
    }
    set_src_rect(frame_actual*ANCHO_FRAME,0,ALTO_FRAME,ANCHO_FRAME);
    camara->acomodar_a_imagen(this);
}