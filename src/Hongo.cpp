#include "Hongo.h"

#define MAX_ACEL_GRAVEDAD 10
#define DECAIMIENTO_ACEL_Y 1

Hongo::Hongo(int x, int y){
    set_src_rect(0,0,ALTO_SOURCE,ANCHO_SOURCE);
    set_dest_rect(x,y,ALTO_HONGO,ANCHO_HONGO);
    velocidad_x = VELOCIDAD_HONGO;
    velocidad_y = 0;
    path_to_image = "./res/mario.png";
}

void Hongo::desplazar(){
    aceleracion_gravitatoria();
    set_dest_rect(get_dest_rect().x+velocidad_x,get_dest_rect().y+velocidad_y,get_dest_rect().h, get_dest_rect().w);
}

void Hongo::cambiar_direccion() {
    velocidad_x = -velocidad_x;
}

void Hongo::aceleracion_gravitatoria(){
    if(!en_aire)
        velocidad_y = 0;
    if(velocidad_y < MAX_ACEL_GRAVEDAD && en_aire){
        velocidad_y += DECAIMIENTO_ACEL_Y;
    }
}

void Hongo::set_hongo_en_aire(bool esta_en_aire){
    en_aire = esta_en_aire;
    if(!en_aire)
        velocidad_y = 0;
}