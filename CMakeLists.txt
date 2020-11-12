cmake_minimum_required(VERSION 3.17)

project(MarioBros-Ballestrinque)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB MBB_SRC
        "./src/*.cpp"
        "./include/.h"
        )
add_executable(MarioBros-Ballestrinque ${MBB_SRC})

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
include_directories(${SDL2_TTF_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_INCLUDE_DIRS} ./include/)

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES})