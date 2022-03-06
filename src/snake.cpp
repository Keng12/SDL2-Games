#include <iostream>
#include <array>
#include <chrono>
#include <thread>
#include <random>
#include "SDL.h"

#include "sdl2_util/video.hpp"
#include "snake_util.hpp"
#include "game_util.hpp"

int main()
{
    try
    {
        SDL_Init(SDL_INIT_VIDEO); // Initialize SDL2
        constexpr int WINDOW_HEIGHT = 720;
        constexpr int N_ROWS = 72;
        constexpr int vertical_remainder = WINDOW_HEIGHT % N_ROWS;
        constexpr int CELL_HEIGHT = WINDOW_HEIGHT / N_ROWS;
        constexpr int WINDOW_WIDTH = 1280;
        constexpr int N_COLUMNS = 128;
        constexpr int horizontal_remainder = WINDOW_WIDTH % N_COLUMNS;
        constexpr int CELL_WIDTH = WINDOW_WIDTH / N_COLUMNS;
        constexpr int N_CELLS = N_COLUMNS * N_ROWS;
        constexpr int INIT_SNAKE_LENGTH = 3;
        constexpr std::array<std::array<SDL_Rect, N_COLUMNS>, N_ROWS> rect_array = game::init_rect<N_ROWS, N_COLUMNS>(CELL_WIDTH, CELL_HEIGHT);
        // Initialise snake position as cosntexpr array and convert to vector afterwards
        constexpr std::array<std::pair<int, int>, INIT_SNAKE_LENGTH> init_snake_position = snake::init_snake<INIT_SNAKE_LENGTH>(N_ROWS, N_COLUMNS);
        constexpr std::array<std::array<char, N_COLUMNS + 2>, N_ROWS + 2> init_board_state = snake::init_board<N_COLUMNS, N_ROWS, INIT_SNAKE_LENGTH>(init_snake_position);
        if (vertical_remainder != 0)
        {
            throw std::runtime_error{"Window height must be multiple of no. of rows"};
        }
        if (horizontal_remainder != 0)
        {
            throw std::runtime_error{"Window width must be multiple of no. of columns"};
        }
        std::array<std::array<char, N_COLUMNS + 2>, N_ROWS + 2> board_state = init_board_state;
        std::vector<std::pair<int, int>> snake_position{std::cbegin(init_snake_position), std::cend(init_snake_position)};
        std::pair<int, int> food_idx = snake::set_food<N_COLUMNS, N_ROWS>(snake_position);
        snake_position.reserve(INIT_SNAKE_LENGTH * 2);
        sdl2_util::Window window{
            "Snake",                 // window title
            SDL_WINDOWPOS_UNDEFINED, // initial x position
            SDL_WINDOWPOS_UNDEFINED, // initial y position
            WINDOW_WIDTH,            // width, in pixels
            WINDOW_HEIGHT,           // height, in pixels
            SDL_WINDOW_RESIZABLE};   // Declare a pointer
        sdl2_util::Renderer renderer{window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED};
        SDL_Log("Finished init");
        bool quit = false;
        SDL_Event event{};
        while (!quit)
        {
            SDL_PollEvent(&event);
            char direction{};
            switch (event.type)
            {
            case SDL_QUIT:
                quit = true;
                // Insert direction case
            }
            // Check hit and remove food
            // snake::set_snake(snake_position, )
            snake::set_food<N_COLUMNS, N_ROWS>(snake_position);
        }
    }
    catch (const std::exception &ex)
    {
        // Quit game
        std::cerr << ex.what() << std::endl;
    }
    // Clean up
    std::cout << "Quit" << std::endl;
    SDL_Quit();
    return 0;
}
