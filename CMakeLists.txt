#sudo apt install libsdl2-image-dev
#git clone https://github.com/assimp/assimp.git
#cd assimp
#cmake ./
#make
#sudo make install

cmake_minimum_required(VERSION 3.5.1)
project(maze-adventure)
set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_LIBRARIES "-lSDL2 -lSDL2main -lSDL2_image -lSDL2_mixer -lGL -ldl -I/usr/include/SDL2/")

include_directories(lib/glad)
add_subdirectory(lib/glm)

find_package(assimp REQUIRED)

file(GLOB_RECURSE SRC
        Src/*.*
        lib/*.h
        lib/glad/*.c
        )


add_executable(maze-adventure ${SRC})
target_link_libraries(${PROJECT_NAME} SDL2main SDL2 assimp)
