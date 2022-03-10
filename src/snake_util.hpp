#ifndef SNAKE_UTIL_HPP
#define SNAKE_UTIL_HPP

#include <array>
#include <random>

#include "SDL.h"

#include "sdl2_util/video.hpp"
#include "game_util.hpp"

namespace snake
{
    template <size_t N_ROWS, size_t N_COLUMNS>
    std::array<std::array<char, N_COLUMNS>, N_ROWS> constexpr init_board(const int init_snake_length, const std::array<int, 2> &snake_head)
    {
        std::array<std::array<char, N_COLUMNS>, N_ROWS> board{};
        // Set snake
        for (size_t i = 0; i < init_snake_length; i++)
        {
            board.at(snake_head.at(0)).at(snake_head.at(1) - i) = 1;
        }
        return board;
    }
    template <size_t N_ROWS, size_t N_COLUMNS>
    std::array<int, 2> set_food(std::array<std::array<char, N_COLUMNS>, N_ROWS> &board, std::mt19937_64 &mt, std::uniform_int_distribution<> &col_dist, std::uniform_int_distribution<> &row_dist)
    {
        bool food_set{};
        int row_idx{};
        int col_idx{};
        do
        {
            row_idx = row_dist(mt);
            col_idx = col_dist(mt);
            // Check if snake exists on index
            if (board.at(row_idx).at(col_idx) != 1)
            {
                board.at(row_idx).at(col_idx) = 2;
                food_set = true;
            }
        } while (!food_set);
        std::array<int, 2> food_idx{row_idx, col_idx};
        return food_idx;
    }

    template <size_t N_ROWS, size_t N_COLUMNS>
    char update_snake(std::array<int, 2> &snake_head, const std::array<int, 2> &direction, std::array<std::array<char, N_COLUMNS>, N_ROWS> &board)
    {
        snake_head = game::add_arrays<int, 2>(snake_head, direction);
        char game_over = check_game_over<N_ROWS, N_COLUMNS>(snake_head, board);
        if (game_over == 0)
        {
            board.at(snake_head[0]).at(snake_head[1]) = 1; // Set to snake cell
        }
        // Update snake head on board
        return game_over;
    }
    template <size_t N_ROWS, size_t N_COLUMNS>
    char check_game_over(const std::array<int, 2> &snake_head, const std::array<std::array<char, N_COLUMNS>, N_ROWS> &board)
    {
        char game_over{};
        bool boundary_check = (snake_head[0] == -1) || (snake_head[1] == -1) || (snake_head[0] == N_ROWS) || (snake_head[1] == N_COLUMNS);
        if (boundary_check)
        {
            game_over = 1;
        }
        else
        {
            char board_state = board.at(snake_head[0]).at(snake_head[1]);
            if (board_state == 1)
            {
                game_over = 2;
            }
        }
        return game_over;
    }
    template <size_t N_COLUMNS, size_t N_ROWS>
    void draw_board(sdl2_util::Renderer &renderer, const std::array<std::array<char, N_COLUMNS>, N_ROWS> &board, std::array<std::array<SDL_Rect, N_COLUMNS>, N_ROWS> rect_array)
    {
        renderer.setLiveColor();
        for (size_t row = 0; row < N_ROWS; row++)
        {
            for (size_t col = 0; col < N_COLUMNS; col++)
            {
                char board_state = board.at(row).at(col);
                if (board_state == 1 || board_state == 2)
                {
                    renderer.fillRect(&rect_array.at(row).at(col));
                }
            }
        }
    }
    template <class T, size_t FOOD_BUFFER>
    std::array<std::array<T, 2>, FOOD_BUFFER> get_food_idx(std::mt19937 &mt, std::uniform_int_distribution<> &col_dist, std::uniform_int_distribution<> &row_dist)
    {
        std::array<std::array<int, 2>, FOOD_BUFFER> food_idx{};
        for (size_t i = 0; i < FOOD_BUFFER; i++)
        {
            food_idx.at(i) = std::array<int, 2>{row_dist(mt), col_dist(mt)};
        }
        return food_idx;
    }
    template <class T, size_t length>
    std::array<T, length> constexpr get_indices()
    {
        std::array<T, length> indices{};
        std::iota(indices.begin(), indices.end(), 0);
        return indices;
    }
    struct Snake{
        std::vector<SDL_Rect> mPieces {};
        std::string mDirection{};
        double mSpeed{};
        int mWidth{};
        int mHeight{};
        int mPenultimate{};
        char mDirection{};
        int mDirectionAbs{};
        int moveLeft(int deltaT);
        Snake(const int x, const int y, const int width, const int height, const int length_factor, const char direction);
    };
}
#endif