/*
Copyright (C) 2022 Keng Yip Chai

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <iostream>
#include <array>
#include <chrono>
#include <thread>
#include <random>
#include <algorithm>
#include <execution>
#include "SDL.h"

#include "sdl2_util.hpp"
#include "gol_util.hpp"

static constexpr int WINDOW_HEIGHT = 720;
static constexpr int WINDOW_WIDTH = 1280;
static_assert(WINDOW_HEIGHT > 0 && WINDOW_WIDTH > 0);

static constexpr int N_ROWS = 72;
static constexpr int N_COLUMNS = 128;
static_assert(N_ROWS > 0 && N_COLUMNS > 0);

static_assert(WINDOW_HEIGHT % N_ROWS == 0 && WINDOW_WIDTH % N_COLUMNS == 0);
static constexpr int CELL_HEIGHT = WINDOW_HEIGHT / N_ROWS;
static constexpr int CELL_WIDTH = WINDOW_WIDTH / N_COLUMNS;

static constexpr std::array<std::array<SDL_Rect, N_COLUMNS>, N_ROWS> rect_array = gol::init_rect<N_ROWS, N_COLUMNS>(CELL_WIDTH, CELL_HEIGHT);
static constexpr double FPS = 10.0;
static_assert(FPS > 0);
static constexpr std::chrono::duration<double> TARGET_DELAY = std::chrono::duration<double>{1 / FPS};

void terminateHandler()
{
    sdl2_util::quitSDL();
#ifdef __GNUC__
    __gnu_cxx::__verbose_terminate_handler();
#endif
}

bool handle_event(SDL_Event& event){
    bool quit{};
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            quit = true;
            break;
        }
    }
    return quit;
}

int main()
{
    std::set_terminate(terminateHandler);
    sdl2_util::initSDL(SDL_INIT_VIDEO); // Initialize SDL2
    sdl2_util::createWindow(
        "Game of Life",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        0);
    sdl2_util::createRenderer(-1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    sdl2_util::renderClear(sdl2_util::RenderColor::black); // Clear to black screen
    sdl2_util::setRenderDrawColor(sdl2_util::RenderColor::white);       // Set color to white

    std::random_device rd{};
    std::mt19937_64 mt(rd());
    std::uniform_int_distribution<uint_fast8_t> dist(0, 1);

    std::array<std::array<uint_fast8_t, N_COLUMNS>, N_ROWS> cell_array{};
    std::array<size_t, N_COLUMNS> col_iterator{};
    std::iota(col_iterator.begin(), col_iterator.end(), 0);
    std::array<size_t, N_ROWS> row_iterator{};
    std::iota(row_iterator.begin(), row_iterator.end(), 0);
    std::for_each(std::execution::unseq, row_iterator.cbegin(), row_iterator.cend(), [&](const size_t row){
        std::for_each(std::execution::unseq, col_iterator.cbegin(), col_iterator.cend(),
        [&](const size_t col){
                        uint_fast8_t result = dist(mt);
            if (1 == result)
            {
                cell_array.at(row).at(col) = true;
                sdl2_util::fillRect(&rect_array.at(row).at(col)); // Fill rectangle with white color
            }
        }
        );
    }
    );  
    sdl2_util::present(sdl2_util::RenderColor::black);
    SDL_Event event{};
    bool quit = false;
    while (!quit)
    {
        std::chrono::time_point<std::chrono::steady_clock> start = std::chrono::steady_clock::now();
        quit = handle_event(event);
        cell_array = gol::next_state(cell_array, rect_array, row_iterator, col_iterator);
        sdl2_util::present(sdl2_util::RenderColor::black);
        std::chrono::time_point<std::chrono::steady_clock> end = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        if (TARGET_DELAY > elapsed)
        {
            static std::chrono::duration<double> delay = TARGET_DELAY - elapsed;
            std::this_thread::sleep_for(delay);
        }
    }
    // Clean up
    sdl2_util::quitSDL();
    return EXIT_SUCCESS;
}
