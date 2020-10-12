#include <iostream>
#include <SDL2/SDL.h>
//#include "res_path.h"

const int ANCHO_VENTANA = 800;
const int ALTO_VENTANA = 600;
const int ERROR = -1;

int inicializar_ventana(SDL_Window* win, SDL_Renderer* ren);

int main(int, char**){
    
    SDL_Window * ventana = nullptr;
    SDL_Renderer * renderer = nullptr;
    bool quit = false;

    int valor = inicializar_ventana(ventana, renderer);
    if (valor == ERROR)
        quit = true;
    
    SDL_Event evento;
    while (!quit){
        SDL_RenderClear(renderer);
        
        SDL_RenderPresent(renderer);
        
        
        while (SDL_PollEvent (&evento) != 0){
            if (evento.type == SDL_KEYDOWN) {
                switch(evento.key.keysym.sym){
                    case (SDLK_ESCAPE):
                      quit=true;
                      break;
            }
        }
            
    }
}

	//Clean up our objects and quit
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(ventana);
	SDL_Quit();
	
	return 0;
}

int inicializar_ventana(SDL_Window* win, SDL_Renderer* ren){
	if (SDL_Init(SDL_INIT_VIDEO) != 0){
	   std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
	   return ERROR;
    }
    	
	win = SDL_CreateWindow("Hermanos Mario", 100, 100, ANCHO_VENTANA, ALTO_VENTANA, SDL_WINDOW_SHOWN);
	if (win == nullptr){
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		return ERROR;
	}


	ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (ren == nullptr){
		SDL_DestroyWindow(win);
		std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return ERROR;
	}
    return 0;
}


