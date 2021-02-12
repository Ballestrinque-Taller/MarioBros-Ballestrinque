#include "Hongo.h"

#define MAX_ACEL_GRAVEDAD 10
#define DECAIMIENTO_ACEL_Y 1

Hongo::Hongo(int x, int y){
    set_src_rect(0,0,ALTO_SOURCE,ANCHO_SOURCE);
    set_dest_rect(x,y,ALTO_HONGO,ANCHO_HONGO);
    velocidad_x = 0;
    velocidad_y = 0;
    path_to_image = "./res/ItemsObjectsNPCS.png";
}

void Hongo::animacion_emerger() {
    if (animacion > 40) {
        velocidad_y = 0;
    }
    else {
        velocidad_y = -0.5;
        ++animacion;
    }
}

void Hongo::desplazar(){
    
    animacion_emerger();
    frames_render.dest_rect.x += velocidad_x;
    frames_render.dest_rect.y += velocidad_y;
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