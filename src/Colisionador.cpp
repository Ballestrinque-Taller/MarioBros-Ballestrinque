#include "Colisionador.h"
#include "ReproductorDeSonido.h"

#include <utility>

Colisionador::Colisionador(Servidor* servidor_recibido){
    servidor = servidor_recibido;
}

void Colisionador::agregar_bloques(std::vector<Escenario*> escenarios) {
    bloques = std::move(escenarios);
}

void Colisionador::agregar_enemigos(std::vector<Enemigo*> enemigos_recibidos) {
    enemigos = std::move(enemigos_recibidos);
}

void Colisionador::agregar_monedas(std::vector<Moneda*> monedas_recibidas) {
    monedas = std::move(monedas_recibidas);
}

void Colisionador::agregar_sorpresas(std::vector<Sorpresa*> sorpresas_recibidas){
    sorpresas = std::move(sorpresas_recibidas);
}

void Colisionador::agregar_hongos(std::vector<Hongo*> hongos_recibidos){
    hongos = std::move(hongos_recibidos);
}

void Colisionador::clear_entidades() {
    if(!monedas.empty())
        monedas.clear();
    if(!enemigos.empty())
        enemigos.clear();
    if(!bloques.empty())
        bloques.clear();
    if(!sorpresas.empty())
        sorpresas.clear();
    if(!hongos.empty())
        hongos.clear();
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

void Colisionador::jugador_colisiona_con_bloque(Jugador* jugador) {
    bool hubo_colision_superior = false;
    for(Escenario* escenario: bloques){
        if(colision_arriba(escenario, jugador)) {
            jugador->colisionar_con_bloque(COLISION_SUPERIOR);
            jugador->set_dest_rect_y(escenario->get_dest_rect().y - jugador->get_dest_rect().h);
            hubo_colision_superior = true;
        }else if(colision_derecha(escenario, jugador)) {
            jugador->set_dest_rect_x(escenario->get_dest_rect().x + escenario->get_dest_rect().w + 1);
            jugador->colisionar_con_bloque(COLISION_DERECHA);
        }else if(colision_izquierda(escenario, jugador)) {
            jugador->colisionar_con_bloque(COLISION_IZQUIERDA);
            jugador->set_dest_rect_x(escenario->get_dest_rect().x - jugador->get_dest_rect().w - 1);
        }else if(colision_abajo(escenario, jugador)){
            jugador->set_dest_rect_y(escenario->get_dest_rect().y + escenario->get_dest_rect().h + 1);
            jugador->colisionar_con_bloque(COLISION_INFERIOR);
        }
    }
    if(!hubo_colision_superior)
        jugador->colisionar_con_bloque(NO_COLISIONA);
}

void Colisionador::jugador_colisiona_con_enemigo(Jugador* jugador) {
    bool hubo_colision = false;
    for(Enemigo* enemigo: enemigos){
        if((colision_izquierda(enemigo, jugador) || colision_derecha(enemigo, jugador) || colision_abajo(enemigo, jugador)) && !enemigo->esta_muerto() && !jugador->esta_inmune()){
            jugador->colisionar_con_enemigo(COLISION_IZQUIERDA); //Es lo mismo con izquierda, derecha, y abajo.
            hubo_colision = true;
        }else if(colision_arriba(enemigo, jugador) && !enemigo->esta_muerto() && jugador->get_velocidad_y() > 0){
            jugador->colisionar_con_enemigo(COLISION_SUPERIOR);
            jugador->sumar_puntos(enemigo->tipo_enemigo);
            servidor->matar_enemigo(enemigo->get_dest_rect());
            jugador->set_sonido_a_reproducir(SONIDO_PISAR_ENEMIGOS);
            hubo_colision = true;
        }
    }
    if(!hubo_colision){
        jugador->colisionar_con_enemigo(NO_COLISIONA);
    }
}

void Colisionador::jugador_colisiona_con_moneda(Jugador* jugador) {
    for(int i=0; i<monedas.size();i++){
        Moneda* moneda = monedas.at(i);
        if(!moneda->se_mueve && (colision_izquierda(moneda, jugador) || colision_derecha(moneda, jugador) ||
           colision_arriba(moneda, jugador) || colision_abajo(moneda, jugador))) {
            jugador->colisionar_con_moneda();
            servidor->consumir_moneda(moneda->get_dest_rect());
            jugador->set_sonido_a_reproducir(SONIDO_MONEDA);
        }
    }
}

void Colisionador::jugador_colisiona_con_sorpresa(Jugador* jugador){
    for(auto& sorpresa : sorpresas){
        if(colision_abajo(sorpresa, jugador) && !sorpresa->sorpresa_consumida()){
            if(sorpresa->get_tipo_premio() == SORPRESA_HONGO) {
                servidor->spawn_hongo(sorpresa->get_dest_rect_x(),
                                      sorpresa->get_dest_rect().y);
                jugador->set_sonido_a_reproducir(SONIDO_HONGO);
            }else{
                servidor->spawn_moneda(sorpresa->get_dest_rect().x, 
                                        sorpresa->get_dest_rect().y-16);
                jugador->colisionar_con_moneda();
                jugador->set_sonido_a_reproducir(SONIDO_MONEDA);
            }
            sorpresa->consumir_sorpresa();
        }
    }
}

void Colisionador::jugador_colisiona_con_hongo(Jugador* jugador){
    for(auto& hongo: hongos){
        if((colision_abajo(hongo, jugador) || colision_arriba(hongo,jugador) || colision_izquierda(hongo,jugador) ||
        colision_derecha(hongo,jugador)) && !jugador->esta_crecido()){
            jugador->crecer();
            servidor->consumir_hongo(hongo->get_dest_rect());
            jugador->set_sonido_a_reproducir(SONIDO_CRECIMIENTO);
        }
    }
}

void Colisionador::jugador_colisionar(Jugador* jugador) {
    jugador_colisiona_con_sorpresa(jugador);
    jugador_colisiona_con_bloque(jugador);
    jugador_colisiona_con_moneda(jugador);
    jugador_colisiona_con_enemigo(jugador);
    jugador_colisiona_con_hongo(jugador);
}

void Colisionador::enemigo_colisionar(Enemigo* enemigo){
    enemigo_colisiona_con_enemigo(enemigo);
    enemigo_colisiona_con_bloque(enemigo);
}

void Colisionador::hongo_colisionar_con_pared(Hongo *hongo) {
    bool colision_superior = false;
    for(Escenario* escenario: bloques){
        if(colision_izquierda(escenario, hongo) || colision_derecha(escenario, hongo)) {
            hongo->cambiar_direccion();
        }
        else if (colision_arriba(escenario, hongo)){
            hongo->set_hongo_en_aire(false);
            hongo->set_dest_rect_y(escenario->get_dest_rect().y - hongo->get_dest_rect().h);
            colision_superior = true;
        }
    }
    if(!colision_superior)
        hongo->set_hongo_en_aire(true);
}

void Colisionador::enemigo_colisiona_con_bloque(Enemigo* enemigo) {
    bool colision_superior = false;
    int velocidad_enemigo_inicial = enemigo->get_velocidad_x();
    for(Escenario* escenario: bloques){
        if(colision_izquierda(escenario, enemigo) || colision_derecha(escenario, enemigo)) {
            if(colision_izquierda(escenario, enemigo)){
                enemigo->set_dest_rect_x(escenario->get_dest_rect().x - enemigo->get_dest_rect().w);
            }else
                enemigo->set_dest_rect_x(escenario->get_dest_rect().x + escenario->get_dest_rect().w);
            if(enemigo->get_velocidad_x() == velocidad_enemigo_inicial)
                enemigo->cambiar_direccion();
        }
        else if (colision_arriba(escenario, enemigo) && !enemigo->esta_muerto()){
            enemigo->set_enemigo_en_aire(false);
            enemigo->set_dest_rect_y(escenario->get_dest_rect().y - enemigo->get_dest_rect().h);
            colision_superior = true;
        }
    }
    if(!colision_superior || enemigo->esta_muerto())
        enemigo->set_enemigo_en_aire(true);
}

void Colisionador::enemigo_colisiona_con_enemigo(Enemigo* enemigo){
    if(!enemigo->esta_muerto()){
        for(Enemigo* enemigo_extra: enemigos){
            if((colision_izquierda(enemigo_extra, enemigo) || colision_derecha(enemigo_extra, enemigo)) &&
               (enemigo_extra->get_dest_rect().x != enemigo->get_dest_rect().x) && !enemigo_extra->esta_muerto()){
                if(colision_izquierda(enemigo_extra, enemigo)){
                    enemigo->set_dest_rect_x(enemigo_extra->get_dest_rect().x - enemigo->get_dest_rect().w);
                }else
                    enemigo->set_dest_rect_x(enemigo_extra->get_dest_rect().x + enemigo_extra->get_dest_rect().w);

                enemigo->cambiar_direccion();
            }
        }
    }
}

