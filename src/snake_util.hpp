#ifndef SNAKE_UTIL_HPP
#define SNAKE_UTIL_HPP

#include <array>
#include <vector>
#include <random>

#include "SDL.h"

#include "sdl2_util/video.hpp"
#include "game_util.hpp"

namespace snake
{
    template <size_t init_length>
    std::array<std::pair<int, int>, init_length> constexpr init_snake(const int n_row, const int n_col)
    {
        std::array<std::pair<int, int>, init_length> init_snake_position{};
        const int half_width = n_col / 2;
        const int half_height = n_row / 2;
        for (size_t i = 0; i < init_length; i++)
        {
            init_snake_position.at(i).first = half_height + i;
            init_snake_position.at(i).second = half_width;
        }
        return init_snake_position;
    };
    template <size_t N_ROWS, size_t N_COLUMNS, size_t init_snake_length>
    std::array<std::array<char, N_COLUMNS>, N_ROWS> constexpr init_board(const std::array<std::pair<int, int>, init_snake_length> &snake)
    {
        std::array<std::array<char, N_COLUMNS>, N_ROWS> board{};
        // Set snake
        for (auto position : snake)
        {
            board.at(position.first).at(position.second) = 1;
        }
        return board;
    }
    template <size_t N_ROWS, size_t N_COLUMNS>
    bool set_food(std::array<std::array<char, N_COLUMNS>, N_ROWS> &board, const int row_idx, const int col_idx)
    {
        // Check if snake exists on index
        if (board.at(row_idx).at(col_idx) == 1)
        {
            return false;
        }
        else
        {
            board.at(row_idx).at(col_idx) = 2;
            return true;
        }
    }

    template <size_t N_ROWS, size_t N_COLUMNS>
    bool update_snake(std::pair<int, int> &snake_head, const std::pair<int, int> &direction, std::array<std::array<char, N_COLUMNS>, N_ROWS> &board)
    {
        std::pair<int, int> new_head = game::add_pairs<int, int>(snake_head, direction);
        bool boundary_check = (new_head.first == -1) || (new_head.second == -1) || (new_head.first == N_ROWS) || (new_head.second == N_COLUMNS);
        if (boundary_check)
        {
            throw std::runtime_error("Snake touched boundary");
        }
        char board_state = board.at(new_head.first).at(new_head.second);
        bool hit_food{};
        if (board_state == 1) // Hit border or itself
        {
            throw std::runtime_error("Snake hit itself");
        }
        else if (board_state == 2) // Hit food
        {
            hit_food = true;
        }
        // Update snake head
        snake_head = new_head;
        board.at(new_head.first).at(new_head.second) = 1; // Set to snake cell
        return hit_food;
    }
    template <size_t N_COLUMNS, size_t N_ROWS>
    void draw_board(sdl2_util::Renderer &renderer, const std::array<std::array<char, N_COLUMNS>, N_ROWS> &board, std::array<std::array<SDL_Rect, N_COLUMNS>, N_ROWS> rect_array)
    {
        for (size_t row = 0; row < N_ROWS ; row++)
        {
            for (size_t col = 0; col < N_COLUMNS ; col++)
            {
                char board_state = board.at(row).at(col);
                if (board_state == 1 || board_state == 2)
                {
                    renderer.setLiveColor();
                }
                else
                {
                    renderer.setDeadColor();
                }
                renderer.fillRect(&rect_array.at(row ).at(col ));
            }
        }
    }
}
#endif