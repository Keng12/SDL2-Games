#include <iostream>
#include <array>
#include <chrono>
#include <thread>
#include <random>
#include <forward_list>
#include <future>

#include "SDL.h"
#include "sdl2_util/video.hpp"
#include "snake_util.hpp"
#include "game_util.hpp"
using namespace std::chrono_literals;

int main()
{
    constexpr int INIT_SNAKE_LENGTH = 3;
    size_t point_counter = 0;
    try
    {
        constexpr double FPS = 60;
        constexpr std::chrono::duration<double> TARGET_DELAY = std::chrono::duration<double>{1 / FPS};
        constexpr double defDeltaT = TARGET_DELAY.count();
        constexpr int WINDOW_HEIGHT = 720;
        constexpr int y = WINDOW_HEIGHT / 2;
        constexpr int SCALE_FACTOR = 100;
        constexpr int CELL_HEIGHT = WINDOW_HEIGHT / SCALE_FACTOR;
        constexpr int WINDOW_WIDTH = 1280;
        constexpr int x = WINDOW_WIDTH / 2;
        constexpr int CELL_WIDTH = CELL_HEIGHT;
        constexpr char INIT_DIRECTION = -1;
        constexpr int LENGTH_FACTOR = 3;
        const double SPEED = 8000;
        snake::Snake snake_instance = snake::Snake{x, y, CELL_WIDTH, CELL_HEIGHT, LENGTH_FACTOR, INIT_DIRECTION, WINDOW_WIDTH, WINDOW_HEIGHT, SPEED};
        SDL_Init(SDL_INIT_VIDEO); // Initialize SDL2
        sdl2_util::Window window{
            "Snake",                 // window title
            SDL_WINDOWPOS_UNDEFINED, // initial x position
            SDL_WINDOWPOS_UNDEFINED, // initial y position
            WINDOW_WIDTH,            // width, in pixels
            WINDOW_HEIGHT,           // height, in pixels
            SDL_WINDOW_RESIZABLE};   // Declare a pointer
        sdl2_util::Renderer renderer{window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED};
        renderer.renderClear("black"); // Clear to black screen
        SDL_Rect food{};
        food.w = CELL_WIDTH;
        food.h = CELL_HEIGHT;
        std::random_device rd;
        std::mt19937_64 mt(rd());
        std::uniform_int_distribution<> col_dist{0, WINDOW_WIDTH - 1 - CELL_WIDTH};
        std::uniform_int_distribution<> row_dist{0, WINDOW_HEIGHT - 1 - CELL_HEIGHT};
        // Set food randomly
        snake::setFood(food, mt, col_dist, row_dist, snake_instance);
        snake::drawFood(renderer, &food);
        std::cout << "X " << WINDOW_WIDTH - 1 - CELL_WIDTH << "Y " << WINDOW_HEIGHT - 1 - CELL_HEIGHT << std::endl;
        snake::drawSnake(renderer, snake_instance);
        renderer.present("black");
        SDL_Log("Finished init");
        bool quit{};
        const unsigned char *keystate = SDL_GetKeyboardState(nullptr);
        SDL_Event event{};
        std::chrono::duration<double> elapsed = TARGET_DELAY;
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
                SDL_FlushEvents(SDL_TEXTINPUT, SDL_MOUSEWHEEL);
                break;
            case SDL_KEYUP:
                SDL_FlushEvents(SDL_TEXTINPUT, SDL_MOUSEWHEEL);
                break;
            }
            char new_direction{};
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
            char hit_boundary = snake_instance.move(elapsed.count(), new_direction);
            snake::drawSnake(renderer, snake_instance);
            if (hit_boundary > 0)
            {
                if (hit_boundary == 1)
                {
                    std::cout << "Hit boundary" << std::endl;
                }
                else if (hit_boundary == 2)
                {
                    std::cout << "Hit self" << std::endl;
                }
                SDL_FlushEvents(SDL_TEXTINPUT, SDL_MOUSEWHEEL);
                SDL_WaitEvent(&event);
                switch (event.type)
                {
                case SDL_KEYUP:
                    quit = true;
                    break;
                }
            }
            bool hit_food = snake_instance.hasHitFood(&food);
            if (hit_food)
            {
                point_counter++;
                std::cout << "Hit food" << std::endl;
                snake::setFood(food, mt, col_dist, row_dist, snake_instance);
            }
            snake::drawFood(renderer, &food);
            renderer.present("black");
            auto end = std::chrono::steady_clock::now();
            elapsed = end - start;
            std::cout << elapsed.count() << std::endl;
            if (TARGET_DELAY > elapsed)
            {
                auto delay = TARGET_DELAY - elapsed;
                std::this_thread::sleep_for(delay);
            }
        }
    }

    catch (const std::exception &ex)
    {
        // Quit game
        std::cerr << ex.what() << std::endl;
    }
    // Clean up
    std::cout << "Quit, points: " << point_counter << std::endl;
    SDL_Quit();
    return 0;
}
