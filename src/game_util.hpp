#ifndef GAME_UTIL_HPP
#define GAME_UTIL_HPP

#include <algorithm>
#include <array>
#include <execution>
#include <chrono>
#include <getopt.h>
#include <tuple>
#include <iostream>

#include "SDL.h"

#include "sdl2_util/video.hpp"

namespace game
{

    template <class T, size_t length>
    std::array<T, length> add_arrays(const std::array<T, length> &array1, const std::array<T, length> &array2)
    {
        std::array<T, length> output_array{};
        std::transform(std::execution::unseq, array1.cbegin(), array1.cend(), array2.cbegin(), output_array.begin(), [&](T elem1, T elem2) -> T
                       { return elem1 + elem2; });
        return output_array;
    }
    template <class T, size_t length>
    std::array<T, length> scale_array(const std::array<T, length> &array1, const T factor)
    {
        std::array<T, length> output{};
        std::for_each(std::execution::unseq, array1.begin(), array1.end(), [&](T elem)
                      {T result = elem * factor; return result; });
        return output;
    }
    template <class T, size_t length>
    T sum_array(const std::array<T, length> &array)
    {
        T sum = 0;
        std::for_each(std::execution::unseq, array.cbegin(), array.cend(), [&](T elem)
                      { sum = sum + elem; });
        return sum;
    }
    template <class T, size_t length>
    bool check_equality_arrays(const std::array<T, length> &array1, const std::array<T, length> &array2)
    {
        bool result = std::equal(std::execution::unseq, array1.cbegin(), array1.cend(), array2.cbegin());
        return result;
    }
}

#endif