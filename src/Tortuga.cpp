#include "Tortuga.h"
#include "Enemigo.h"
#include "Camara.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define ALTO_ENEMIGO_PANTALLA 60
#define ANCHO_ENEMIGO_PANTALLA 40

#define POS_INICIAL_X_FRAME 0
#define POS_INICIAL_Y_FRAME 0
#define ALTO_FRAME 24
#define ANCHO_FRAME 16

#define MAXIMO_TICKS 5
#define MAX_FRAME 1
#define FRAME_CAPARAZON 2
#define FRAME_MUERTO 3

Tortuga::Tortuga(int pos_x, int pos_y,std::string path_to_image):Enemigo(){
    set_dest_rect(pos_x, pos_y, ALTO_ENEMIGO_PANTALLA, ANCHO_ENEMIGO_PANTALLA);
    set_src_rect(POS_INICIAL_X_FRAME, POS_INICIAL_Y_FRAME, ALTO_FRAME, ANCHO_FRAME);
    this->path_to_image = path_to_image;
    default_path = ("./res/Tortuga_default.png");
    encaparazonado = false;
    tipo_enemigo = TORTUGA;
}

void Tortuga::cambiar_frame(Camara* camara){
    tick_actual++;
    if(tick_actual>MAXIMO_TICKS && !muerto && !encaparazonado){
        tick_actual = 0;
        frame_actual++;
        if (frame_actual > MAX_FRAME)
            frame_actual = 0;
    }
    else if (encaparazonado && !muerto){
        tick_actual = 0;
        frame_actual = FRAME_CAPARAZON;
    }
    else if (muerto){
        frame_actual = FRAME_MUERTO;
    }
    set_src_rect(frame_actual*ANCHO_FRAME,0,ALTO_FRAME,ANCHO_FRAME);
    camara->acomodar_a_imagen(this);
}

