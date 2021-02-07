#ifndef MARIOBROS_BALLESTRINQUE_DIBUJADOR_H
#define MARIOBROS_BALLESTRINQUE_DIBUJADOR_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <vector>
#include "Mensajes.h"

typedef struct puntajes_tabla{
    std::string usuario;
    std::vector<int> puntajes_rondas;
}puntajes_tabla_t;

class Dibujador{
    public:
        Dibujador();
        void dibujar(std::vector<entidad_t> entidades_a_dibujar, TextWriter* nivel_label, int nivel_actual, TextWriter* temporizador_label, int tiempo_restante, SDL_Renderer* renderer);
        void crear_texturas(std::vector<entidad_t> entidades_a_texturizar, SDL_Renderer* renderer);
        ~Dibujador();
        void dibujar_cambio_nivel(std::vector<entidad_t> jugadores, std::string texto_arriba, SDL_Renderer* renderer);
        void dibujar_fin_juego(std::vector<entidad_t> jugadores, SDL_Renderer* renderer);
        void set_ronda_cambiada(bool ronda_cambiada);
    private:
        bool es_ronda_cambiada = false;
        bool usuarios_registrados = false;
        void registrar_usuarios(std::vector<entidad_t> jugadores);
        int cantidad_de_jugadores=0;
        bool genere_identificadores=false;
        std::vector<TextWriter*> text_writers;
        std::vector<TextWriter*> text_puntos;
        std::vector<TextWriter*> nombres_jugadores;
        std::vector<TextWriter*> puntajes_jugadores;
        std::vector<puntajes_tabla_t> puntajes_tabla;
        TextWriter* texto_nivel;
        void generar_identificador_jugador();
        void dibujar_tabla_puntajes(std::vector<entidad_t>* jugadores, SDL_Renderer* renderer);
        void agregar_puntajes_restantes(std::vector<entidad_t> jugadores);
        void establecer_dimensiones_jugadores(std::vector<entidad_t>* jugadores);
        void escribir_puntajes_y_nombres(std::vector<entidad_t> jugadores, SDL_Renderer* renderer);
        void dibujar_lineas_tabla(SDL_Renderer* renderer);
        std::vector<SDL_Texture*> texturas;
        SDL_Texture* crear_textura(entidad_t entidad, SDL_Renderer* renderer);
};

#endif //MARIOBROS_BALLESTRINQUE_DIBUJADOR_H
