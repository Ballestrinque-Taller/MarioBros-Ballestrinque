#include "Colisionador.h"

void Colisionador::agregar_bloques(std::vector<Escenario*> escenarios) {
    bloques = escenarios;
}

void Colisionador::agregar_enemigos(std::vector<Enemigo*> enemigos_recibidos) {
    enemigos = enemigos_recibidos;
}

void Colisionador::agregar_monedas(std::vector<Moneda*> monedas_recibidas) {
    monedas = monedas_recibidas;
}

void Colisionador::clear_entidades() {
    if(!monedas.empty())
        monedas.clear();
    if(!enemigos.empty())
        enemigos.clear();
    if(!bloques.empty())
        bloques.clear();
}

bool colision_izquierda(Renderer* entidad_almacenada, Renderer* entidad_a_colisionar){
    SDL_Rect entidad_almacenada_rect = entidad_almacenada->get_dest_rect();
    SDL_Rect entidad_col_rect = entidad_a_colisionar->get_dest_rect();
    if((entidad_almacenada_rect.x<=(entidad_col_rect.x+entidad_col_rect.w)) && ((entidad_almacenada_rect.x+entidad_almacenada_rect.w/2)>=(entidad_col_rect.x+entidad_col_rect.w)) &&
      (entidad_col_rect.y<=(entidad_almacenada_rect.y+entidad_almacenada_rect.h*5/6)) && (entidad_col_rect.y+entidad_col_rect.h>=(entidad_almacenada_rect.y+entidad_almacenada_rect.h*1/6))){
        return true;
    }
    return false;
}
bool colision_derecha(Renderer* entidad_almacenada, Renderer* entidad_a_colisionar){
    SDL_Rect entidad_almacenada_rect = entidad_almacenada->get_dest_rect();
    SDL_Rect entidad_col_rect = entidad_a_colisionar->get_dest_rect();
    if(((entidad_almacenada_rect.x+entidad_almacenada_rect.w/2)<=entidad_col_rect.x) && ((entidad_almacenada_rect.x+entidad_almacenada_rect.w)>=(entidad_col_rect.x)) &&
        (entidad_col_rect.y<=(entidad_almacenada_rect.y+entidad_almacenada_rect.h*5/6)) && (entidad_col_rect.y+entidad_col_rect.h>=(entidad_almacenada_rect.y+entidad_almacenada_rect.h*1/6))){
        return true;
    }
    return false;
}
bool colision_abajo(Renderer* entidad_almacenada, Renderer* entidad_a_colisionar){
    SDL_Rect entidad_almacenada_rect = entidad_almacenada->get_dest_rect();
    SDL_Rect entidad_col_rect = entidad_a_colisionar->get_dest_rect();
    if((((entidad_col_rect.x <= (entidad_almacenada_rect.x+entidad_almacenada_rect.w)) && (entidad_col_rect.x >= entidad_almacenada_rect.x)) ||
        (((entidad_col_rect.x+entidad_col_rect.w) <= (entidad_almacenada_rect.x+entidad_almacenada_rect.w)) && ((entidad_col_rect.x+entidad_col_rect.w) >= (entidad_almacenada_rect.x)))) &&
       ((entidad_col_rect.y)>=(entidad_almacenada_rect.y+entidad_almacenada_rect.h*4/6)) && ((entidad_col_rect.y)<=(entidad_almacenada_rect.y+entidad_almacenada_rect.h))){
        return true;
    }
    return false;
}
bool colision_arriba(Renderer* entidad_almacenada, Renderer* entidad_a_colisionar){
    SDL_Rect entidad_almacenada_rect = entidad_almacenada->get_dest_rect();
    SDL_Rect entidad_col_rect = entidad_a_colisionar->get_dest_rect();
    if((((entidad_col_rect.x <= (entidad_almacenada_rect.x+entidad_almacenada_rect.w)) && (entidad_col_rect.x >= entidad_almacenada_rect.x)) ||
        (((entidad_col_rect.x+entidad_col_rect.w) <= (entidad_almacenada_rect.x+entidad_almacenada_rect.w)) && ((entidad_col_rect.x+entidad_col_rect.w) >= (entidad_almacenada_rect.x)))) &&
        ((entidad_col_rect.y+entidad_col_rect.h)<(entidad_almacenada_rect.y+entidad_almacenada_rect.h*2/6)) && ((entidad_col_rect.y+ entidad_col_rect.h)>=(entidad_almacenada_rect.y))){
        return true;
    }
    return false;
}

void Colisionador::jugador_colisiona_con_bloque(Jugador* entidad) {
    bool hubo_colision_superior = false;
    for(Escenario* escenario: bloques){
        if(colision_arriba(escenario, entidad)) {
            entidad->colisionar_con_bloque(COLISION_SUPERIOR);
            entidad->set_dest_rect_y(escenario->get_dest_rect().y-entidad->get_dest_rect().h);
            hubo_colision_superior = true;
        }else if(colision_derecha(escenario, entidad)) {
            entidad->set_dest_rect_x(escenario->get_dest_rect().x+escenario->get_dest_rect().w+1);
            entidad->colisionar_con_bloque(COLISION_DERECHA);
        }else if(colision_izquierda(escenario, entidad)) {
            entidad->colisionar_con_bloque(COLISION_IZQUIERDA);
            entidad->set_dest_rect_x(escenario->get_dest_rect().x-entidad->get_dest_rect().w-1);
        }else if(colision_abajo(escenario, entidad)){
            entidad->set_dest_rect_y(escenario->get_dest_rect().y+escenario->get_dest_rect().h+1);
            entidad->colisionar_con_bloque(COLISION_INFERIOR);
        }
    }
    if(!hubo_colision_superior)
        entidad->colisionar_con_bloque(NO_COLISIONA);
}

void Colisionador::jugador_colisiona_con_enemigo(Jugador* entidad) {
    for(Enemigo* enemigo: enemigos){
        if(colision_izquierda(enemigo, entidad)){
            //return COLISION_IZQUIERDA;
        }else if(colision_derecha(enemigo, entidad)){
          //  return COLISION_DERECHA;
        }else if(colision_arriba(enemigo, entidad)){
        //    return COLISION_SUPERIOR;
        }else if(colision_abajo(enemigo, entidad)){
      //      return COLISION_INFERIOR;
        }
    }
    //return NO_COLISIONA;
}

void Colisionador::jugador_colisiona_con_moneda(Jugador* entidad) {
    for(Moneda* moneda: monedas){
        if(colision_izquierda(moneda, entidad)){
        //    return COLISION_IZQUIERDA;
        }else if(colision_derecha(moneda, entidad)){
        //    return COLISION_DERECHA;
        }else if(colision_arriba(moneda, entidad)){
        //    return COLISION_SUPERIOR;
        }else if(colision_abajo(moneda, entidad)){
        //    return COLISION_INFERIOR;
        }
    }
    //return NO_COLISIONA;
}

void Colisionador::jugador_colisionar(Jugador* jugador) {
    //int direccion_colision = colisiona_con_bloque(jugador);
    //jugador->colisionar_con_bloque(direccion_colision);
    jugador_colisiona_con_bloque(jugador);
}