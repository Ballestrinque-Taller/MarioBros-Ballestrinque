#dir_path = $(dir $(realpath $(firstword $(MAKEFILE_LIST))))
flags = -std=c++11 -Wall
OBJS = Background.o Camara.o Enemigo.o Escenario.o Juego.o Jugador.o Koopa.o Ladrillo.o main.o Moneda.o Renderer.o Sorpresa.o Tortuga.o
ifeq ($(OS),Windows_NT)
	libs = -lmingw32 -lSDL2main -lSDL2_image -lSDL2
else
	libs = -lSDL2_image -lSDL2
endif

all: $(OBJS)
	g++ $(OBJS) $(libs) $(flags) -I./include  -o ./bin/app.exe

%.o: ./src/%.cpp
	g++ $(libs_win) $(libs) $(flags) -I./include -c $^ -o $@

.PHONY: clear
clear: 
	rm ./*.o