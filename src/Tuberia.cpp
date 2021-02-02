#include "../include/Tuberia.h"
#include "Escenario.h"
#include "Camara.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


#define POS_INICIAL_X_FRAME 0
#define POS_INICIAL_Y_FRAME 0

Tuberia::Tuberia(int pos_x, int pos_y, std::string path): Escenario(){
    set_dest_rect(pos_x, pos_y, ALTO_TUBERIA_PANTALLA, ANCHO_TUBERIA_PANTALLA);
    set_src_rect(POS_INICIAL_X_FRAME, POS_INICIAL_Y_FRAME, ALTO_TUBERIA_FRAME, ANCHO_TUBERIA_FRAME);
    path_to_image = path;
    default_path = ("./res/Ladrillo_default.png");
}

void Tuberia::cambiar_frame(Camara* camara){
    camara->acomodar_a_imagen(this);
}