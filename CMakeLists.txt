cmake_minimum_required(VERSION 3.5)

project(game-of-life LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-Ofast -D_FORTIFY_SOURCE=2")
find_package(SDL2 REQUIRED)
find_package(TBB REQUIRED tbb)

add_executable(game-of-life src/game-of-life.cpp)
target_link_libraries(game-of-life SDL2 TBB::tbb) 
target_include_directories(game-of-life PRIVATE ${SDL2_INCLUDE_DIRS})
target_sources(game-of-life PRIVATE src/sdl2_util/video.cpp src/gol_util.hpp src/game_util.hpp)

add_executable(snake src/snake.cpp)
target_link_libraries(snake SDL2 TBB::tbb) 
target_include_directories(snake PRIVATE ${SDL2_INCLUDE_DIRS})
target_sources(snake PRIVATE src/sdl2_util/video.cpp src/snake_util.cpp src/game_util.hpp)
