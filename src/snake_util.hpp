#ifndef GAME_UTIL_HPP
#define GAME_UTIL_HPP

#include <array>
#include "SDL.h"

#include "sdl2_util/video.hpp"

namespace snake
{
    template <size_t len_snake>
    std::vector<std::pair<int, int>, n_row> constexpr init_snake(int board_width,int board_height)
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
}

#endif