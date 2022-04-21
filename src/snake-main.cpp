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
#include <getopt.h>
#include "SDL.h"

#include "sdl2_util.hpp"
#include "snake_util.hpp"

static constexpr int WINDOW_WIDTH = 500;
static constexpr int WINDOW_HEIGHT = WINDOW_WIDTH;
static_assert(WINDOW_WIDTH > 0 && WINDOW_HEIGHT > 0);

static constexpr int_least8_t SCALE_FACTOR = 25;
static_assert(WINDOW_WIDTH % SCALE_FACTOR == 0 && WINDOW_HEIGHT % SCALE_FACTOR == 0);
static constexpr int CELL_LENGTH = WINDOW_HEIGHT / SCALE_FACTOR;

static constexpr double FPS = 60.0;
static_assert(FPS > 0);
static constexpr std::chrono::duration<double> TARGET_DELAY = std::chrono::duration<double>{1 / FPS};

static constexpr int INIT_DIRECTION = -1;
static_assert(std::abs(INIT_DIRECTION) <= 2 && std::abs(INIT_DIRECTION) >= 0);
static constexpr double SPEED_FACTOR = 15000;
static_assert(SPEED_FACTOR > 0);

void terminateHandler()
{
    sdl2_util::quitSDL();
#ifdef __GNUC__
    __gnu_cxx::__verbose_terminate_handler();
#endif
}

std::tuple<bool, int> handle_event(SDL_Event &event)
{
    bool quit{};
    int new_direction{};
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            quit = true;
        }
        else if (event.type == SDL_KEYDOWN)
        { 
            switch( event.key.keysym.sym ){
                case SDLK_LEFT:
                                new_direction = -1;

                break;
                                case SDLK_RIGHT:
                                                new_direction = 1;

                break;
                case SDLK_UP:
                                new_direction = -2;

                break;
                case SDLK_DOWN:
                                new_direction = 2;

                break;

            }
        }
    }
    return {quit, new_direction};
}

int main()
{
    std::set_terminate(terminateHandler);
    sdl2_util::initSDL(SDL_INIT_VIDEO); // Initialize SDL2
    sdl2_util::createWindow(
        "Snake",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        0);
    sdl2_util::createRenderer(-1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    sdl2_util::renderClear("black"); // Clear to black screen

    uint_fast64_t point_counter = 0;
    // Prepare SDL2
    // Initialize snake
    snake::Snake snake_instance = snake::Snake{CELL_LENGTH, INIT_DIRECTION, WINDOW_WIDTH, WINDOW_HEIGHT, SPEED_FACTOR};
    // Create random distributions
    std::random_device rd{};
    std::mt19937_64 mt(rd());
    std::uniform_int_distribution<> col_dist{0, WINDOW_WIDTH - 1 - CELL_LENGTH};
    std::uniform_int_distribution<> row_dist{0, WINDOW_HEIGHT - 1 - CELL_LENGTH};
    // Set food randomly
    SDL_Rect food = sdl2_util::initRect(0, 0, CELL_LENGTH, CELL_LENGTH);
    snake::setFood(food, mt, col_dist, row_dist, snake_instance);
    // Render objects
    snake::drawSnake(snake_instance);
    snake::drawFood(&food);
    sdl2_util::present("black");
    // Prepare main loop
    bool quit{};
  //  const unsigned char *keystate = SDL_GetKeyboardState(nullptr);
    SDL_Event event{};
    std::chrono::duration<double> elapsed = TARGET_DELAY;
    int new_direction{};

    // Main game loop
    while (!quit)
    {

        auto start = std::chrono::steady_clock::now();
        std::tie(quit, new_direction) = handle_event(event);
        uint_fast8_t game_over = snake_instance.move(elapsed.count(), new_direction);
        snake::drawSnake(snake_instance);
        if (game_over > 0)
        {
            snake::drawFood(&food);
            sdl2_util::present("black");
            if (1 == game_over)
            {
                std::cout << "Hit boundary" << std::endl;
            }
            else if (2 == game_over)
            {
                std::cout << "Hit self" << std::endl;
            }
            SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
            while (!quit)
            {
                SDL_WaitEvent(&event);
                if (event.type == SDL_QUIT || event.type == SDL_KEYUP)
                {
                    quit = true;
                }
            }
        }
        bool hit_food = snake_instance.hasHitFood(&food);
        if (hit_food)
        {
            point_counter++;
            snake::setFood(food, mt, col_dist, row_dist, snake_instance);
        }
        snake::drawFood(&food);
        sdl2_util::present("black");
        auto end = std::chrono::steady_clock::now();
        elapsed = end - start;
        if (TARGET_DELAY > elapsed)
        {
            auto delay = TARGET_DELAY - elapsed;
            std::this_thread::sleep_for(delay);
        }
    }
    // Clean up
    sdl2_util::quitSDL();
    std::cout << "Quit, points: " << point_counter << std::endl;
    return EXIT_SUCCESS;
}
