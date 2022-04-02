#include "snake_util.hpp"
#include <cstdlib>
#include <algorithm>
#include <execution>
#include "sdl2_util/video.hpp"
#include <iostream>
#include <deque>

namespace snake
{
    char Snake::moveLeft(int deltaXY)
    {
        int new_x = mPieces.back().x - deltaXY;
        if (new_x < 0)
        {
            return 1;
        }
        else
        {
            mPieces.back().x = new_x;
            mPieces.back().w = mPieces.back().w + deltaXY;
            return 0;
        }
    };
    char Snake::moveRight(int deltaXY)
    {
        int new_w = mPieces.back().w + deltaXY;
        if (mPieces.back().x + new_w >= mWindowWidth)
        {
            return 1;
        }
        else
        {
            mPieces.back().w = new_w;
            return 0;
        }
    };
    char Snake::moveUp(int deltaXY)
    {
        int new_y = mPieces.back().y - deltaXY;
        if (new_y < 0)
        {
            return 1;
        }
        else
        {
            mPieces.back().y = new_y;
            mPieces.back().h = mPieces.back().h + deltaXY;
            return 0;
        }
    };
    char Snake::moveDown(int deltaXY)
    {
        int new_h = mPieces.back().h + deltaXY;
        if (mPieces.back().y + new_h >= mWindowHeight)
        {
            return 1;
        }
        else
        {
            mPieces.back().h = new_h;
            return 0;
        }
    };
    void Snake::changeDirection()
    {
        addPiece();
        mDirection.push_back(mNewDirection);
        mDirectionAbs = std::abs(mDirection.back());
    }
    void Snake::addPiece()
    {
        // Call BEFORE setting new direction
        SDL_Rect new_piece{.w = mWidth, .h = mHeight};
        switch (mDirection.back())
        {
        case 1: // Moving right currently
            new_piece.x = mPieces.back().x + mPieces.back().w - mWidth;
            new_piece.y = mPieces.back().y;
            mPieces.back().w = mPieces.back().w - mWidth;
            break;
        case 2: // Moving down currently
            new_piece.x = mPieces.back().x;
            new_piece.y = mPieces.back().y + mPieces.back().h - mHeight;
            mPieces.back().h = mPieces.back().h - mHeight;
            break;
        case -1:
            new_piece.x = mPieces.back().x;
            new_piece.y = mPieces.back().y;
            mPieces.back().x = mPieces.back().x + mWidth;
            mPieces.back().w = mPieces.back().w - mWidth;
            break;
        case -2:
            new_piece.x = mPieces.back().x;
            new_piece.y = mPieces.back().y;
            mPieces.back().y = mPieces.back().y + mHeight;
            mPieces.back().h = mPieces.back().h - mHeight;
            break;
        }
        mPieces.push_back(new_piece);
    }

    char Snake::move(double deltaT, char new_direction)
    {
        int deltaXY = static_cast<int>(deltaT * mSpeed);
        if (deltaXY > mSpeedMax)
        {
            deltaXY = mSpeedMax;
        }
        else if (deltaXY == 0)
        {
            deltaXY = 1;
        }
        if (mWaitTurn)
        {
            int bound = getMovingBound();
            if ((mDirection.back() > 0 && bound >= mTarget) || (mDirection.back() < 0 && bound <= mTarget))
            {
                changeDirection();
                mWaitTurn = false;
            }
        }
        else if ((new_direction != 0) && (std::abs(new_direction) != mDirectionAbs)) // Not same or opposite direction
        {
            int bound = getMovingBound();
            switch (mDirection.back())
            {
            case 1: // Moving right currently
                mTarget = bound + mWidth;
                break;
            case 2: // Moving down currently
                mTarget = bound + mHeight;
                break;
            case -1:
                mTarget = bound - mWidth;
                break;
            case -2:
                mTarget = bound - mHeight;
                break;
            }
            mWaitTurn = true;
            mNewDirection = new_direction;
        }
        std::cout << deltaXY << std::endl;
        char result{};
        switch (mDirection.back())
        {
        case -1:
            result = moveLeft(deltaXY);
            break;
        case -2:
            result = moveUp(deltaXY);
            break;
        case 1:
            result = moveRight(deltaXY);
            break;
        case 2:
            result = moveDown(deltaXY);
            break;
        default:
            std::terminate();
        }
        if (mPieces.size() > 3 && hasHitSelf())
        {
            result = 2;
        }
        return result;
    }

    int Snake::getMovingBound()
    {
        unsigned int movingBound{};
        switch (mDirection.back())
        {
        case 1: // Moving right currently
            movingBound = mPieces.back().x + mPieces.back().w;
            break;
        case 2: // Moving down currently
            movingBound = mPieces.back().y + mPieces.back().h;
            break;
        case -1:
            movingBound = mPieces.back().x;
            break;
        case -2:
            movingBound = mPieces.back().y;
            break;
        }
        return movingBound;
    }

    Snake::Snake(const int x, const int y, const int width, const int height, const char direction, const int window_width, const int window_height, const double speed, const double speedMax)
        : mHeight{height}, mWidth{width}, mWindowWidth{window_width}, mWindowHeight{window_height}, mSpeed{speed}, mSpeedMax{speedMax}
    {
        SDL_Rect init_piece{.x = x, .y = y, .w = mWidth, .h = mHeight};
        mDirection.push_back(direction);
        mDirectionAbs = std::abs(direction);
        mPieces.push_back(init_piece);
    }
    bool Snake::hasHitSelf()
    {
        bool result{};
        std::for_each_n(std::execution::unseq, mPieces.cbegin(), mPieces.size() - 3, [&](const SDL_Rect piece)
                        // clang-format off
                        {
                            SDL_bool head_hit = SDL_HasIntersection(&piece, &mPieces.back()); 
                            if (SDL_TRUE == head_hit)
                            {
                                result = true;
                            }
                        }
                        // clang-format on
                        );
        return result;
    }
    bool Snake::hasHitFood(SDL_Rect *food)
    {
        bool result = std::any_of(std::execution::unseq, mPieces.cbegin(), mPieces.cend(), [&](SDL_Rect piece)
                                  {bool result = SDL_HasIntersection(&piece, food) == SDL_TRUE; return result; });
        return result;
    }
    void setFood(SDL_Rect &food, std::mt19937_64 &mt, std::uniform_int_distribution<> &col_dist, std::uniform_int_distribution<> &row_dist, Snake snake_instance)
    {
        bool food_hit{};
        do
        {
            int x = (col_dist(mt) / snake_instance.mWidth) * snake_instance.mWidth;
            int y = (row_dist(mt) / snake_instance.mHeight) * snake_instance.mHeight;
            food.x = x;
            food.y = y;
            food_hit = snake_instance.hasHitFood(&food);
        } while (food_hit);
    }
    void drawSnake(sdl2_util::Renderer &renderer, Snake snake_instance)
    {
        renderer.setLiveColor();
        std::for_each(std::execution::unseq, snake_instance.mPieces.cbegin(), snake_instance.mPieces.cend(), [&](SDL_Rect rect)
                      { renderer.fillRect(&rect); });
    }
    void drawFood(sdl2_util::Renderer &renderer, SDL_Rect *food)
    {
        renderer.setRenderDrawColor("green");
        renderer.fillRect(food);
    }
}
