#ifndef GOL_UTIL_HPP
#define GOL_UTIL_HPP

#include <array>
#include "SDL.h"

#include "sdl2_util/video.hpp"

static int prev_row_idx{};
static int live_neighbours{};
static int cell_state{};
static int next_row_idx{};
static bool cell_survives{};
static bool cell_born{};
static int prev_col_idx{};
static int next_col_idx{};

namespace gol
{
    template <size_t n_row, size_t n_col>
    std::array<std::array<SDL_Rect, n_col>, n_row> constexpr init_rect(const int cell_width, const int cell_height)
    {
        std::array<std::array<SDL_Rect, n_col>, n_row> rect_array{};
        for (size_t row = 0; row < n_row; row++)
        {
            for (size_t col = 0; col < n_col; col++)
            {
                rect_array.at(row).at(col).w = cell_width;
                rect_array.at(row).at(col).h = cell_height;
                rect_array.at(row).at(col).x = col * cell_width;
                rect_array.at(row).at(col).y = row * cell_height;
            }
        }
        return rect_array;
    }
    template <size_t n_row, size_t n_col>
    int getLiveNeighbours(const std::array<std::array<int, n_col>, n_row> &state, size_t row, size_t col)
    {
        prev_row_idx = row - 1;
        if (prev_row_idx < 0)
        {
            prev_row_idx = n_row - 1;
        }
        next_row_idx = row + 1;
        if (next_row_idx == n_row)
        {
            next_row_idx = 0;
        }
        prev_col_idx = col - 1;
        if (prev_col_idx < 0)
        {
            prev_col_idx = n_col - 1;
        }
        next_col_idx = col + 1;
        if (next_col_idx == n_col)
        {
            next_col_idx = 0;
        }
        live_neighbours = state.at(prev_row_idx).at(prev_col_idx) + state.at(row).at(prev_col_idx) + state.at(next_row_idx).at(prev_col_idx) + state.at(next_row_idx).at(col) + state.at(next_row_idx).at(next_col_idx) + state.at(row).at(next_col_idx) + state.at(prev_row_idx).at(next_col_idx) + state.at(prev_row_idx).at(col);
        return live_neighbours;
    }
    template <size_t n_row, size_t n_col>
    std::array<std::array<int, n_col>, n_row> next_state(const std::array<std::array<int, n_col>, n_row> &old_state, SDL_Renderer* renderer, const std::array<std::array<SDL_Rect, n_col>, n_row> &rect_array)
    {
        sdl2_util::setLiveColor(renderer);

        std::array<std::array<int, n_col>, n_row> new_state{};
        for (size_t row = 0; row < n_row; row++)
        {
            for (size_t col = 0; col < n_col; col++)
            {
                live_neighbours = getLiveNeighbours<n_row, n_col>(old_state, row, col);
                cell_state = old_state.at(row).at(col);
                cell_survives = cell_state == 1 && (live_neighbours == 2 || live_neighbours == 3);
                cell_born = cell_state == 0 && (live_neighbours == 3);
                if (cell_survives || cell_born)
                {
                    // Cell survives, rectangle filled as white
                    new_state.at(row).at(col) = 1;
                    sdl2_util::fillRect(renderer, &rect_array.at(row).at(col));
                }
                else
                {
                    // Cell dies, do not fill rectangle as it clears to black later
                    new_state.at(row).at(col) = 0;
                }
            }
        }
        return new_state;
    }
}

#endif