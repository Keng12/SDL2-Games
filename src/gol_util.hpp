/*
Copyright (C) 2022 Keng Yip Chai

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef GOL_UTIL_HPP
#define GOL_UTIL_HPP

#include <array>
#include <assert.h>

#include "SDL.h"

#include "sdl2_util.hpp"

namespace gol
{
    template <size_t n_row, size_t n_col>
    std::array<std::array<SDL_Rect, n_col>, n_row> constexpr init_rect(const int cell_width, const int cell_height)
    {
        std::array<std::array<SDL_Rect, n_col>, n_row> rect_array{};
        for (size_t initRowLoop = 0; initRowLoop < n_row; initRowLoop++)
        {
            for (size_t initColLoop = 0; initColLoop < n_col; initColLoop++)
            {
                rect_array.at(initRowLoop).at(initColLoop).w = cell_width;
                rect_array.at(initRowLoop).at(initColLoop).h = cell_height;
                rect_array.at(initRowLoop).at(initColLoop).x = initColLoop * cell_width;
                rect_array.at(initRowLoop).at(initColLoop).y = initRowLoop * cell_height;
            }
        }
        return rect_array;
    }
    template <class T, size_t n_row, size_t n_col>
    uint_fast8_t getLiveNeighbours(const std::array<std::array<T, n_col>, n_row> &state, size_t row, size_t col)
    {
        size_t prev_row_idx{};
        if (0 == row)
        {
            prev_row_idx = n_row - 1;
        }
        else
        {
            prev_row_idx = row - 1;
        }
        size_t next_row_idx = row + 1;
        if (next_row_idx == n_row || SIZE_MAX == row)
        {
            next_row_idx = 0;
        }
        size_t prev_col_idx{};
        if (col == 0)
        {
            prev_col_idx = n_col - 1;
        }
        else
        {
            prev_col_idx = col - 1;
        }
        size_t next_col_idx = col + 1;
        if (next_col_idx == n_col || SIZE_MAX == col)
        {
            next_col_idx = 0;
        }
        uint_fast8_t live_neighbours = state.at(prev_row_idx).at(prev_col_idx) + state.at(row).at(prev_col_idx) + state.at(next_row_idx).at(prev_col_idx) + state.at(next_row_idx).at(col) + state.at(next_row_idx).at(next_col_idx) + state.at(row).at(next_col_idx) + state.at(prev_row_idx).at(next_col_idx) + state.at(prev_row_idx).at(col);
        return live_neighbours;
    }
    template <class T, size_t n_row, size_t n_col>
    std::array<std::array<T, n_col>, n_row> next_state(const std::array<std::array<T, n_col>, n_row> &old_state, const std::array<std::array<SDL_Rect, n_col>, n_row> &rect_array, const std::array<size_t, n_row> &row_iterator, const std::array<size_t, n_col> &col_iterator)
    {
        sdl2_util::setRenderDrawColor(sdl2_util::RenderColor::white);
        std::array<std::array<T, n_col>, n_row> new_state{};
        std::for_each(std::execution::unseq, row_iterator.cbegin(), row_iterator.cend(), [&](const size_t row)
                      { std::for_each(std::execution::unseq, col_iterator.cbegin(), col_iterator.end(), [&](const size_t col)
                                      {
                                          uint_fast8_t live_neighbours = getLiveNeighbours(old_state, row, col);
                                          T cell_state = old_state.at(row).at(col);
                                          bool cell_survives = cell_state == 1 && (live_neighbours == 2 || live_neighbours == 3);
                                          bool cell_born = cell_state == 0 && (live_neighbours == 3);
                                          if (cell_survives || cell_born)
                                          {
                                              // Cell survives, rectangle filled as white
                                              new_state.at(row).at(col) = 1;
                                              sdl2_util::fillRect(&rect_array.at(row).at(col));
                                          }
                                          else
                                          {
                                              // Cell dies, do not fill rectangle as it clears to black later
                                              new_state.at(row).at(col) = 0;
                                          }
                                      }); });
        return new_state;
    }
}
#endif