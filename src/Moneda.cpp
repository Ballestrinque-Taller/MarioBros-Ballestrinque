#include "Moneda.h"
#include "Escenario.h"
//#include "Servidor.h"
#include "Camara.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define MAX_FRAME 3
#define MAXIMO_TICKS 15
#define FRAME_USADA 4

#define ANCHO_FRAME 16
#define ALTO_FRAME 16

#define POS_INICIAL_X_FRAME 0
#define POS_INICIAL_Y_FRAME 0
#define ALTO_MONEDA_PANTALLA 40
#define ANCHO_MONEDA_PANTALLA 40

Moneda::Moneda(int pos_x, int pos_y, std::string path): Escenario(){
    set_dest_rect(pos_x, pos_y, ALTO_MONEDA_PANTALLA, ANCHO_MONEDA_PANTALLA);
    set_src_rect(POS_INICIAL_X_FRAME, POS_INICIAL_Y_FRAME, ALTO_FRAME, ANCHO_FRAME);
    path_to_image = path;
    default_path = ("./res/Moneda_default.png");
    frame_actual = 0;
}

void Moneda::animar() {
    se_mueve = true;
    velocidad_y = -15;
}

void Moneda::desplazar() {
    if (se_mueve) {
        aceleracion_gravitatoria();

        frames_render.dest_rect.x += velocidad_x;
        frames_render.dest_rect.y += velocidad_y;
    }
}

void Moneda::aceleracion_gravitatoria(){
    if(velocidad_y < 10 ){
        velocidad_y += 1;
    }
}

void Moneda::cambiar_frame(Camara* camara){
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
}