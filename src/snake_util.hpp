#ifndef SNAKE_UTIL_HPP
#define SNAKE_UTIL_HPP

#include <array>
#include <random>
#include <deque>

#include "SDL.h"

#include "sdl2_util.hpp"

namespace snake
{
    class Snake
    {
        const int mLength{};
        const int mWindowWidth{};
        const int mWindowHeight{};
        const double mSpeedFactor{};
        const int mSpeedMax{};

        int mNewDirection{};
        bool mWaitTurn = false;
        int mTarget{};

        std::deque<SDL_Rect> mPieces{};
        std::deque<int> mDirection{};
        std::deque<int> mDirectionAbs{};

        void growSnake(SDL_Rect &piece, const int direction, const int increment);
        void growTail();
        void growHead(const int increment);
        void shrinkTail(const int decrement);
        void addPiece();
        void changeDirection();
        int checkHeadBoundary() const;
        int getMovingBound() const;

    public:
        Snake(const int &length, int direction, int window_width, int window_height, double speed_factor);
        int move(const double deltaT, const int new_direction);
        bool hasHitSelf() const;
        bool hasHitFood(const SDL_Rect *food);
        std::deque<SDL_Rect> getPieces() const { return mPieces; };
        bool foodCheck(const SDL_Rect *food) const;
    };
    void setFood(SDL_Rect &food, std::mt19937_64 &mt, std::uniform_int_distribution<> &col_dist, std::uniform_int_distribution<> &row_dist, const snake::Snake &snake);
    void drawSnake(const Snake &snake_instance);
    void drawFood(const SDL_Rect *food);
}
#endif