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
#define FRAME_MUERTO 6

#define ESTA_CRECIENDO 2
#define CRECIDO 1
#define NO_CRECIDO 0

#define TIEMPO_ANIMACION_CRECIMIENTO 1200 //en milisegundos

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
    if(!muerto) {
        if (tick_actual >= TIEMPO_FRAME && velocidad_x != 0 && !en_aire) {
            tick_actual = 0;
            frame_actual++;
            if (frame_actual > FRAME_MOV_FINAL)
                frame_actual = 0;
        } else if (agachado) {
            tick_actual = 0;
            frame_actual = FRAME_AGACHADO;
        } else if (en_aire) {
            tick_actual = 0;
            frame_actual = FRAME_SALTO;
        } else if (velocidad_x == 0 && !en_aire) {
            tick_actual = 0;
            frame_actual = 0;
        }
        if (estado_crecimiento == CRECIDO) {
            set_src_rect(frame_actual * ANCHO_FRAME, 1, ALTO_FRAME, ANCHO_FRAME);
            set_dest_rect(get_dest_rect().x, get_dest_rect().y, ALTO_PANTALLA, ANCHO_PANTALLA);
        } else if (estado_crecimiento == NO_CRECIDO) {
            set_src_rect(frame_actual * ANCHO_FRAME, ALTO_FRAME + 1, ALTO_FRAME / 2, ANCHO_FRAME);
            set_dest_rect(get_dest_rect().x, get_dest_rect().y, ALTO_PANTALLA / 2, ANCHO_PANTALLA);
        } else if (estado_crecimiento == ESTA_CRECIENDO)
            animacion_crecimiento();
        camara->acomodar_a_imagen(this);
    }else{
        frame_actual = FRAME_MUERTO;
        set_src_rect(frame_actual * ANCHO_FRAME, ALTO_FRAME + 1, ALTO_FRAME / 2, ANCHO_FRAME);
        en_aire = true;
        velocidad_y = 2;
    }
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
        clear_clientes_que_recibieron_sonido();
    }
}

void Jugador::desplazar(){
    frames_render.dest_rect.x += velocidad_x;
    aceleracion_gravitatoria();
    frames_render.dest_rect.y += velocidad_y;
    if(frames_render.dest_rect.y > 600){
        if(!modo_test && vidas > 0) {
            estado_crecimiento = NO_CRECIDO;
            vidas--;
            sonido_a_reproducir = SONIDO_PERDER_VIDA;
            clear_clientes_que_recibieron_sonido();
        }
        if(vidas<=0 && !muerto) {
            muerto = true;
            sonido_a_reproducir = SONIDO_MUERTE;
            clear_clientes_que_recibieron_sonido();
        }
        if(!muerto)
            reset_posicion();
    }
    acelerando = false;
}

void Jugador::aceleracion_gravitatoria() {
    if(velocidad_y < MAX_ACEL_GRAVEDAD && en_aire){
        velocidad_y += DECAIMIENTO_ACEL_Y;
    }
}

void Jugador::crecer(){
    estado_crecimiento = ESTA_CRECIENDO;
    //set_dest_rect(get_dest_rect_x(), get_dest_rect().y-ALTO_PANTALLA/2, get_dest_rect().h, get_dest_rect().w);
}

