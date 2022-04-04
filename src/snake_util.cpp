#include <cstdlib>
#include <algorithm>
#include <execution>
#include <iostream>
#include <deque>
#include <iostream>

#include "snake_util.hpp"
#include "sdl2_util/video.hpp"

namespace snake
{
    int Snake::checkHeadBoundary() const
    {
        int hit = 0;
        switch (mDirection.front())
        {
        case 1:
            if (mPieces.front().x + mPieces.front().w > mWindowWidth)
            {
                hit = 1;
            }
            break;
        case 2:
            if (mPieces.front().y + mPieces.front().h > mWindowHeight)
            {
                hit = 1;
            }
            break;
        case -1:
            if (mPieces.front().x < 0)
            {
                hit = 1;
            }
            break;
        case -2:
            if (mPieces.front().y < 0)
            {
                hit = 1;
            }
            break;
        }
        return hit;
    }

    void Snake::growHead(const int increment)
    {
        growSnake(mPieces.front(), mDirection.front(), increment);
    }

    void Snake::growSnake(SDL_Rect &piece, int direction, int increment)
    {
        switch (direction)
        {
        case 1:
            piece.w = piece.w + increment;
            break;
        case 2:
            piece.h = piece.h + increment;
            break;
        case -1:
            piece.x = piece.x - increment;
            piece.w = piece.w + increment;
            break;
        case -2:
            piece.y = piece.y - increment;
            piece.h = piece.h + increment;
            break;
        default:
            throw std::runtime_error{"Unknown direction in growSnake"};
        }
    }

    void Snake::shrinkTail(int decrement)
    {
        switch (mDirection.back())
        {
        case 1:
            mPieces.back().x = mPieces.back().x + decrement;
            mPieces.back().w = mPieces.back().w - decrement;
            break;
        case 2:
            mPieces.back().y = mPieces.back().y + decrement;
            mPieces.back().h = mPieces.back().h - decrement;
            break;
        case -1:
            mPieces.back().w = mPieces.back().w - decrement;
            break;
        case -2:
            mPieces.back().h = mPieces.back().h - decrement;
            break;
        default:
            throw std::runtime_error{"Unknown direction in shrinkTail"};
        }
        if ((mDirectionAbs.back() == 1 && mPieces.back().w <= 0) || (mDirectionAbs.back() == 2 && mPieces.back().h <= 0))
        {
            int remainder = 0;
            if (mPieces.back().w < 0)
            {
                remainder = (-1) * mPieces.back().w;
            }
            else if (mPieces.back().h < 0)
            {
                remainder = (-1) * mPieces.back().h;
            }
            mPieces.pop_back();
            mDirection.pop_back();
            mDirectionAbs.pop_back();
            if (remainder > 0)
            {
                shrinkTail(remainder);
            }
        }
    }
    
    void Snake::changeDirection()
    {
        addPiece();
        mDirection.push_front(mNewDirection);
        mDirectionAbs.push_front(std::abs(mNewDirection));
    }
    void Snake::addPiece()
    {
        // Call BEFORE setting new direction
        SDL_Rect new_piece{.x = 0, .y = 0, .w = mLength, .h = mLength};
        switch (mDirection.front())
        {
        case 1: // Moving right currently
            new_piece.x = mPieces.front().x + mPieces.front().w - mLength;
            new_piece.y = mPieces.front().y;
            mPieces.front().w = mPieces.front().w - mLength;
            break;
        case 2: // Moving down currently
            new_piece.x = mPieces.front().x;
            new_piece.y = mPieces.front().y + mPieces.front().h - mLength;
            mPieces.front().h = mPieces.front().h - mLength;
            break;
        case -1:
            new_piece.x = mPieces.front().x;
            new_piece.y = mPieces.front().y;
            mPieces.front().x = mPieces.front().x + mLength;
            mPieces.front().w = mPieces.front().w - mLength;
            break;
        case -2:
            new_piece.x = mPieces.front().x;
            new_piece.y = mPieces.front().y;
            mPieces.front().y = mPieces.front().y + mLength;
            mPieces.front().h = mPieces.front().h - mLength;
            break;
        default:
            throw std::runtime_error{"Unknown direction in addPiece"};
        }
        mPieces.push_front(new_piece);
    }

