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
    template <class T, size_t length>
    std::array<T, length> add_arrays(const std::array<T, length> &array1, const std::array<T, length> &array2)
    {
        std::array<T, length> result{};
        for (size_t i = 0; i < length; i++)
        {
            result[i] = array1[i] + array2[i];
        }
        return result;
    }
    template <class T, size_t length>
    T sum_array(const std::array<T, length> &array)
    {
        T result{};
        for (T element : array)
        {
            result += element;
        }
        return result;
    }
    template <class T, size_t length>
    bool check_equality_arrays(const std::array<T, length> &array1, const std::array<T, length> &array2){
        std::array<unsigned int, length> equality_array{};
        for (size_t i = 0; i<length; i++){
            equality_array[i] = static_cast<unsigned int>(array1[i] == array2[i]);
        }
        unsigned int count = sum_array<unsigned int, length>(equality_array);
        bool result{};
        if (count == length){
            result=true;
        }
        return result;
    }
}

#endif