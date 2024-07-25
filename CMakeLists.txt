cmake_minimum_required(VERSION 3.26)
project(TowerDefense)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lmingw32")
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

include_directories(headers)
include_directories(source)
include_directories(${PROJECT_SOURCE_DIR}/include)
link_directories(${PROJECT_SOURCE_DIR}/lib)

set(SOURCE_FILES main.cpp
        source/Game.cpp
        source/Level.cpp
        source/MathAddon.cpp
        source/Projectile.cpp
        source/TextureLoader.cpp
        source/Timer.cpp
        source/Turret.cpp
        source/Unit.cpp
        source/Vector2D.cpp
        source/TowerTypes.cpp
        source/UnitTypes.cpp
        headers/UnitTypes.h
)
add_executable(OpenGLTest ${SOURCE_FILES})

target_link_libraries(OpenGLTest mingw32 SDL2main SDL2_ttf SDL2)
