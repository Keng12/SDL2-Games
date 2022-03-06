#include <iostream>
#include <array>
#include <chrono>
#include <thread>
#include <random>
#include "SDL.h"

#include "sdl2_util/video.hpp"
#include "gol_util.hpp"

int main()
{
    SDL_Init(SDL_INIT_VIDEO); // Initialize SDL2
    constexpr int WINDOW_HEIGHT = 720;
    constexpr int N_ROWS = 72;
    constexpr int vertical_remainder = WINDOW_HEIGHT % N_ROWS;
    if (vertical_remainder != 0)
    {
        throw std::runtime_error{"Window height should be multiple of no. of rows"};
    }
    constexpr int CELL_HEIGHT = WINDOW_HEIGHT / N_ROWS;
    constexpr int WINDOW_WIDTH = 1280;
    constexpr int N_COLUMNS = 128;
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
        WINDOW_WIDTH,            // width, in pixels
        WINDOW_HEIGHT,           // height, in pixels
        SDL_WINDOW_RESIZABLE};   // Declare a pointer
    sdl2_util::Renderer renderer{window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED};
    SDL_Event event{};
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<> dist(0, 1);
    renderer.setDeadColor(); // Set color to black
    renderer.renderClear();  // Clear to black screen
    renderer.setLiveColor(); // Set color to white
    std::array<std::array<SDL_Rect, N_COLUMNS>, N_ROWS> rect_array = gol_util::init_array<N_ROWS, N_COLUMNS>(CELL_WIDTH, CELL_HEIGHT);
    std::array<std::array<char, N_COLUMNS>, N_ROWS> old_cell_state{};
    for (std::size_t row = 0; row < N_ROWS; row++)
    {
        for (std::size_t col = 0; col < N_COLUMNS; col++)
        {
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
        unsigned long long start = SDL_GetPerformanceCounter();
        std::array<std::array<char, N_COLUMNS>, N_ROWS> new_cell_state{};
        SDL_PollEvent(&event);
        switch (event.type)
        {
        case SDL_QUIT:
            quit = true;
        }
        gol_util::next_state<N_ROWS, N_COLUMNS>(old_cell_state, new_cell_state, renderer, rect_array);
        renderer.present();
        old_cell_state = std::move(new_cell_state);
        SDL_Delay(100);
        unsigned long long end = SDL_GetPerformanceCounter();
	    float elapsed = (end - start) / (float)SDL_GetPerformanceFrequency();
	    std::cout << "Current FPS: " << std::to_string(1.0 / elapsed) << std::endl;
    } while (!quit);
    // Clean up
    std::cout << "Quit" << std::endl;
    SDL_Quit();
    return 0;
}
