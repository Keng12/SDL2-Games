cmake_minimum_required(VERSION 3.5)

project(game-of-life LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-Ofast -Wall -Wextra -Wpedantic -Werror -fno-exceptions -Wshadow=global")
find_package(SDL2 REQUIRED)

add_executable(game-of-life src/game-of-life-main.cpp)
target_link_libraries(game-of-life SDL2) 
target_include_directories(game-of-life PRIVATE ${SDL2_INCLUDE_DIRS})
target_sources(game-of-life PRIVATE src/sdl2_util.cpp src/gol_util.hpp)

add_executable(snake src/snake-main.cpp)
target_link_libraries(snake SDL2) 
target_include_directories(snake PRIVATE ${SDL2_INCLUDE_DIRS})
target_sources(snake PRIVATE src/sdl2_util.cpp src/snake_util.cpp)
