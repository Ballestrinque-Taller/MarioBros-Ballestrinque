#ifndef MARIOBROS_BALLESTRINQUE_DIBUJADOR_H
#define MARIOBROS_BALLESTRINQUE_DIBUJADOR_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <vector>
#include "Mensajes.h"

class Dibujador{
    public:
        Dibujador();
        void dibujar(std::vector<entidad_t> entidades_a_dibujar, TextWriter* nivel_label, int nivel_actual, TextWriter* temporizador_label, int tiempo_restante, SDL_Renderer* renderer);
        void crear_texturas(std::vector<entidad_t> entidades_a_texturizar, SDL_Renderer* renderer);
        ~Dibujador();
        void dibujar_cambio_nivel(std::vector<entidad_t> jugadores, std::string texto_arriba, SDL_Renderer* renderer);
        void dibujar_fin_juego(std::vector<entidad_t> jugadores, SDL_Renderer* renderer);
    private:
        int cantidad_de_jugadores;
        bool genere_identificadores=false;
        std::vector<TextWriter*> text_writers;
        std::vector<TextWriter*> text_puntos;
        std::vector<TextWriter*> nombres_jugadores;
        std::vector<TextWriter*> puntajes_jugadores;
        TextWriter* texto_nivel;
        void generar_identificador_jugador();
        std::vector<SDL_Texture*> texturas;
        SDL_Texture* crear_textura(entidad_t entidad, SDL_Renderer* renderer);
};

#endif //MARIOBROS_BALLESTRINQUE_DIBUJADOR_H
