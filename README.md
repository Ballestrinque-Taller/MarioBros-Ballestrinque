# MarioBros

## Contenidos
- [Compilación](#compilación)
  - [Compilacion en Windows](#compilación-en-windows)
  - [Compilacion en Linux](#compilación-en-linux)
- [Ejecución](#ejecución) <!-- no se, aca iria algo sobre como ejecutarlo tal vez? -->
- [Juego](#juego) <!-- instrucciones del juego ( controles, objetivo, etc) -->

## Compilación

Primero es necesario clonar el repositorio a un lugar en tu PC. Para eso se puede usar **git** con el comando: 
```
git clone https://github.com/Ballestrinque-Taller/MarioBros-Ballestrinque.git
```
o descargar el código de este repositorio en formato zip y extraerlo a alguna carpeta.  

### Compilación en Linux

*Tener instalados SDL2, SDL2_TTF, SDL2_mixer, y SDL2_image, cmake
*Comandos de instalacion:

  sudo apt-get install cmake  
  sudo apt-get install libsdl2-dev  
  sudo apt-get install libsdl2-image-dev  
  sudo apt-get install libsdl2-ttf-dev  
  sudo apt-get install libsdl2-mixer-dev  


*Compilar con:
  Parado en la carpeta base del proyecto -> cmake CMakeLists.txt
  
  Luego:
  `make` u opcionalmente con el flag `-j NUCLEOS` donde `NUCLEOS` es el numero de nucleos de la computadora (recomendado poner x1.5 o x2 veces el numero de nucleos físicos)  
  ej: `make -j 12`

*En el directorio principal y el ejecutable 'app' se genera en bin/

## Ejecución

*Para correr el cliente: ./MarioBros-Ballestrinque c IP PUERTO

*Para correr el servidor: ./MarioBros-Ballestrinque s IP PUERTO ./res/config.xml

U opcionalmente se puede poner otro path al xml