void Jugador::recibir_evento(SDL_Event evento) {
    if (evento.type == SDL_KEYDOWN){
        switch (evento.key.keysym.sym) {
                case (SDLK_LEFT):
                    if(!muerto) {
                        texturas.flip = SDL_FLIP_HORIZONTAL;
                        acelerar_x(IZQUIERDA);
                    }
                    break;
                case (SDLK_RIGHT):
                    if(!muerto) {
                        texturas.flip = SDL_FLIP_NONE;
                        acelerar_x(DERECHA);
                    }
                    break;
                case (SDLK_DOWN):
                    agacharse();
                    break;
                case (SDLK_UP):
                    saltar();
                    break;
                case (SDLK_t):
                    modo_test = !modo_test;
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
    if (!en_aire && estado_crecimiento == CRECIDO){
        agachado = true;
        velocidad_x=0;
    }
}

void Jugador::reset_posicion(){
    if(!muerto)
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
            if(!muerto) {
                velocidad_y = 0;
                en_aire = false;
            }
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
            if(!modo_test) {
                if (estado_crecimiento == NO_CRECIDO) {
                    if (vidas > 0) {
                        vidas--;
                    }
                    if (vidas <= 0){
                        muerto = true;
                        sonido_a_reproducir = SONIDO_MUERTE;
                        clear_clientes_que_recibieron_sonido();
                    }
                }
                else {
                    set_dest_rect_y(get_dest_rect().y + get_dest_rect().h / 2);
                }
                if(!muerto) {
                    sonido_a_reproducir = SONIDO_PERDER_VIDA;
                    clear_clientes_que_recibieron_sonido();
                }
                estado_crecimiento = NO_CRECIDO;
                TickDanio = SDL_GetTicks();
            }
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
    clear_clientes_que_recibieron_sonido();
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

bool Jugador::esta_muerto() {
    return muerto;
}
bool Jugador::finalizo_nivel(){
    return termino_nivel;
}
void Jugador::finalizar_nivel(bool termino){
    termino_nivel = termino;
}

void Jugador::sumar_puntos_fin_nivel(int puntos_a_sumar){
    puntos += puntos_a_sumar;
}


void Jugador::animacion_crecimiento(){
    unsigned int tiempo_inicial = SDL_GetTicks();
    unsigned int tiempo_animacion = 0;
    int posicion_y = get_dest_rect().y;
    while(tiempo_animacion < TIEMPO_ANIMACION_CRECIMIENTO){

        //mario chiquito: 0-120 240-360 480-600 840-960
        if((tiempo_animacion > TIEMPO_ANIMACION_CRECIMIENTO/10*2 && tiempo_animacion < TIEMPO_ANIMACION_CRECIMIENTO/10*3)
            || (tiempo_animacion > TIEMPO_ANIMACION_CRECIMIENTO/10*4 && tiempo_animacion < TIEMPO_ANIMACION_CRECIMIENTO/10*5) || (tiempo_animacion > TIEMPO_ANIMACION_CRECIMIENTO/10*7 && tiempo_animacion < TIEMPO_ANIMACION_CRECIMIENTO/10*8) ){

            set_src_rect(frame_actual * ANCHO_FRAME, ALTO_FRAME+1, ALTO_FRAME/2, ANCHO_FRAME);
            set_dest_rect(get_dest_rect().x,posicion_y, ALTO_PANTALLA/2, ANCHO_PANTALLA);
        }
        //mario mediano: 120-240 360-480 600-720 960-1080
        if((tiempo_animacion > TIEMPO_ANIMACION_CRECIMIENTO/10 && tiempo_animacion < TIEMPO_ANIMACION_CRECIMIENTO/10*2) || (tiempo_animacion > (TIEMPO_ANIMACION_CRECIMIENTO/10)*3 && tiempo_animacion < (TIEMPO_ANIMACION_CRECIMIENTO/10)*4)
            || (tiempo_animacion > TIEMPO_ANIMACION_CRECIMIENTO/10*5 && tiempo_animacion < (TIEMPO_ANIMACION_CRECIMIENTO/10)*6) || (tiempo_animacion > (TIEMPO_ANIMACION_CRECIMIENTO/10)*8 && tiempo_animacion < (TIEMPO_ANIMACION_CRECIMIENTO/10)*9) ){

            set_src_rect(frame_actual * ANCHO_FRAME, 1, ALTO_FRAME, ANCHO_FRAME);
            set_dest_rect(get_dest_rect().x,posicion_y-ALTO_PANTALLA/4, (ALTO_PANTALLA*3)/4, ANCHO_PANTALLA);
        }
        //mario grande: 720-840 1080-1200
        if((tiempo_animacion > TIEMPO_ANIMACION_CRECIMIENTO/10*6 && tiempo_animacion < TIEMPO_ANIMACION_CRECIMIENTO/10*7) || (tiempo_animacion > TIEMPO_ANIMACION_CRECIMIENTO/10*9 && tiempo_animacion < TIEMPO_ANIMACION_CRECIMIENTO)){
            set_src_rect(frame_actual * ANCHO_FRAME, 1, ALTO_FRAME, ANCHO_FRAME);
            set_dest_rect(get_dest_rect().x, posicion_y-ALTO_PANTALLA/2, ALTO_PANTALLA, ANCHO_PANTALLA);
        }

        tiempo_animacion = SDL_GetTicks() - tiempo_inicial;
    }

    estado_crecimiento = CRECIDO;
}

void Jugador::agregar_cliente_que_recibio_sonido(int cliente){
    clientes_que_recibieron_sonido.push_back(cliente);
}
bool Jugador::cliente_recibio_sonido(int cliente){
    for(int cliente_array : clientes_que_recibieron_sonido){
        if(cliente_array == cliente)
            return true;
    }
    return false;
}
void Jugador::clear_clientes_que_recibieron_sonido(){
    clientes_que_recibieron_sonido.clear();
}
