#ifndef SNAKE_UTIL_HPP
#define SNAKE_UTIL_HPP

#include <array>
#include <random>

#include "SDL.h"

#include "sdl2_util/video.hpp"
#include "game_util.hpp"

namespace snake
{
    struct Snake
    {
        std::vector<SDL_Rect> mPieces{};
        std::string mDirection{};
        double mSpeed{};
        int mWidth{};
        int mHeight{};
        int mPenultimate{};
        char mDirection{};
        int mDirectionAbs{};
        int moveLeft(int deltaXY);
        int moveUp(int deltaXY);
        int moveDown(int deltaXY);
        int moveRight(int deltaXY);
        void addPiece();
        int move(double deltaT, char new_direction);
        Snake(const int x, const int y, const int width, const int height, const int length_factor, const char direction);
    };
}
#endif