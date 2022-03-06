#include <iostream>
#include <array>
#include <chrono>
#include <thread>
#include <random>
#include "SDL.h"

#include "sdl2_util/video.hpp"
#include "snake_util.hpp"
#include "game_util.hpp"
int main()
{
    constexpr int INIT_SNAKE_LENGTH = 3;
    size_t point_counter = 0;
    size_t snake_length = INIT_SNAKE_LENGTH;
    try
    {
        constexpr int WINDOW_HEIGHT = 720;
        constexpr int N_ROWS = 72;
        constexpr int vertical_remainder = WINDOW_HEIGHT % N_ROWS;
        constexpr int CELL_HEIGHT = WINDOW_HEIGHT / N_ROWS;
        constexpr int WINDOW_WIDTH = 1280;
        constexpr int N_COLUMNS = 128;
        constexpr int horizontal_remainder = WINDOW_WIDTH % N_COLUMNS;
        constexpr int CELL_WIDTH = WINDOW_WIDTH / N_COLUMNS;
        constexpr int N_CELLS = N_COLUMNS * N_ROWS;
        constexpr std::array<std::array<SDL_Rect, N_COLUMNS>, N_ROWS> rect_array = game::init_rect<N_ROWS, N_COLUMNS>(CELL_WIDTH, CELL_HEIGHT);
        // Initialise snake position as cosntexpr array
        constexpr std::array<std::pair<int, int>, INIT_SNAKE_LENGTH> init_snake_position = snake::init_snake<INIT_SNAKE_LENGTH>(N_ROWS, N_COLUMNS);
        constexpr std::array<std::array<char, N_COLUMNS>, N_ROWS> init_board_state = snake::init_board<N_ROWS, N_COLUMNS, INIT_SNAKE_LENGTH>(init_snake_position);
        if (vertical_remainder != 0)
        {
            throw std::runtime_error{"Window height must be multiple of no. of rows"};
        }
        if (horizontal_remainder != 0)
        {
            throw std::runtime_error{"Window width must be multiple of no. of columns"};
        }

        std::array<std::array<char, N_COLUMNS>, N_ROWS> board_state = init_board_state;
        std::pair<int, int> snake_head = init_snake_position.at(0);
        SDL_Init(SDL_INIT_VIDEO); // Initialize SDL2
        sdl2_util::Window window{
            "Snake",                 // window title
            SDL_WINDOWPOS_UNDEFINED, // initial x position
            SDL_WINDOWPOS_UNDEFINED, // initial y position
            WINDOW_WIDTH,            // width, in pixels
            WINDOW_HEIGHT,           // height, in pixels
            SDL_WINDOW_RESIZABLE};   // Declare a pointer
        sdl2_util::Renderer renderer{window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED};
        bool quit = false;
        SDL_Event event{};
        renderer.setDeadColor(); // Set color to black
        renderer.renderClear();  // Clear to black screen
        renderer.setLiveColor(); // Set color to white
        std::random_device rd{};
        std::mt19937 mt(rd());
        std::uniform_int_distribution<> col_dist(1, N_COLUMNS);
        std::uniform_int_distribution<> row_dist(1, N_ROWS);
        // Set food randomly
        bool food_set{};
        constexpr N_BUFFER=100;
        size_t rand_counter=0;
        std::array<int, N_BUFFER> row_idx{};
        std::array<int, N_BUFFER> col_idx{};
        for (i=0; i < N_BUFFER; i++){
            row_idx.at(i) = row_dist(mt);
            col_idx.at(i) = col_dist(mt)
        }
        do
        {
            food_set = snake::set_food<N_ROWS, N_COLUMNS>(board_state, row_idx.at(rand_counter), col_idx.at(rand_counter));
        } while (!food_set);
        snake::draw_board(renderer, board_state, rect_array);
        renderer.present();
        SDL_Log("Finished init");
        std::pair<int, int> old_direction = std::pair{0, 1};
        const unsigned char *keystate = SDL_GetKeyboardState(nullptr);
        while (!quit)
        {
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
            std::pair<int, int> new_direction{};
            if (keystate[SDL_SCANCODE_LEFT])
            {
                SDL_FlushEvent(SDL_MOUSEMOTION);
                new_direction = std::pair{0, -1};
            }
            else if (keystate[SDL_SCANCODE_RIGHT])
            {
                SDL_FlushEvent(SDL_MOUSEMOTION);
                new_direction = std::pair{0, 1};
            }
            else if (keystate[SDL_SCANCODE_UP])
            {
                SDL_FlushEvent(SDL_MOUSEMOTION);

                new_direction = std::pair{-1, 0};
            }
            else if (keystate[SDL_SCANCODE_DOWN])
            {
                SDL_FlushEvent(SDL_MOUSEMOTION);

                new_direction = std::pair{1, 0};
            }

            std::pair<int, int> sum_direction = game::add_pairs<int, int>(old_direction, new_direction);
            if ((new_direction.first == 0) && (new_direction.second == 0))
            {
                new_direction = old_direction;
            }
            else if ((sum_direction.first == 0) && (sum_direction.second == 0))
            {
                new_direction = old_direction;
            }
            snake_length += 1;
            bool hit = snake::update_snake<N_ROWS, N_COLUMNS>(snake_head, new_direction, board_state);
            if (hit)
            {
                point_counter++;
                do
                {
                    const int col_idx = col_dist(mt);
                    const int row_idx = row_dist(mt);
                    food_set = snake::set_food<N_ROWS, N_COLUMNS>(board_state, row_idx, col_idx);
                } while (!food_set);
            }
            snake::draw_board(renderer, board_state, rect_array);
            renderer.present();
            old_direction = std::move(new_direction);
            SDL_Delay(50);
            // Check hit and remove food
            // snake::set_snake(snake_position, )
        }
    }
    catch (const std::exception &ex)
    {
        // Quit game
        std::cerr << ex.what() << std::endl;
    }
    // Clean up
    std::cout << "Quit, points: " << point_counter << "snake length: " << snake_length << std::endl;
    SDL_Quit();
    return 0;
}
