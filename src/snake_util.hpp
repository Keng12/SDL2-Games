#ifndef SNAKE_UTIL_HPP
#define SNAKE_UTIL_HPP

#include <array>
#include "SDL.h"

#include "sdl2_util/video.hpp"

namespace snake
{
    std::vector<std::pair<int, int>> constexpr init_snake(const int board_width, const int board_height, const int init_length)
    {
        std::vector<std::pair<int, int>> init_snake_position{};
        init_snake_position.reserve(init_length);
        int half_width = board_width / 2;
        int half_height = board_height / 2;
        for (size_t i = 0; row < init_length; row++)
        {
            init_snake_position.push_back(std::pair<int, int>{half_width + i, half_height});
        }
        return init_snake_position;
    }
}

#endif