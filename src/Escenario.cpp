#include "Escenario.h"
#include "Texturas.h"
#include "Renderer.h"
#include <SDL2/SDL.h>

Escenario::Escenario(){
    texturas.flip = SDL_FLIP_NONE;
    usado = false;
}