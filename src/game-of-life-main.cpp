#include <iostream>
#include <array>
#include <chrono>
#include <thread>
#include <random>
#include "SDL.h"

#include "sdl2_util/video.hpp"
#include "gol_util.hpp"

int main()
{
    try
    {
        constexpr int WINDOW_HEIGHT = 720;
        constexpr int WINDOW_WIDTH = 1280;
        static_assert(WINDOW_HEIGHT > 0 && WINDOW_WIDTH > 0);
        constexpr int N_ROWS = 72;
        constexpr int N_COLUMNS = 128;
        static_assert(N_ROWS > 0 && N_COLUMNS > 0);
        static_assert(WINDOW_HEIGHT % N_ROWS == 0 && WINDOW_WIDTH % N_COLUMNS == 0);
        constexpr int CELL_HEIGHT = WINDOW_HEIGHT / N_ROWS;
        constexpr int CELL_WIDTH = WINDOW_WIDTH / N_COLUMNS;
        constexpr std::array<std::array<SDL_Rect, N_COLUMNS>, N_ROWS> rect_array = gol::init_rect<N_ROWS, N_COLUMNS>(CELL_WIDTH, CELL_HEIGHT);

        constexpr double FPS = 15.0;
        static_assert(FPS > 0);
        constexpr std::chrono::duration<double> TARGET_DELAY = std::chrono::duration<double>{1 / FPS};

        SDL_Init(SDL_INIT_VIDEO); // Initialize SDL2

        sdl2_util::Window window{
            "Game of Life",          // window title
            SDL_WINDOWPOS_UNDEFINED, // initial x position
            SDL_WINDOWPOS_UNDEFINED, // initial y position
            WINDOW_WIDTH,            // width, in pixels
            WINDOW_HEIGHT,           // height, in pixels
            SDL_WINDOW_RESIZABLE};   // Declare a pointer

        sdl2_util::Renderer renderer{window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED};
        std::random_device rd{};
        std::mt19937_64 mt(rd());
        std::uniform_int_distribution<> dist(0, 1);
        renderer.renderClear("black"); // Clear to black screen
        renderer.setLiveColor();       // Set color to white
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
        renderer.present("black");
        SDL_Log("Finished init");
        SDL_Event event{};
        bool quit = false;
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
            cell_state = gol::next_state<N_ROWS, N_COLUMNS>(cell_state, renderer, rect_array);
            renderer.present("black");
            auto end = std::chrono::steady_clock::now();
            auto elapsed = end - start;
            if (TARGET_DELAY > elapsed)
            {
                auto delay = TARGET_DELAY - elapsed;
                std::this_thread::sleep_for(delay);
            }
        }
    }
    catch (std::exception &e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    // Clean up
    std::cout << "Quit" << std::endl;
    SDL_Quit();
    return EXIT_SUCCESS;
}
