project(Google_tests)
add_subdirectory(lib)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

file(GLOB TEST_SRC "*.cpp")
file(GLOB MARIO_SRC
        "../src/Background.cpp" "../src/Camara.cpp" "../src/Cliente.cpp" "../src/Dibujador.cpp" "../src/Enemigo.cpp" "../src/Escenario.cpp"
        "../src/Goomba.cpp" "../src/Jugador.cpp" "../src/Ladrillo.cpp" "../src/LectorXML.cpp" "../src/Log.cpp" "../src/Logger.cpp"
        "../src/Moneda.cpp" "../src/Renderer.cpp" "../src/Servidor.cpp" "../src/Sorpresa.cpp" "../src/Temporizador.cpp"
        "../src/TextWriter.cpp" "../src/Tortuga.cpp" "../src/Colisionador.cpp" "../src/ReproductorDeSonido.cpp" "../src/Hongo.cpp"
        "../include/*.h"
        )
add_executable(Google_Tests_run ${TEST_SRC} ${MARIO_SRC})

# linking Google_Tests_run with lib to be tested

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2TTF REQUIRED)
find_package(SDL2_mixer REQUIRED)

include_directories(${SDL2TTF_INCLUDE_DIR} ${SDL2_MIXER_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_INCLUDE_DIRS} ./include/)

target_link_libraries(Google_Tests_run ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2TTF_LIBRARY} ${SDL2_MIXER_LIBRARY})

target_link_libraries(Google_Tests_run gtest gtest_main)