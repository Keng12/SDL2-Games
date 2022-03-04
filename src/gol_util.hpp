#ifndef GOL_UTIL_HPP
#define GOL_UTIL_HPP

#include <array>
#include <SDL2/SDL.h>

#include "sdl2_util/video.hpp"

namespace gol_util
{
    template <size_t n_row, size_t n_col>
    void next_state(const std::array<std::array<char, n_col>, n_row> &old_state, std::array<std::array<char, n_col>, n_row> &new_state, sdl2_util::Renderer &renderer, std::array<std::array<SDL_Rect, n_col>, n_row> &rect_array)
    {
        for (size_t row = 0; row < n_row; row++)
        {
            for (size_t col = 0; col < n_col; col++)
            {
                char live_neighbours = getLiveNeighbours<n_row, n_col>(old_state, row, col);
                char cell_state = old_state.at(row).at(col);
                bool cell_survives = cell_state == 1 && (live_neighbours == 2 || live_neighbours == 3);
                bool cell_born = cell_state == 0 && (live_neighbours == 3);
                if (cell_survives || cell_born)
                {
                    // Cell survives, rerendered as it is cleared after each frame
                    new_state.at(row).at(col) = 1;
                    renderer.setLiveColor();
                }
                else
                {
                    // Cell dies
                    new_state.at(row).at(col) = 0;
                    renderer.setDeadColor();
                }
                renderer.fillRect(&rect_array.at(row).at(col)); // Fill rectangle with white color
            }
        }
    }
    template <size_t n_row, size_t n_col>
    char getLiveNeighbours(const std::array<std::array<char, n_col>, n_row> &state, size_t row, size_t col)
    {
        int prev_row_idx = static_cast<int>(row) - 1;
        if (prev_row_idx < 0)
        {
            prev_row_idx = n_row - 1;
        }
        size_t next_row_idx = row + 1;
        if (next_row_idx == n_row)
        {
            next_row_idx = 0;
        }
        int prev_col_idx = static_cast<int>(col) - 1;
        if (prev_col_idx < 0)
        {
            prev_col_idx = n_col - 1;
        }
        size_t next_col_idx = col + 1;
        if (next_col_idx == n_col)
        {
            next_col_idx = 0;
        }
        char live_neighbours = state.at(prev_row_idx).at(prev_col_idx) + state.at(row).at(prev_col_idx) + state.at(next_row_idx).at(prev_col_idx) + state.at(next_row_idx).at(col) + state.at(next_row_idx).at(next_col_idx) + state.at(row).at(next_col_idx) + state.at(prev_row_idx).at(next_col_idx) + state.at(prev_row_idx).at(col);
        return live_neighbours;
    }
}

#endif