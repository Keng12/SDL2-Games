#include <iostream>
#include <array>
#include <chrono>
#include <thread>
#include <random>
#include "SDL.h"

#include "src/sdl2_util/video.hpp"
#include "src/gol_util.hpp"

int main()
{
    SDL_Init(SDL_INIT_VIDEO); // Initialize SDL2
    constexpr int WINDOW_HEIGHT = 1080;
    constexpr int N_ROWS = 540;
    constexpr int vertical_remainder = WINDOW_HEIGHT % N_ROWS;
    if (vertical_remainder != 0)
    {
        throw std::runtime_error{"Window height should be multiple of no. of rows"};
    }
    constexpr int CELL_HEIGHT = WINDOW_HEIGHT / N_ROWS;
    constexpr int WINDOW_WIDTH = 1920;
    constexpr int N_COLUMNS = 810;
    constexpr int horizontal_remainder = WINDOW_WIDTH % N_COLUMNS;
    if (horizontal_remainder != 0)
    {
        throw std::runtime_error{"Window width should be multiple of no. of columns"};
    }
    constexpr int CELL_WIDTH = WINDOW_WIDTH / N_COLUMNS;
    constexpr int N_CELLS = N_COLUMNS * N_ROWS;
    sdl2_util::Window window{
        "Game of Life",          // window title
        SDL_WINDOWPOS_UNDEFINED, // initial x position
        SDL_WINDOWPOS_UNDEFINED, // initial y position
        WINDOW_HEIGHT,           // width, in pixels
        WINDOW_WIDTH,            // height, in pixels
        SDL_WINDOW_RESIZABLE};   // Declare a pointer
    sdl2_util::Renderer renderer{window, -1, 0};
    SDL_Event event{};
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<> dist(0, 1);
    renderer.setDeadColor(); // Set color to black
    renderer.renderClear();  // Clear to black screen
    renderer.setLiveColor(); // Set color to white
    std::array<std::array<SDL_Rect, N_COLUMNS>, N_ROWS> rect_array{};
    std::array<std::array<char, N_COLUMNS>, N_ROWS> old_cell_state{};
    std::array<std::array<char, N_COLUMNS>, N_ROWS> new_cell_state{};
    for (std::size_t row = 0; row < N_ROWS; row++)
    {
        for (std::size_t col = 0; col < N_COLUMNS; col++)
        {
            rect_array.at(row).at(col).w = CELL_WIDTH;
            rect_array.at(row).at(col).h = CELL_HEIGHT;
            rect_array.at(row).at(col).x = row * CELL_WIDTH;
            rect_array.at(row).at(col).y = col * CELL_HEIGHT;
            int result = dist(mt);
            if (1 == result)
            {
                old_cell_state.at(row).at(col) = 1;
                renderer.fillRect(&rect_array.at(row).at(col)); // Fill rectangle with white color
            }
        }
    }
    renderer.present();
    SDL_Log("Finished init");
    bool quit = false;
    do
    {
        SDL_PollEvent(&event);
        std::cout << event.type << std::endl;
        switch (event.type)
        {
        case SDL_QUIT:
            std::cout << "break" << std::endl;
            quit = true;
        }
        gol_util::next_state<N_ROWS, N_COLUMNS>(old_cell_state, new_cell_state, renderer, rect_array);
       renderer.present();
       old_cell_state = std::move(new_cell_state);
       SDL_Delay(100);
    } while (!quit);
    // Clean up
    std::cout << "Quit" << std::endl;
    SDL_Quit();
    return 0;
}
