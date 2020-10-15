#include "../include/Jugador.h"
#include "../include/Renderer.h"


Jugador::Jugador(){
    path_to_image = "../res/mario.png";
    set_dest_rect(0,0,200,200);
    set_src_rect(0,0,920,920);
}


void Jugador::mover_pos_x(int x) {
    frames_render.dest_rect.x= frames_render.dest_rect.x - x;
}

void Jugador::mover_pos_y(int y) {
    frames_render.dest_rect.y= frames_render.dest_rect.y - y;
}