#include "Ladrillo.h"
#include "Escenario.h"
#include "Camara.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define ANCHO_FRAME 16
#define ALTO_FRAME 16

#define POS_INICIAL_X_FRAME 0
#define POS_INICIAL_Y_FRAME 0

Ladrillo::Ladrillo(int pos_x, int pos_y, std::string path): Escenario(){
    set_dest_rect(pos_x, pos_y, ALTO_LADRILLO_PANTALLA, ANCHO_LADRILLO_PANTALLA);
    set_src_rect(POS_INICIAL_X_FRAME, POS_INICIAL_Y_FRAME, ALTO_FRAME, ANCHO_FRAME);
    path_to_image = path;
    default_path = ("./res/Ladrillo_default.png");
}

void Ladrillo::cambiar_frame(Camara* camara){
    camara->acomodar_a_imagen(this);
}