all_win:
	g++ src/*.cpp -I./include -L./libs -lmingw32 -lSDL2_image -lSDL2main -lSDL2 -std=c++11 -o bin/app.exe -Wall

all_linux:
    g++ src/*.cpp -I./include -lSDL2_image -lSDL2 -std=c++11 -o bin/app -Wall