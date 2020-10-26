#dir_path = $(dir $(realpath $(firstword $(MAKEFILE_LIST))))

default:
	@echo "usar 'make all_win' para compilar en windows y 'make all_linux' para Linux"

all_win:
	g++ ./src/*.cpp -I./include -lmingw32 -lSDL2_image -lSDL2main -lSDL2 -std=c++11 -o app.exe -Wall -g
	
all_linux:
	g++ ./src/*.cpp -I./include -lSDL2_image -lSDL2 -std=c++11 -o app -Wall
