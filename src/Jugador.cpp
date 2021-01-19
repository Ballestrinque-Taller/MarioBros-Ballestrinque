#include "Jugador.h"
#include "Renderer.h"
#include "Camara.h"
#include "Enemigo.h"


#define DERECHA 1
#define IZQUIERDA -1


#define MAX_CORRIDA 12
#define TICK_ACELERACION 4
#define DECAIMIENTO_ACEL_Y 1
//#define DECAIMIENTO_ROZAMIENTO 1
#define MAX_ACEL_GRAVEDAD 10
#define ACELERACION_SALTO 20

#define VELOCIDAD_MATAR_ENEMIGO_SALTO 10

#define ANCHO_FRAME 17
#define ALTO_FRAME 32
#define ANCHO_IMAGEN 358

#define ANCHO_PANTALLA 40
#define ALTO_PANTALLA 80

#define TIEMPO_FRAME 5
#define FRAME_SALTO 5
#define FRAME_MOV_FINAL 3
#define FRAME_AGACHADO 6

#define CRECIDO 1
#define NO_CRECIDO 0

Jugador::Jugador(std::string path){
    path_to_image = path;
    default_path = "./res/Mario_default.png";
    set_dest_rect(0,0,ALTO_PANTALLA,ANCHO_PANTALLA);
    set_src_rect(1,1,ALTO_FRAME,ANCHO_FRAME);
    velocidad_x = 0;
    velocidad_y = 0;
    max_acel = MAX_ACELERACION;
    frame_actual = 0;
    agachado = false;
    en_aire = true;
    tick_actual = TIEMPO_FRAME;
    texturas.flip = SDL_FLIP_NONE;
    estado_crecimiento = NO_CRECIDO;
}

void Jugador::cambiar_frame(Camara* camara){
    tick_actual++;
    if (tick_actual >= TIEMPO_FRAME && velocidad_x!= 0 && !en_aire){
        tick_actual = 0;
        frame_actual++;
            if(frame_actual > FRAME_MOV_FINAL)
                frame_actual = 0;
    }
    else if(agachado){
        tick_actual = 0;
        frame_actual = FRAME_AGACHADO;
    }
    else if(en_aire){
        tick_actual = 0;
        frame_actual = FRAME_SALTO;
    }
    else if (velocidad_x == 0 && !en_aire){
        tick_actual = 0;
        frame_actual = 0;
    }
    if (estado_crecimiento == CRECIDO) {
        set_src_rect(frame_actual * ANCHO_FRAME, 1, ALTO_FRAME, ANCHO_FRAME);
        set_dest_rect(get_dest_rect().x, get_dest_rect().y, ALTO_PANTALLA, ANCHO_PANTALLA);
    }else{
        set_src_rect(frame_actual * ANCHO_FRAME, ALTO_FRAME+1, ALTO_FRAME/2, ANCHO_FRAME);
        set_dest_rect(get_dest_rect().x,get_dest_rect().y, ALTO_PANTALLA/2, ANCHO_PANTALLA);
    }
    camara->acomodar_a_imagen(this);
}

int Jugador::get_velocidad_x(){
    return velocidad_x;
}

int Jugador::get_velocidad_y() {
    return velocidad_y;
}

void Jugador::acelerar_x(int direccion){
    if (direccion == DERECHA && velocidad_x < max_acel && !agachado){
        //velocidad_x += TICK_ACELERACION;
        velocidad_x = MAX_ACELERACION;
    }
    else if (direccion == IZQUIERDA && velocidad_x > -max_acel && !agachado){
        //velocidad_x -= TICK_ACELERACION;
        velocidad_x = -MAX_ACELERACION;
    }
    acelerando = true;
}

void Jugador::saltar() {
    if (velocidad_y == 0 && !en_aire) {
        velocidad_y = -ACELERACION_SALTO;
        en_aire = true;
        sonido_a_reproducir = SONIDO_SALTO;
    }
}

void Jugador::desplazar(){
    frames_render.dest_rect.x += velocidad_x;
    aceleracion_gravitatoria();
    frames_render.dest_rect.y += velocidad_y;
    //rozamiento();
    acelerando = false;
}

void Jugador::aceleracion_gravitatoria() {
    if(velocidad_y < MAX_ACEL_GRAVEDAD && en_aire){
        velocidad_y += DECAIMIENTO_ACEL_Y;
    }
}

