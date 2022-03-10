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
        double mSpeed{};
        int mWidth{};
        int mHeight{};
        int mWindowHeight{};
        int mWindowWidth{};
        int mPenultimate{};
        char mDirection{};
        int mDirectionAbs{};
        char moveLeft(int deltaXY);
        char moveUp(int deltaXY);
        char moveDown(int deltaXY);
        char moveRight(int deltaXY);
        void addPiece();
        char move(double deltaT, char new_direction);
        Snake(const int x, const int y, const int width, const int height, const int length_factor, const char direction, const int window_width, const int window_height, const double speed);
        bool hasHitSelf();
        bool hasHitFood(SDL_Rect *food);
    };
    void setFood(SDL_Rect &food, std::mt19937_64 &mt, std::uniform_int_distribution<> &col_dist, std::uniform_int_distribution<> &row_dist, snake::Snake snake);
    void drawSnake(sdl2_util::Renderer &renderer, Snake snake_instance);
}
#endif