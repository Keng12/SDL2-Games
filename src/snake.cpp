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
    size_t snake_length = INIT_SNAKE_LENGTH;
    try
    {
        constexpr double FPS = 60;
        constexpr std::chrono::duration<double> TARGET_DELAY = std::chrono::duration<double>{1 / FPS};
        constexpr double defDeltaT = TARGET_DELAY.count(); 
        constexpr int WINDOW_HEIGHT = 720;
        constexpr int y = WINDOW_HEIGHT / 2;
        constexpr int CELL_HEIGHT = 72;
        constexpr int WINDOW_WIDTH = 1280;
        constexpr int x = WINDOW_WIDTH /2;
        constexpr int CELL_WIDTH = 128;
        constexpr char INIT_DIRECTION = -1;
        constexpr int LENGTH_FACTOR = 3;
        snake::Snake snake = snake::Snake{x, y, CELL_WIDTH, CELL_WIDTH, LENGTH_FACTOR, INIT_DIRECTION, WINDOW_WIDTH, WINDOW_HEIGHT};
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
        snake::setFood(food, mt, col_dist, row_dist, snake);
        SDL_Log("Finished init");
    }
    catch (const std::exception &ex)
    {
        // Quit game
        std::cerr << ex.what() << std::endl;
    }
    // Clean up
    std::cout << "Quit, points: " << point_counter << ", snake length: " << snake_length << std::endl;
    SDL_Quit();
    return 0;
}
