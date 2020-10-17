dir_path = $(dir $(realpath $(firstword $(MAKEFILE_LIST))))

default:
	@echo "usar 'make all_win' para compilar en windows y 'make all_linux' para Linux"

all_win:
	g++ $(dir_path)src/*.cpp -I$(dir_path)include -lmingw32 -lSDL2_image -lSDL2main -lSDL2 -std=c++11 -o $(dir_path)bin/app.exe -Wall
	
all_linux:
	g++ $(dir_path)src/*.cpp -I$(dir_path)include -lSDL2_image -lSDL2 -std=c++11 -o $(dir_path)bin/app.out -Wall
