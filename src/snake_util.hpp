#ifndef SNAKE_UTIL_HPP
#define SNAKE_UTIL_HPP

#include <array>
#include <vector>
#include <random>

#include "SDL.h"

#include "sdl2_util/video.hpp"

namespace snake
{
    template <size_t init_length>
    std::array<std::pair<int, int>, init_length> constexpr init_snake(const int n_col, const int n_row)
    {
        std::array<std::pair<int, int>, init_length> init_snake_position{};
        const int half_width = n_col / 2;
        const int half_height = n_row / 2;
        for (size_t i = 0; i < init_length; i++)
        {
            init_snake_position.at(i).first = half_width + i;
            init_snake_position.at(i).second = half_height;
        }
        return init_snake_position;
    };
    template <size_t N_COLUMNS, size_t N_ROWS, size_t init_snake_length>
    std::array<std::array<char, N_COLUMNS + 2>, N_ROWS + 2> constexpr init_board(const std::array<std::pair<int, int>, init_snake_length> &snake)
    {
        std::array<std::array<char, N_COLUMNS + 2>, N_ROWS + 2> board{};
        // Set snake
        for (auto position : snake)
        {
            board.at(position.first + 1).at(position.second + 1) = 1;
        }
        // Set border
        for (size_t row = 0; row < N_ROWS + 2; row++)
        {
            board.at(row).at(0) = 1;
            board.at(row).at(N_COLUMNS + 1) = 1;
        }
        for (size_t col = 0; col < N_ROWS + 2; col++)
        {
            board.at(0).at(col) = 1;
            board.at(N_ROWS + 1).at(col) = 1;
        }
        return board;
    }
    template <size_t N_COLUMNS, size_t N_ROWS>
    std::pair<int, int> get_food_idx(const std::vector<std::pair<int, int>> &snake)
    {
        std::random_device rd{};
        std::mt19937 mt(rd());
        std::uniform_int_distribution<> col_dist(1, N_COLUMNS);
        std::uniform_int_distribution<> row_dist(1, N_ROWS);
        std::pair<int, int> food_idx{};
        while (true)
        {
            // Set food randomly
            const int col_idx = col_dist(mt);
            const int row_idx = row_dist(mt);
            bool overlap{};
            // Check if snake exists on index
            for (auto position : snake)
            {
                if (position.first == col_idx && position.second == row_idx)
                {
                    overlap = true;
                    break;
                }
            }
            if (overlap == false)
            {
                food_idx = std::pair{col_idx, row_idx};
                break;
            }
        }
        return food_idx;
    }
    template <size_t N_COLUMNS, size_t N_ROWS>
    void set_food(std::array<std::array<char, N_COLUMNS + 2>, N_ROWS + 2> &board, std::pair<int, int> &food_idx)
    {
        board.at(food_idx.first).at(food_idx.second) = 2;
    }
    template <size_t N_COLUMNS, size_t N_ROWS>
    void update_snake(std::vector<std::pair<int, int>> &snake, const std::pair<int, int> &direction, const std::array<std::array<char, N_COLUMNS + 2>, N_ROWS + 2> &board)
    {
        std::pair<int, int> old_head = snake.front();
        std::pair<int, int>new_head = old_head + direction;
        char board_state = board.at(new_head[0]).at(new_head[1]);
        bool add_piece{};
        std::pair<int, int> tail{};
        if (board_state == 1) // Hit border or itself
        {
            throw std::runtime_error("Game over");
        }
        else if (board_state == 2) // Hit food
        {   
            tail = snake.back()
            add_piece = true;
        }
        // Update snake in reverse order after moving head
        snake.front() = new_head;
        for (size_t i = snake.size() - 1; i > 0; i--)
        {
            snake.at(i) = snake.at(i - 1);
            if (i == 1)
            {
                snake.at(i) = old_head;
            }
        }
        if (add_piece)
        {
            snake.push_back(tail);
        }
    }
}
#endif