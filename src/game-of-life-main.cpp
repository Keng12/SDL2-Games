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
        "Game of Life",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_RESIZABLE);
    renderer = sdl2_util::createRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    sdl2_util::renderClear(renderer, "black"); // Clear to black screen
    sdl2_util::setLiveColor(renderer);         // Set color to white

    std::random_device rd{};
    std::mt19937_64 mt(rd());
    std::uniform_int_distribution<> dist(0, 1);

    std::array<std::array<uint_fast8_t, N_COLUMNS>, N_ROWS> cell_array{};
    for (size_t row = 0; row < N_ROWS; row++)
    {
        for (size_t col = 0; col < N_COLUMNS; col++)
        {
            int_fast8_t result = dist(mt);
            if (1 == result)
            {
                cell_array.at(row).at(col) = 1;
                sdl2_util::fillRect(renderer, &rect_array.at(row).at(col)); // Fill rectangle with white color
            }
        }
    }
    sdl2_util::present(renderer, "black");
    SDL_Event event{};
    bool quit = false;

    while (!quit)
    {
        std::chrono::time_point<std::chrono::steady_clock> start = std::chrono::steady_clock::now();
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
        cell_array = gol::next_state(cell_array, renderer, rect_array);
        sdl2_util::present(renderer, "black");
        std::chrono::time_point<std::chrono::steady_clock> end = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        if (TARGET_DELAY > elapsed)
        {
            static std::chrono::duration<double> delay = TARGET_DELAY - elapsed;
            std::this_thread::sleep_for(delay);
        }
    }
    // Clean up
    sdl2_util::quitSDL(window, renderer);
    return EXIT_SUCCESS;
}