void Jugador::crecer(){
    estado_crecimiento = CRECIDO;
    set_dest_rect(get_dest_rect_x(), get_dest_rect().y-ALTO_PANTALLA/2, get_dest_rect().h, get_dest_rect().w);
}

void Jugador::recibir_evento(SDL_Event evento) {
    if (evento.type == SDL_KEYDOWN){
        switch (evento.key.keysym.sym) {
                case (SDLK_LEFT):
                    texturas.flip = SDL_FLIP_HORIZONTAL;
                    acelerar_x(IZQUIERDA);
                    break;
                case (SDLK_RIGHT):
                    texturas.flip = SDL_FLIP_NONE;
                    acelerar_x(DERECHA);
                    break;
                case (SDLK_DOWN):
                    agacharse();
                    break;
                case (SDLK_UP):
                    saltar();
                    break;
        }
    }
    else if (evento.type == SDL_KEYUP){
        switch(evento.key.keysym.sym) {
            case (SDLK_DOWN):
                agachado = false;
                break;
            case (SDLK_LEFT):
                velocidad_x = 0;
                break;
            case (SDLK_RIGHT):
                velocidad_x = 0;
                break;
        }

    }
}

void Jugador::agacharse(){
    if (!en_aire){
        agachado = true;
        velocidad_x=0;
    }
}

void Jugador::reset_posicion(){
    set_dest_rect(0,0,ALTO_PANTALLA,ANCHO_PANTALLA);
    velocidad_x = 0;
}

void Jugador::grisar(){
    if(!desconectado) {
        std::string png_str(".png");
        for (int i = 0; i < png_str.size(); i++)
            path_to_image.erase(path_to_image.size() - 1);
        path_to_image += "_GRISADO.png";
    }
    velocidad_x = 0;
    desconectado = true;
}

void Jugador::colisionar_con_bloque(int direccion_colision) {
    switch(direccion_colision){
        case COLISION_SUPERIOR:
            velocidad_y= 0;
            en_aire = false;
            break;
        case COLISION_IZQUIERDA:
            velocidad_x = 0;
            break;
        case COLISION_DERECHA:
            velocidad_x = 0;
            break;
        case COLISION_INFERIOR:
            velocidad_y = 0;
            break;
        case NO_COLISIONA:
            en_aire = true;
            break;
    }
}

void Jugador::colisionar_con_enemigo(int direccion_colision) {
    tiempo_inmune = SDL_GetTicks() - TickDanio;
    if (tiempo_inmune > MAX_SEGS_INMUNE*1000)
        inmune = false;
    switch(direccion_colision){
        case COLISION_SUPERIOR:
            velocidad_y = -VELOCIDAD_MATAR_ENEMIGO_SALTO;
            break;
        case NO_COLISIONA:
            break;
        default:
            inmune = true;
            if(estado_crecimiento == NO_CRECIDO) {
                if(vidas>0)
                    vidas--;
            }else
                set_dest_rect_y(get_dest_rect().y+get_dest_rect().h/2);
            estado_crecimiento = NO_CRECIDO;
            TickDanio = SDL_GetTicks();
            break;
    }
}

bool Jugador::esta_inmune(){
    return inmune;
}

bool Jugador::esta_crecido() {
    return (estado_crecimiento == CRECIDO);
}

void Jugador::sumar_puntos(int tipo_enemigo){
    switch(tipo_enemigo){
        case GOOMBA:
            puntos+=500;
        case TORTUGA:
            puntos+=1000;
    }
}

void Jugador::colisionar_con_moneda(){
    puntos += PUNTOS_MONEDAS;
}

void Jugador::reconectar(){
    if (desconectado){
        std::string grisado("_GRISADO.png");
        for (int i=0; i<grisado.size(); i++)
            path_to_image.erase(path_to_image.size()-1);
        path_to_image += ".png";
    }
    desconectado = false;

}

bool Jugador::esta_desconectado(){
    return desconectado;
}

void Jugador::set_sonido_a_reproducir(uint8_t sonido) {
    sonido_a_reproducir = sonido;
}

uint8_t Jugador::get_sonido_a_reproducir() {
    return sonido_a_reproducir;
}

uint8_t Jugador::get_cantidad_vidas(){
    return vidas;
}

size_t Jugador::get_puntaje(){
    return puntos;
}
