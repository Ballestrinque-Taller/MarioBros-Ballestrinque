#include "Background.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Texturas.h"


#define POS_Y 0
#define POS_X_INIC 0



Background::Background(std::string path, int ancho, int alto){
    ancho_imagen = ancho;
    alto_imagen = alto;
    fin_nivel = ancho_imagen - alto_imagen*4/3;
    set_src_rect(POS_X_INIC, POS_Y, alto_imagen, alto_imagen*RATIO_ASPECTO);
    set_dest_rect(POS_X_INIC, POS_Y, HEIGHT, WIDTH);
    texturas.flip = SDL_FLIP_NONE;
    path_to_image = path;
    default_path = "./res/Fondo_default.png";
    pos_x_actual = 0;
}


void Background::scroll(int velocidad_scroll){
    float ancho_imagen_en_pantalla = alto_imagen*4/3;
    pos_x_actual += velocidad_scroll*ancho_imagen_en_pantalla/WIDTH;
    set_src_rect(pos_x_actual, POS_Y, alto_imagen, alto_imagen*4/3);
}

bool Background::es_fin_nivel(){
    return ((fin_nivel-pos_x_actual)<=0);
}

