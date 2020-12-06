#include "Camara.h"
#include "Renderer.h"
#include "Background.h"



Camara::Camara(){
    velocidad_camara = 0;
    puedo_mover_camara = true;
    debo_mover_camara = false;
}

void Camara::check_movimiento(std::vector<Jugador*>jugadores){
    for (auto &jugador: jugadores){
        if (jugador->get_dest_rect_x() <= 0 && !jugador->esta_desconectado()) {
            puedo_mover_camara = false;
        }
    }
    for (auto &jugador: jugadores){
        if(jugador->get_dest_rect_x() > MARGEN_CAMARA && puedo_mover_camara){
            if (jugador->get_velocidad_x() > velocidad_camara)
                velocidad_camara = jugador->get_velocidad_x();
            debo_mover_camara = true;
        }else if (jugador->get_dest_rect_x() > MARGEN_CAMARA && !puedo_mover_camara){
            jugador->set_dest_rect_x(MARGEN_CAMARA);
        }else if (jugador->get_dest_rect_x() <= 0){
            jugador->set_dest_rect_x(0);
        }
    }
}

/*
void Camara::check_movimiento(Jugador* jugador, int aceleracion_jugador){
    if (jugador->get_dest_rect_x() > MARGEN_CAMARA && puedo_mover_camara){
        debo_mover_camara = true;
        if(aceleracion_jugador > 0)
            velocidad_camara = aceleracion_jugador;
        else
            velocidad_camara = 0;
    }
    else if(jugador->get_dest_rect_x() <= 0 && !jugador->esta_desconectado()){
        jugador->set_dest_rect_x(0);
        debo_mover_camara = false;
        puedo_mover_camara = false;
    }
    else if (jugador->get_dest_rect_x() > MARGEN_CAMARA && !puedo_mover_camara) {
        debo_mover_camara = false;
        jugador->set_dest_rect_x(MARGEN_CAMARA);
    }
}
*/
void Camara::acomodar_a_imagen(Renderer* renderizado){
    if(debo_mover_camara && puedo_mover_camara)
        renderizado->set_dest_rect_x(renderizado->get_dest_rect_x() - velocidad_camara);
}

void Camara::scroll_background(Background* background){
    if(debo_mover_camara && puedo_mover_camara)
        background->scroll(velocidad_camara);
}

void Camara::stop_scrolling(){
    velocidad_camara = 0;
    puedo_mover_camara = true;
    debo_mover_camara = false;
}


