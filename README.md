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

### Compilación en Windows

Descargar e instalar el entorno [MSYS](https://www.msys2.org/). Seguir las instrucciones de la página web para configurar el entorno y actualizar los repositorios.

Todavía en el entorno de MSYS, ejecutar el siguiente comando para obtener las dependencias:  
```
pacman -S make mingw-w64-x86_64-gcc mingw-w64-x86_64-SDL2 mingw-w64-x86_64-SDL2_image
```  
Responder a `:: Proceed with installation? [Y/n]` con `y` para instalarlos.

Ahora, dentro se msys, cambiar el directorio de trabajo a ese (o sea, `cd /path_al_projecto-MarioBros-Ballestrinque/`). Este paso es importante

asegurandose que `pwd` devuelve el directorio del projecto, ejecutar  
```
./windows-obtener_dll.sh
```
para descargar los archivos _.dll_ necesarios. Este paso solo debe hacerse una vez; no hay que volverlo a ejecutar cada vez que se quiera compilar.

Una vez terminado con esto, cerrar la ventana para luego abrir un entorno de MSYS MinGW64 (MSYS MinGW 64-bit).  

Cambiar el directorio de trabajo al directorio del projecto y ejecutar:  
`make all_win`  

Luego de que termine de ejecutarse, el ejecutable se encuentra en el directorio del projecto bin/app.exe 

### Compilación en Linux

*Tener instalados SDL2 y SDL2_image
*Compilar con:
  '''make all_linux'''

*En el directorio principal y el ejecutable 'app' se genera en bin/

## Ejecución

ejecutar desde una terminal con el derectorio de trabajo en el projecto  
`bin/app` o `bin/app.exe`


