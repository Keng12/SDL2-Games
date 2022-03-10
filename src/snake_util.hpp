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
        bool hasHitSelf();
        bool hasHitFood(SDL_Rect food);
    };
    void setFood(SDL_Rect &food, std::mt19937_64 &mt, std::uniform_int_distribution<> &col_dist, std::uniform_int_distribution<> &row_dist, snake::Snake snake);
}
#endif