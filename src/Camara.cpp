#include "Camara.h"
#include "Renderer.h"
#include "Background.h"

#define MARGEN_CAMARA 600*5/6

Camara::Camara(){
    velocidad_camara = 0;
}

void Camara::check_movimiento(Renderer* renderizado, int aceleracion_jugador){
    if (renderizado->get_dest_rect_x() > MARGEN_CAMARA){
        debo_mover_camara = true;
        if(aceleracion_jugador > 0)
            velocidad_camara = aceleracion_jugador;
        else
            velocidad_camara = 0;
    }
    else if(renderizado->get_dest_rect_x() <= 0){
        renderizado->set_dest_rect_x(0);
        debo_mover_camara = false;
    }
    else
        debo_mover_camara = false;
}

void Camara::acomodar_a_imagen(Renderer* renderizado){
    if(debo_mover_camara)
        renderizado->set_dest_rect_x(renderizado->get_dest_rect_x() - velocidad_camara);
}

void Camara::scroll_background(Background* background, SDL_Renderer* renderer){
    background->scroll(velocidad_camara, renderer);
}

void Camara::stop_scrolling(){
    velocidad_camara = 0;
}


