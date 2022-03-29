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
        char mDirection{};
        int mDirectionAbs{};
        char mNewDirection{};
        double mSpeedMax{};
        bool mWaitTurn = false;
        char moveLeft(int deltaXY);
        char moveUp(int deltaXY);
        char moveDown(int deltaXY);
        char moveRight(int deltaXY);
        void addPiece();
        char move(double deltaT, char new_direction);
        Snake(const int x, const int y, const int width, const int height, const char direction, const int window_width, const int window_height, const double speed, const double speedMax);
        bool hasHitSelf();
        bool hasHitFood(SDL_Rect *food);
        bool mCheckThirdLast{};
        unsigned int getCellIndex();
        unsigned int mOldCellIndex{};
        void changeDirection();
    };
    void setFood(SDL_Rect &food, std::mt19937_64 &mt, std::uniform_int_distribution<> &col_dist, std::uniform_int_distribution<> &row_dist, snake::Snake snake);
    void drawSnake(sdl2_util::Renderer &renderer, Snake snake_instance);
    void drawFood(sdl2_util::Renderer &renderer, SDL_Rect* food);
}
#endif