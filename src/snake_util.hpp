#ifndef SNAKE_UTIL_HPP
#define SNAKE_UTIL_HPP

#include <array>
#include <random>
#include <deque>

#include "SDL.h"

#include "sdl2_util/video.hpp"

namespace snake
{
    class Snake
    {
        std::deque<SDL_Rect> mPieces{};
        std::deque<int_least8_t> mDirection{};
        std::deque<int_least8_t> mDirectionAbs{};
        void growSnake(SDL_Rect &piece, const int direction, const int increment);
        void growTail();
        void growHead(int increment);
        const double mSpeedFactor{};
        const int mLength{};
        const int mWindowHeight{};
        const int mWindowWidth{};
        int mNewDirection{};
        const unsigned int mSpeedMax{};
        bool mWaitTurn = false;
        void shrinkTail(const int decrement);
        char checkHeadBoundary() const;
        void addPiece();
        bool mCheckThirdLast{};
        int getMovingBound() const;
        int mTarget{};
        void changeDirection();

    public:
        Snake(const int &length, int direction, int window_width, int window_height, double speed_factor);
        int move(double deltaT, int new_direction);
        bool hasHitSelf() const;
        bool hasHitFood(const SDL_Rect *food);
        std::deque<SDL_Rect> getPieces() const { return mPieces; };
        bool foodCheck(const SDL_Rect *food) const;
    };
    void setFood(SDL_Rect &food, std::mt19937_64 &mt, std::uniform_int_distribution<> &col_dist, std::uniform_int_distribution<> &row_dist, const snake::Snake &snake);
    void drawSnake(sdl2_util::Renderer &renderer, const Snake &snake_instance);
    void drawFood(sdl2_util::Renderer &renderer, SDL_Rect *food);
}
#endif