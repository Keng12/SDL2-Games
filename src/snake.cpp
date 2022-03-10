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
        constexpr int WINDOW_HEIGHT = 720;
        constexpr int N_ROWS = 72;
        constexpr int vertical_remainder = WINDOW_HEIGHT % N_ROWS;
        constexpr int CELL_HEIGHT = WINDOW_HEIGHT / N_ROWS;
        constexpr int WINDOW_WIDTH = 1280;
        constexpr int N_COLUMNS = 128;
        constexpr int horizontal_remainder = WINDOW_WIDTH % N_COLUMNS;
        constexpr int CELL_WIDTH = WINDOW_WIDTH / N_COLUMNS;
        constexpr std::array<std::array<SDL_Rect, N_COLUMNS>, N_ROWS> rect_array = game::init_rect<N_ROWS, N_COLUMNS>(CELL_WIDTH, CELL_HEIGHT);
        constexpr std::array<int, 2> init_snake_head{N_ROWS / 2, N_COLUMNS / 2};
        // Initialise snake position as cosntexpr array
        constexpr std::array<std::array<char, N_COLUMNS>, N_ROWS> init_board_state = snake::init_board<N_ROWS, N_COLUMNS>(INIT_SNAKE_LENGTH, init_snake_head);
        if (vertical_remainder != 0)
        {
            throw std::runtime_error{"Window height must be multiple of no. of rows"};
        }
        if (horizontal_remainder != 0)
        {
            throw std::runtime_error{"Window width must be multiple of no. of columns"};
        }

        std::array<std::array<char, N_COLUMNS>, N_ROWS> board_state = init_board_state;
        std::array<int, 2> snake_head = init_snake_head;
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
        std::random_device rd;
        std::mt19937_64 mt(rd());
        std::uniform_int_distribution<> col_dist{0, N_COLUMNS - 1};
        std::uniform_int_distribution<> row_dist{0, N_ROWS - 1};
        // Set food randomly
        std::array<int, 2> food_idx = snake::set_food<N_ROWS, N_COLUMNS>(board_state, mt, col_dist, row_dist);
        snake::draw_board(renderer, board_state, rect_array);
        renderer.present("black");
        SDL_Log("Finished init");
        std::array<int, 2> current_direction{0, 1};
        const unsigned char *keystate = SDL_GetKeyboardState(nullptr);
        bool quit{};
        SDL_Event event{};
        double speed = 1;
        int speed_0 = 1;
        while (!quit)
        {
            auto start = std::chrono::steady_clock::now();
            SDL_PollEvent(&event);
            // Render board
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
            std::array<int, 2> new_direction{};
            if (keystate[SDL_SCANCODE_LEFT])
            {
                new_direction = std::array<int, 2>{0, -1};
            }
            else if (keystate[SDL_SCANCODE_RIGHT])
            {
                new_direction = std::array<int, 2>{0, 1};
            }
            else if (keystate[SDL_SCANCODE_UP])
            {
                new_direction = std::array<int, 2>{-1, 0};
            }
            else if (keystate[SDL_SCANCODE_DOWN])
            {
                new_direction = std::array<int, 2>{1, 0};
            }
            std::array<int, 2> sum_direction = game::add_arrays<int, 2>(current_direction, new_direction);
            if ((game::sum_array(new_direction) == 0) || ((sum_direction[0] == 0) && (sum_direction[1] == 0)))
            {
                new_direction = current_direction;
            }
            snake_length += 1;
            char game_over = snake::update_snake<N_ROWS, N_COLUMNS>(snake_head, new_direction, board_state);
            if (game_over > 0)
            {
                switch (game_over)
                {
                case 1:
                    std::cout << "Snake hit border" << std::endl;
                    break;
                case 2:
                    std::cout << "Snake hit itself" << std::endl;
                    break;
                }
                while (!quit)
                {
                    SDL_FlushEvents(SDL_TEXTINPUT, SDL_MOUSEWHEEL);
                    SDL_WaitEvent(&event);
                    // Render board
                    switch (event.type)
                    {
                    case SDL_KEYDOWN:
                        quit = true;
                        break;
                    }
                }
            }
            current_direction = std::move(new_direction);
            bool hit_food = game::check_equality_arrays(snake_head, food_idx);
            if (hit_food)
            {
                point_counter++;
                food_idx = snake::set_food<N_ROWS, N_COLUMNS>(board_state, mt, col_dist, row_dist);
            }
            snake::draw_board(renderer, board_state, rect_array);
            renderer.present("black");
            auto end = std::chrono::steady_clock::now();
            std::chrono::duration<double> elapsed = end - start;
            double speed{};
            if (elapsed < TARGET_DELAY)
            {
                auto delay = TARGET_DELAY - elapsed;
                std::this_thread::sleep_for(delay);
                speed = TARGET_DELAY.count();
            }
            else
            {
                speed = elapsed.count();
            }
            std::cout << "FPS: " << speed << std::endl;
        }
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
