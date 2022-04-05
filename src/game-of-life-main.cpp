#include <iostream>
#include <array>
#include <chrono>
#include <thread>
#include <random>
#include "SDL.h"

#include "sdl2_util/video.hpp"
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
static std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> window{nullptr, SDL_DestroyWindow};
void terminateHandler()
{
    if (window){
        SDL_DestroyWindow(window.get());
    }
}

int main()
{
    SDL_Init(SDL_INIT_VIDEO); // Initialize SDL2
    window{
        "Game of Life",          // window title
        SDL_WINDOWPOS_UNDEFINED, // initial x position
        SDL_WINDOWPOS_UNDEFINED, // initial y position
        WINDOW_WIDTH,            // width, in pixels
        WINDOW_HEIGHT,           // height, in pixels
        SDL_WINDOW_RESIZABLE};   // Declare a pointer
    sdl2_util::Renderer renderer{window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED};
    renderer.renderClear("black"); // Clear to black screen
    renderer.setLiveColor();       // Set color to white
    std::random_device rd{};
    std::mt19937_64 mt(rd());
    std::uniform_int_distribution<> dist(0, 1);
    std::array<std::array<int, N_COLUMNS>, N_ROWS> cell_state{};

    for (std::size_t row = 0; row < N_ROWS; row++)
    {
        for (std::size_t col = 0; col < N_COLUMNS; col++)
        {
            int result = dist(mt);
            if (1 == result)
            {
                cell_state.at(row).at(col) = 1;
                renderer.fillRect(&rect_array.at(row).at(col)); // Fill rectangle with white color
            }
        }
    }
    SDL_Event event{};
    renderer.present("black");
    bool quit = false;
    while (!quit)
    {
        static auto start = std::chrono::steady_clock::now();
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
        cell_state = gol::next_state<N_ROWS, N_COLUMNS>(cell_state, renderer, rect_array);
        renderer.present("black");
        static auto end = std::chrono::steady_clock::now();
        static auto elapsed = end - start;
        if (TARGET_DELAY > elapsed)
        {
            static auto delay = TARGET_DELAY - elapsed;
            std::this_thread::sleep_for(delay);
        }
    }
    // Clean up
    SDL_Quit();
    return EXIT_SUCCESS;
}
