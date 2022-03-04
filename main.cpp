#include <iostream>
#include <array>
#include <chrono>
#include <thread>
#include <random>
#include <SDL2/SDL.h>

#include "src/sdl2_util/video.hpp"
#include "src/gol_util.hpp"

int main()
{
    SDL_Init(SDL_INIT_VIDEO); // Initialize SDL2
    const int WINDOW_HEIGHT = 500;
    const int CELL_HEIGHT = 10;
    constexpr int vertical_remainder = WINDOW_HEIGHT % CELL_HEIGHT;
    if (vertical_remainder != 0)
    {
        throw std::runtime_error{"Window height should be multiple of no. of rows"};
    }
    constexpr int N_ROWS = WINDOW_HEIGHT / CELL_HEIGHT;
    const int WINDOW_WIDTH = 500;
    const int CELL_WIDTH = 10;
    constexpr int horizontal_remainder = WINDOW_WIDTH % CELL_WIDTH;
    if (horizontal_remainder != 0)
    {
        throw std::runtime_error{"Window width should be multiple of no. of columns"};
    }
    constexpr int N_COLUMNS = WINDOW_HEIGHT / CELL_WIDTH;
    constexpr int N_CELLS = N_COLUMNS * N_ROWS;
    sdl2_util::Window window{
        "Game of Life",          // window title
        SDL_WINDOWPOS_UNDEFINED, // initial x position
        SDL_WINDOWPOS_UNDEFINED, // initial y position
        WINDOW_HEIGHT,           // width, in pixels
        WINDOW_WIDTH,            // height, in pixels
        SDL_WINDOW_RESIZABLE};   // Declare a pointer
    sdl2_util::Renderer renderer{window, -1, 0};
    sdl2_util::Texture texture{renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, WINDOW_HEIGHT, WINDOW_WIDTH};
    SDL_Event event{};
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<> dist(0, 1);
    renderer.setRenderTarget(texture);
    renderer.setRenderDrawColor("black"); // Set color to black
    renderer.renderClear();               // Clear to black screen
    renderer.setRenderDrawColor("white"); // Set color to white
    std::array<std::array<SDL_Rect, N_COLUMNS>, N_ROWS> rect_array{};
    std::array<std::array<char, N_COLUMNS + 2>, N_ROWS + 2> old_cell_state{};
    std::array<std::array<char, N_COLUMNS + 2>, N_ROWS + 2> new_cell_state{};
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
                old_cell_state.at(row + 1).at(col + 1) = 1;
                renderer.renderFillRect(&rect_array.at(row).at(col)); // Fill rectangle with white color
            }
        }
    }
    renderer.presentTexture(texture);
    SDL_Log("Finished init");
    while (SDL_PollEvent(&event))
    {
        std::cout << event.type << std::endl;
        switch (event.type)
        {
        case SDL_QUIT:
            break;
        }
        renderer.setRenderTarget(texture);
        gol_util::next_state<N_COLUMNS, N_ROWS>(old_cell_state, new_cell_state, renderer, rect_array);
        renderer.presentTexture(texture);
        old_cell_state = std::move(new_cell_state);
        SDL_Delay(2000);
    }
    // Clean up
    SDL_Quit();
    return 0;
}
