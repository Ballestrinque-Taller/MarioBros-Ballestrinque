set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}")

cmake_minimum_required(VERSION 3.16)

project(MarioBros-Ballestrinque)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB MBB_SRC
        "${CMAKE_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_SOURCE_DIR}/include/*.h"
        )
add_executable(${PROJECT_NAME} ${MBB_SRC})

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2TTF REQUIRED)
find_package(SDL2_mixer REQUIRED)

include_directories(${SDL2TTF_INCLUDE_DIR} ${SDL2_MIXER_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_INCLUDE_DIRS} ./include/)

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2TTF_LIBRARY} ${SDL2_MIXER_LIBRARY})

#add_subdirectory(Google_tests)