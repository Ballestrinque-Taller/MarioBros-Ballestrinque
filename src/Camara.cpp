#include "Camara.h"
#include "Renderer.h"
#include "Background.h"



Camara::Camara(){
    velocidad_camara = 0;
    puedo_mover_camara = true;
    debo_mover_camara = false;
}

int Camara::check_movimiento(std::vector<Jugador*>jugadores){
    bool todos_los_jugadores_en_fin = true;
    bool jugadores_en_fin[jugadores.size()];
    int i = 0;
    for (auto &jugador: jugadores){
        if (jugador->get_dest_rect_x() <= 0 && !jugador->esta_desconectado() && !jugador->esta_muerto()) {
            puedo_mover_camara = false;
        }
        if (jugador->get_dest_rect_x() >= MARGEN_CAMARA || jugador->esta_desconectado() || jugador->esta_muerto()){
            if(!jugador->esta_desconectado() && !jugador->esta_muerto() && fin_nivel){
                jugador->finalizar_nivel(true);
            }


            jugadores_en_fin[i] = true;
        }
        i++;
    }
    for (auto j_en_fin: jugadores_en_fin){
        if (!j_en_fin)
            todos_los_jugadores_en_fin = false;
    }
    if(fin_nivel)
        puedo_mover_camara = false;
    for (auto &jugador: jugadores){
        if(jugador->get_dest_rect_x() > MARGEN_CAMARA && puedo_mover_camara){
            if (jugador->get_velocidad_x() > velocidad_camara)
                velocidad_camara = jugador->get_velocidad_x();
            debo_mover_camara = true;
        }else if (jugador->get_dest_rect_x() > MARGEN_CAMARA && !puedo_mover_camara){
                jugador->set_dest_rect_x(MARGEN_CAMARA);
        }else if (jugador->get_dest_rect_x() <= 0){
            jugador->set_dest_rect_x(0);
        }else if(jugador->get_dest_rect_x() >= MARGEN_CAMARA && fin_nivel){
            jugador->set_dest_rect_x(MARGEN_CAMARA);

        }
    }
    if (fin_nivel && todos_los_jugadores_en_fin)
        return FIN_NIVEL;
    return NIVEL_EN_CURSO;
}

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

void Camara::set_fin_nivel(bool es_fin_nivel) {
    fin_nivel = es_fin_nivel;
}


