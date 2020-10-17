#! /bin/bash
function proceder {
    descargar
    extraer
    limpiar
    exit
}

function descargar {
    echo "descargando archivos .dll"
    wget https://www.libsdl.org/projects/SDL_image/release/SDL2_image-2.0.5-win32-x64.zip
    wget https://www.libsdl.org/release/SDL2-2.0.12-win32-x64.zip
}
function extraer {
    echo "extrayendo archivos a $(pwd)/bin/"
    tar -xzf SDL2_image-2.0.5-win32-x64.zip -C bin/ *.dll
    tar -xzf SDL2-2.0.12-win32-x64.zip -C bin/ *.dll
}

function limpiar {
    echo "borrando archivos .zip ya no necesitados de SDL2"
    rm SDL2_image-2.0.5-win32-x64.zip SDL2-2.0.12-win32-x64.zip
}

DIR_PROJECTO='/MarioBros-Ballestrinque'
if [[ "$PWD" =~ "$DIR_PROJECTO"$ ]]
then
    proceder
else
    echo "no estas en el directorio del projecto"
fi