    int Snake::move(double deltaT, int new_direction)
    {
        unsigned int deltaXY = static_cast<unsigned int>(deltaT * mSpeedFactor);
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
            if ((mDirection.front() > 0 && bound >= mTarget) || (mDirection.front() < 0 && bound <= mTarget))
            {
                changeDirection();
                mWaitTurn = false;
            }
        }
        else if ((new_direction != 0) && (std::abs(new_direction) != mDirectionAbs.front())) // Not same or opposite direction
        {
            int bound = getMovingBound();
            switch (mDirection.front())
            {
            case 1: // Moving right currently
                mTarget = bound + mLength;
                break;
            case 2: // Moving down currently
                mTarget = bound + mLength;
                break;
            case -1:
                mTarget = bound - mLength;
                break;
            case -2:
                mTarget = bound - mLength;
                break;
            default:
                throw std::runtime_error{"Unknown direction in move"};
            }
            mWaitTurn = true;
            mNewDirection = new_direction;
        }
        growHead(deltaXY);
        shrinkTail(deltaXY);
        int result = checkHeadBoundary();
        if (mPieces.size() > 3 && hasHitSelf())
        {
            result = 2;
        }
        return result;
    }

    int Snake::getMovingBound() const
    {
        int movingBound{};
        switch (mDirection.front())
        {
        case 1: // Moving right currently
            movingBound = mPieces.front().x + mPieces.front().w;
            break;
        case 2: // Moving down currently
            movingBound = mPieces.front().y + mPieces.front().h;
            break;
        case -1:
            movingBound = mPieces.front().x;
            break;
        case -2:
            movingBound = mPieces.front().y;
            break;
        default:
            throw std::runtime_error{"Unknown direction in movingBound"};
        }
        return movingBound;
    }

    Snake::Snake(const int &length, int direction, int window_width, int window_height, double speed_factor)
        : mLength{length}, mWindowWidth{std::move(window_width)}, mWindowHeight{std::move(window_height)}, mSpeedFactor{std::move(speed_factor)}, mSpeedMax{static_cast<unsigned int>(length)}
    {
        SDL_Rect init_piece{.x = window_width / 2, .y = window_height / 2, .w = mLength, .h = mLength};
        mDirection.push_front(direction);
        mDirectionAbs.push_front(std::abs(direction));
        mPieces.push_front(init_piece);
    }
    bool Snake::hasHitSelf() const
    {
        bool result{};
        std::for_each_n(std::execution::unseq, mPieces.crbegin(), mPieces.size() - 3, [&](const SDL_Rect piece)
                        // clang-format off
                        {
                            SDL_bool head_hit = SDL_HasIntersection(&piece, &mPieces.front()); 
                            if (SDL_TRUE == head_hit)
                            {
                                result = true;
                            }
                        }
                        // clang-format on
        );
        return result;
    }
    bool Snake::hasHitFood(const SDL_Rect *food)
    {
        bool result = foodCheck(food);
        if (result)
        {
            growTail();
        }
        return result;
    }
    bool Snake::foodCheck(const SDL_Rect *food) const
    {
        bool result = std::any_of(std::execution::unseq, mPieces.crbegin(), mPieces.crend(), [&](SDL_Rect piece)
                                  // clang-format off
                                {
                                    bool result = SDL_HasIntersection(&piece, food) == SDL_TRUE;
                                    return result; 
                                }
                                  // clang-format on
        );
        return result;
    }
    void Snake::growTail()
    {
        growSnake(mPieces.back(), (-1) * mDirection.back(), mLength / 2);
    }

    void setFood(SDL_Rect &food, std::mt19937_64 &mt, std::uniform_int_distribution<> &col_dist, std::uniform_int_distribution<> &row_dist, const Snake &snake_instance)
    {
        bool food_hit{};
        do
        {
            int x = col_dist(mt);
            int y = row_dist(mt);
            food.x = x;
            food.y = y;
            food_hit = snake_instance.foodCheck(&food);
        } while (food_hit);
    }
    void drawSnake(sdl2_util::Renderer &renderer, const Snake &snake_instance)
    {
        renderer.setLiveColor();
        const std::deque<SDL_Rect> pieces = snake_instance.getPieces();
        std::for_each(std::execution::unseq, pieces.crbegin(), pieces.crend(), [&](SDL_Rect rect)
                      { renderer.fillRect(&rect); });
    }
    void drawFood(sdl2_util::Renderer &renderer, const SDL_Rect *food)
    {
        renderer.setRenderDrawColor("green");
        renderer.fillRect(food);
    }
}
