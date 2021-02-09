#ifndef MARIOBROS_BALLESTRINQUE_JUGADOR_H
#define MARIOBROS_BALLESTRINQUE_JUGADOR_H

#include "Renderer.h"
#include "Camara.h"
#include "ReproductorDeSonido.h"
#include <vector>

#define MAX_ACELERACION 8

#define COLISION_SUPERIOR 0
#define COLISION_INFERIOR 1
#define COLISION_IZQUIERDA 2
#define COLISION_DERECHA 3
#define NO_COLISIONA -1

#define PUNTOS_MONEDAS 50

#define MAX_VIDAS 3

#define MAX_SEGS_INMUNE 1


class Jugador: public Renderer{
    public:
        int get_velocidad_x();
        int get_velocidad_y();
        uint8_t get_cantidad_vidas();
        size_t get_puntaje();
        void grisar();
        void reconectar();
        Jugador(std::string path);
        void acelerar_x(int direccion);
        void saltar();
        void desplazar();
        void cambiar_frame(Camara* camara) override;
        void recibir_evento(SDL_Event evento);
        void agacharse();
        void reset_posicion();
        void set_sonido_a_reproducir(uint8_t sonido);
        uint8_t get_sonido_a_reproducir();
        bool esta_desconectado();
        bool esta_inmune();
        bool esta_crecido();
        bool esta_muerto();
        bool finalizo_nivel();
        void finalizar_nivel(bool termino);
        void colisionar_con_bloque(int direccion);
        void colisionar_con_enemigo(int direccion);
        void colisionar_con_moneda();
        void crecer();
        void sumar_puntos(int tipo_enemigo);
        void sumar_puntos_fin_nivel(int puntos_a_sumar);
        void agregar_cliente_que_recibio_sonido(int cliente);
        bool cliente_recibio_sonido(int cliente);
        void clear_clientes_que_recibieron_sonido();

    private:
        bool termino_nivel = false;
        bool modo_test = false;
        bool muerto = false;
        int puntos = 0;
        int vidas = MAX_VIDAS;
        bool desconectado = false;
        bool acelerando;
        bool en_aire;
        bool agachado;
        int estado_crecimiento;
        bool inmune = false;
        int tiempo_inmune = 9900;
        int TickDanio = 0;
        uint8_t sonido_a_reproducir = NO_HAY_SONIDO;
        std::vector<int> clientes_que_recibieron_sonido;

        int velocidad_x;
        int velocidad_y;
        int max_acel;
        int frame_actual;
        int tick_actual;

        void aceleracion_gravitatoria();
        void animacion_crecimiento();
        void rozamiento();
};


#endif //MARIOBROS_BALLESTRINQUE_JUGADOR_H
