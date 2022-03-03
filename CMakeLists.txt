cmake_minimum_required(VERSION 3.5)

project(game-of-life LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(SDL2 REQUIRED)

add_executable(game-of-life main.cpp)
target_link_libraries(game-of-life ${SDL2_LIBRARIES})
target_include_directories(game-of-life PRIVATE ${SDL2_INCLUDE_DIRS})
