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
static std::array<std::array<uint_fast8_t, N_COLUMNS>, N_ROWS> cell_array{};
static std::random_device rd{};
static std::mt19937_64 mt(rd());
static std::uniform_int_distribution<> dist(0, 1);

static constexpr double FPS = 10.0;
static_assert(FPS > 0);
static constexpr std::chrono::duration<double> TARGET_DELAY = std::chrono::duration<double>{1 / FPS};
static std::chrono::time_point<std::chrono::steady_clock> start{};
static std::chrono::time_point<std::chrono::steady_clock> end{};
static std::chrono::duration<double> elapsed{};
static std::chrono::duration<double> delay{};

static SDL_Window *window{};
static SDL_Renderer *renderer{};
static SDL_Event event{};

static int result{};
static bool quit = false;

static uint_fast64_t mainRowLoop{};
static uint_fast64_t mainColLoop{};

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

    for (mainRowLoop = 0; mainRowLoop < N_ROWS; mainRowLoop++)
    {
        for (mainColLoop = 0; mainColLoop < N_COLUMNS; mainColLoop++)
        {
            result = dist(mt);
            if (1 == result)
            {
                cell_array.at(mainRowLoop).at(mainColLoop) = 1;
                sdl2_util::fillRect(renderer, &rect_array.at(mainRowLoop).at(mainColLoop)); // Fill rectangle with white color
            }
        }
    }
    sdl2_util::present(renderer, "black");
    while (!quit)
    {
        start = std::chrono::steady_clock::now();
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
        end = std::chrono::steady_clock::now();
        elapsed = end - start;
        if (TARGET_DELAY > elapsed)
        {
            delay = TARGET_DELAY - elapsed;
            std::this_thread::sleep_for(delay);
        }
    }
    // Clean up
    sdl2_util::quitSDL(window, renderer);
    return EXIT_SUCCESS;
}
