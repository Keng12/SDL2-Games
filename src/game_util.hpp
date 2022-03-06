#ifndef GAME_UTIL_HPP
#define GAME_UTIL_HPP

#include <array>
#include "SDL.h"

#include "sdl2_util/video.hpp"

namespace game
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
    template <class T1, class T2>
    std::pair<T1, T2> add_pairs(std::pair<T1, T2> pair1, std::pair<T1, T2> pair2)
    {
        std::pair<T1, T2> result{};
        result.first = pair1.first + pair2.first;
        resutl.second = pair1.second + pair2.second;
        return result;
    }
}

#endif