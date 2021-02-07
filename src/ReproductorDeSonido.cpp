#include "ReproductorDeSonido.h"
#include "Log.h"
#include <iostream>

ReproductorDeSonido::ReproductorDeSonido(){
    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
        LOG(Log::ERROR)<<"SDL_mixer no pudo inicializar! SDL_mixer Error: %s"<<Mix_GetError()<<std::endl;

    inicializar_diccionario_sonidos_especiales();
    inicializar_diccionario_de_musica();
}

void ReproductorDeSonido::reproducir_sonido(uint8_t id_sonido) {
    if(id_sonido == NO_HAY_SONIDO)
        return;

    Mix_Chunk* sonido = diccionario_sonido_especiales[id_sonido];
    Mix_PlayChannel(-1,sonido,0);
}

void ReproductorDeSonido::reproducir_musica(uint8_t id_musica) {
    if(id_musica != id_musica_sonando)
        Mix_HaltMusic();

    Mix_Music* musica = diccionario_musica[id_musica];

    if( Mix_PlayingMusic() == 0 )
    {
        id_musica_sonando = id_musica;
        Mix_PlayMusic( musica, -1 );
    }
}

void ReproductorDeSonido::toggle_musica() {
    if (Mix_PlayingMusic() != 0)
    {
        if( Mix_PausedMusic() == 1 )
        {
            Mix_ResumeMusic();
        }
        else
        {
            Mix_PauseMusic();
        }
    }
}

void ReproductorDeSonido::inicializar_diccionario_de_musica() {
    Mix_Music *musica_original, *musica_victoria;
    Mix_VolumeMusic(MIX_MAX_VOLUME/4);

    musica_original = Mix_LoadMUS( "./res/sonidos/musica_original.wav" );
    if( musica_original == nullptr ){
        LOG(Log::ERROR)<<"No se puedo inicializar la musica del path: ./res/sonidos/musica_original.wav"<<std::endl;
    }
    diccionario_musica.insert(std::pair<int,Mix_Music*>(MUSICA_ORIGINAL,musica_original));

    musica_victoria = Mix_LoadMUS( "./res/sonidos/musica_victoria.wav" );
    if( musica_victoria == nullptr ){
        LOG(Log::ERROR)<<"No se puedo inicializar la musica del path: ./res/sonidos/musica_victoria.wav"<<std::endl;
    }
    diccionario_musica.insert(std::pair<int,Mix_Music*>(MUSICA_VICTORIA,musica_victoria));
}

