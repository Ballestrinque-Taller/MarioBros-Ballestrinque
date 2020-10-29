#include "Temporizador.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

Temporizador :: Temporizador(size_t tiempo_duracion_nivel){

    tiempo_inicial = SDL_GetTicks();
    tiempo_restante = tiempo_duracion_nivel;

}

void Temporizador :: update(){
    size_t tiempo_actual = SDL_GetTicks();
    size_t diferencia_de_tiempo =  tiempo_actual - tiempo_inicial;
    tiempo_restante -= diferencia_de_tiempo/1000;
    tiempo_inicial = tiempo_actual; //Para que en la siguiente iteracion se calcule bien la diferencia de tiempo.
}

void Temporizador::render(SDL_Renderer* renderer){
    TTF_Font* fuente = TTF_OpenFont("./res/Fuente_texto.ttf", 24); //setear fuente
    SDL_Color White = {255, 255, 255}; //setear color
    SDL_Surface* surfaceMessage = TTF_RenderText_Solid( fuente, "Time", White );
    SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
    SDL_Rect Message_rect; //create a rect
    Message_rect.x = 0;  //controls the rect's x coordinate
    Message_rect.y = 0; // controls the rect's y coordinte
    Message_rect.w = 100; // controls the width of the rect
    Message_rect.h = 100; // controls the height of the rect

    SDL_RenderCopy(renderer, Message, NULL, &Message_rect); //you put the renderer's name first, the Message, the crop size(you can ignore this if you don't want to dabble with cropping), and the rect which is the size and coordinate of your texture

//Don't forget to free your surface and texture
    SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(Message);
}