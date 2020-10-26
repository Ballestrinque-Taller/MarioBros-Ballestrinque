#include "Ladrillo.h"
#include "Escenario.h"
#include "Camara.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define ANCHO_FRAME 16
#define ALTO_FRAME 16

#define POS_INICIAL_X_FRAME 0
#define POS_INICIAL_Y_FRAME 0
#define ALTO_LADRILLO_PANTALLA 80
#define ANCHO_LADRILLO_PANTALLA 80

Ladrillo::Ladrillo(SDL_Renderer* renderer, int pos_x, int pos_y): Escenario(){
    //for(int i=0; i<3; i++) {
        set_dest_rect(pos_x, pos_y, ALTO_LADRILLO_PANTALLA, ANCHO_LADRILLO_PANTALLA);
        set_src_rect(POS_INICIAL_X_FRAME, POS_INICIAL_Y_FRAME, ALTO_FRAME, ANCHO_FRAME);
        path_to_image = "./res/LADRILLO.png";
        renderizar(renderer);
    //}
    //ANCHO_LADRILLO_PANTALLA*i +


}

void Ladrillo::cambiar_frame(SDL_Renderer* renderer, Camara* camara){
    camara->acomodar_a_imagen(this);
    SDL_RenderCopyEx(renderer, texturas.textura, &(frames_render.src_rect), &(frames_render.dest_rect), 0, NULL, texturas.flip);
}