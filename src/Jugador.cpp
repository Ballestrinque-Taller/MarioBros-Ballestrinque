#include "../include/Jugador.h"
#include "../include/Renderer.h"


Jugador::Jugador(){
    path_to_image = "../res/mario.png";
    set_dest_rect(0,0,200,200);
    set_src_rect(0,0,920,920);
}