void ReproductorDeSonido::inicializar_diccionario_sonidos_especiales(){
    Mix_Chunk *sonido_moneda, *sonido_salto, *sonido_pisar_enemigo, *sonido_crecimiento, *sonido_cambio_nivel,
              *sonido_game_over, *sonido_muerte, *sonido_hongo, *sonido_perder_vida;

    sonido_moneda = Mix_LoadWAV( "./res/sonidos/sonido_moneda.wav" );
    Mix_VolumeChunk(sonido_moneda,20);
    if( sonido_moneda == nullptr )
    {
        LOG(Log::ERROR)<<"No se puedo inicializar el sonido del path: ./res/sonidos/sonido_moneda.wav"<<std::endl;
    }

    sonido_cambio_nivel = Mix_LoadWAV( "./res/sonidos/sonido_fin_de_nivel.wav" );
    Mix_VolumeChunk(sonido_cambio_nivel,70);
    if( sonido_cambio_nivel == nullptr )
    {
        LOG(Log::ERROR)<<"No se puedo inicializar el sonido del path: ./res/sonidos/sonido_fin_de_nivel.wav"<<std::endl;
    }

    sonido_crecimiento = Mix_LoadWAV( "./res/sonidos/sonido_crecimiento.wav" );
    Mix_VolumeChunk(sonido_crecimiento,100);
    if( sonido_crecimiento == nullptr )
    {
        LOG(Log::ERROR)<<"No se puedo inicializar el sonido del path: ./res/sonidos/sonido_crecimiento.wav"<<std::endl;
    }

    sonido_game_over = Mix_LoadWAV( "./res/sonidos/sonido_game_over.wav" );
    Mix_VolumeChunk(sonido_game_over,100);
    if( sonido_game_over == nullptr )
    {
        LOG(Log::ERROR)<<"No se puedo inicializar el sonido del path: ./res/sonidos/sonido_game_over.wav"<<std::endl;
    }

    sonido_hongo = Mix_LoadWAV( "./res/sonidos/sonido_hongo.wav" );
    Mix_VolumeChunk(sonido_hongo,128);
    if( sonido_hongo == nullptr )
    {
        LOG(Log::ERROR)<<"No se puedo inicializar el sonido del path: ./res/sonidos/sonido_hongo.wav"<<std::endl;
    }

    sonido_muerte = Mix_LoadWAV( "./res/sonidos/sonido_muerte.wav" );
    Mix_VolumeChunk(sonido_muerte,128);
    if( sonido_moneda == nullptr )
    {
        LOG(Log::ERROR)<<"No se puedo inicializar el sonido del path: ./res/sonidos/sonido_muerte.wav"<<std::endl;
    }

    sonido_pisar_enemigo = Mix_LoadWAV( "./res/sonidos/sonido_pisar_enemigos.wav" );
    Mix_VolumeChunk(sonido_pisar_enemigo,64);
    if( sonido_pisar_enemigo == nullptr )
    {
        LOG(Log::ERROR)<<"No se puedo inicializar el sonido del path: ./res/sonidos/sonido_pisar_enemigos.wav"<<std::endl;
    }

    sonido_salto = Mix_LoadWAV( "./res/sonidos/sonido_salto.wav" );
    Mix_VolumeChunk(sonido_salto,20);
    if( sonido_salto == nullptr )
    {
        LOG(Log::ERROR)<<"No se puedo inicializar el sonido del path: ./res/sonidos/sonido_salto.wav"<<std::endl;
    }

    sonido_perder_vida = Mix_LoadWAV( "./res/sonidos/sonido_perder_vida.wav" );
    Mix_VolumeChunk(sonido_perder_vida,60);
    if( sonido_perder_vida == nullptr )
    {
        LOG(Log::ERROR)<<"No se puedo inicializar el sonido del path: ./res/sonidos/sonido_perder_vida.wav"<<std::endl;
    }

    diccionario_sonido_especiales.insert(std::pair<int,Mix_Chunk*>(SONIDO_CRECIMIENTO,sonido_crecimiento));
    diccionario_sonido_especiales.insert(std::pair<int,Mix_Chunk*>(SONIDO_FIN_DE_NIVEL,sonido_cambio_nivel));
    diccionario_sonido_especiales.insert(std::pair<int,Mix_Chunk*>(SONIDO_GAME_OVER,sonido_game_over));
    diccionario_sonido_especiales.insert(std::pair<int,Mix_Chunk*>(SONIDO_HONGO,sonido_hongo));
    diccionario_sonido_especiales.insert(std::pair<int,Mix_Chunk*>(SONIDO_MONEDA,sonido_moneda));
    diccionario_sonido_especiales.insert(std::pair<int,Mix_Chunk*>(SONIDO_MUERTE,sonido_muerte));
    diccionario_sonido_especiales.insert(std::pair<int,Mix_Chunk*>(SONIDO_PISAR_ENEMIGOS,sonido_pisar_enemigo));
    diccionario_sonido_especiales.insert(std::pair<int,Mix_Chunk*>(SONIDO_SALTO,sonido_salto));
    diccionario_sonido_especiales.insert(std::pair<int,Mix_Chunk*>(SONIDO_PERDER_VIDA,sonido_perder_vida));
}

ReproductorDeSonido::~ReproductorDeSonido() {
    for (std::map<int, Mix_Chunk*>::iterator it = diccionario_sonido_especiales.begin(); it != diccionario_sonido_especiales.end(); ++it)
    {
        Mix_FreeChunk(it->second);
    }
    Mix_FreeMusic(diccionario_musica[MUSICA_ORIGINAL]);
    diccionario_musica.clear();
    diccionario_sonido_especiales.clear();
    Mix_Quit();
}

bool ReproductorDeSonido::musica_encendida(){
    return (Mix_PausedMusic() != 1 && Mix_PlayingMusic() != 0);
}
