#dir_path = $(dir $(realpath $(firstword $(MAKEFILE_LIST))))
CXX := g++
SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin
INC_DIR := include
flags := -I$(INC_DIR) -MMD -MP -std=c++11 -Wall
SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
#OBJS = Background.o Camara.o Enemigo.o Escenario.o Juego.o Jugador.o Koopa.o Ladrillo.o main.o Moneda.o Renderer.o Sorpresa.o Tortuga.o
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SOURCES))

ifeq ($(OS),Windows_NT)
	libs = -lmingw32 -lSDL2main -lSDL2_image -lSDL2
else
	libs = -lSDL2_image -lSDL2
endif


.PHONY: all clear

all: $(BIN_DIR)/app.exe

$(BIN_DIR)/app.exe: $(OBJS)
	$(CXX) $^ $(libs) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) -c $< $(flags) -o $@

$(OBJ_DIR):
	mkdir -p $@

clear:
	@$(RM) -rv $(OBJ_DIR)
	@$(RM) $(BIN_DIR)/app.exe

-include $(OBJS:.o=.d)