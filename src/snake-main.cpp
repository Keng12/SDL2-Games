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

static SDL_Window *window{};
static SDL_Renderer *renderer{};

void terminateHandler()
{
    sdl2_util::quitSDL(window, renderer);
#ifdef __GNUC__
    __gnu_cxx::__verbose_terminate_handler();
#endif
}

int main()
{
    std::set_terminate(terminateHandler);
    sdl2_util::initSDL(SDL_INIT_VIDEO); // Initialize SDL2
    window = sdl2_util::createWindow(
        "Snake",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_RESIZABLE);
    renderer = sdl2_util::createRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    sdl2_util::renderClear(renderer, "black"); // Clear to black screen

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
    snake::drawSnake(renderer, snake_instance);
    snake::drawFood(renderer, &food);
    sdl2_util::present(renderer, "black");
    // Prepare main loop
    bool quit{};
    const unsigned char *keystate = SDL_GetKeyboardState(nullptr);
    SDL_Event event{};
    std::chrono::duration<double> elapsed = TARGET_DELAY;
    // Main game loop
    while (!quit)
    {
        auto start = std::chrono::steady_clock::now();
        SDL_PollEvent(&event);
        switch (event.type)
        {
        case SDL_QUIT:
            quit = true;
            break;
        case SDL_KEYDOWN:
        case SDL_KEYUP:
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
            SDL_FlushEvents(SDL_TEXTINPUT, SDL_MOUSEWHEEL);
            break;
        }
        int new_direction{};
        if (keystate[SDL_SCANCODE_LEFT])
        {
            new_direction = -1;
        }
        else if (keystate[SDL_SCANCODE_RIGHT])
        {
            new_direction = 1;
        }
        else if (keystate[SDL_SCANCODE_UP])
        {
            new_direction = -2;
        }
        else if (keystate[SDL_SCANCODE_DOWN])
        {
            new_direction = 2;
        }
        uint_fast8_t game_over = snake_instance.move(elapsed.count(), new_direction);
        snake::drawSnake(renderer, snake_instance);
        if (game_over > 0)
        {
            snake::drawFood(renderer, &food);
            sdl2_util::present(renderer, "black");
            if (1 == game_over)
            {
                std::cout << "Hit boundary" << std::endl;
            }
            else if (2 == game_over)
            {
                std::cout << "Hit self" << std::endl;
            }
            SDL_FlushEvents(SDL_TEXTINPUT, SDL_MOUSEWHEEL);
            while (!quit)
            {
                SDL_WaitEvent(&event);
                switch (event.type)
                {
                case SDL_KEYUP:
                    quit = true;
                    break;
                }
            }
        }
        bool hit_food = snake_instance.hasHitFood(&food);
        if (hit_food)
        {
            point_counter++;
            snake::setFood(food, mt, col_dist, row_dist, snake_instance);
        }
        snake::drawFood(renderer, &food);
        sdl2_util::present(renderer, "black");
        auto end = std::chrono::steady_clock::now();
        elapsed = end - start;
        if (TARGET_DELAY > elapsed)
        {
            auto delay = TARGET_DELAY - elapsed;
            std::this_thread::sleep_for(delay);
        }
    }
    // Clean up
    sdl2_util::quitSDL(window, renderer);
    std::cout << "Quit, points: " << point_counter << std::endl;
    SDL_Quit();
    return EXIT_SUCCESS;
}
