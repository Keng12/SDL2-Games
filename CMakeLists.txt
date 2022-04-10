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

find_program(CMAKE_CXX_CPPCHECK NAMES cppcheck)
if (CMAKE_CXX_CPPCHECK)
    message(STATUS "Enable cppcheck")
    list(APPEND CMAKE_CXX_CPPCHECK 
            "--enable=all"
            "--error-exitcode=1" 
            "--inconclusive"
            "--force" 
            "--inline-suppr"
            "--std=c++17"
            "--suppressions-list=${CMAKE_SOURCE_DIR}/CppCheckSuppressions.txt"
    )
endif()

find_program(CMAKE_CXX_INCLUDE_WHAT_YOU_USE NAMES iwyu)
if (CMAKE_CXX_INCLUDE_WHAT_YOU_USE)
    list(APPEND CMAKE_CXX_INCLUDE_WHAT_YOU_USE
    "--cxx17ns"
    )
    message(STATUS "Enable iwyu")
endif()

option(LINK_WHAT_YOU_USE "Enable lwyu" ON)
message(STATUS "Enable lwyu")
