#ifndef MARIOBROS_BALLESTRINQUE_REPRODUCTORDESONIDO_H
#define MARIOBROS_BALLESTRINQUE_REPRODUCTORDESONIDO_H

#include <SDL_mixer.h>
#include <map>

#define MUSICA_ORIGINAL 0
#define MUSICA_VICTORIA 1
#define NO_HAY_SONIDO 0
#define SONIDO_CRECIMIENTO 1
#define SONIDO_FIN_DE_NIVEL 2
#define SONIDO_GAME_OVER 3
#define SONIDO_HONGO 4
#define SONIDO_MONEDA 5
#define SONIDO_MUERTE 6
#define SONIDO_PISAR_ENEMIGOS 7
#define SONIDO_SALTO 8
#define SONIDO_PERDER_VIDA 9

class ReproductorDeSonido {
    public:
        ReproductorDeSonido();
        void reproducir_musica(uint8_t id_musica=MUSICA_ORIGINAL);
        void reproducir_sonido(uint8_t id_sonido);
        void toggle_musica();
        bool musica_encendida();
        ~ReproductorDeSonido();

    private:
        void inicializar_diccionario_de_musica();
        void inicializar_diccionario_sonidos_especiales();
        int id_musica_sonando;
        std::map<int,Mix_Chunk*> diccionario_sonido_especiales;
        std::map<int,Mix_Music*> diccionario_musica;
};


#endif //MARIOBROS_BALLESTRINQUE_REPRODUCTORDESONIDO_H
