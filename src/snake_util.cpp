/*
Copyright (C) 2022 Keng Yip Chai

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <cstdlib>
#include <algorithm>
#include <execution>
#include <iostream>
#include <deque>
#include <iostream>
#include <limits>
#include "snake_util.hpp"
#include "sdl2_util.hpp"
#include <assert.h>

namespace snake
{
    int Snake::checkHeadBoundary() const
    {
        int hit{};
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
        case RIGHT:
            piece.w = piece.w + increment;
            break;
        case DOWN:
            piece.h = piece.h + increment;
            break;
        case LEFT:
            piece.x = piece.x - increment;
            piece.w = piece.w + increment;
            break;
        case UP:
            piece.y = piece.y - increment;
            piece.h = piece.h + increment;
            break;
        default:
            std::cerr << "Unknown direction in growSnake \n";
            std::terminate();
        }
    }

    void Snake::shrinkTail(int decrement)
    {
        switch (mDirection.back())
        {
        case RIGHT:
            mPieces.back().x = mPieces.back().x + decrement;
            mPieces.back().w = mPieces.back().w - decrement;
            break;
        case DOWN:
            mPieces.back().y = mPieces.back().y + decrement;
            mPieces.back().h = mPieces.back().h - decrement;
            break;
        case LEFT:
            mPieces.back().w = mPieces.back().w - decrement;
            break;
        case UP:
            mPieces.back().h = mPieces.back().h - decrement;
            break;
        default:
            std::cerr << "Unknown direction in shrinkTail \n";
            std::terminate();
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
        SDL_Rect new_piece = sdl2_util::initRect(0, 0, mLength, mLength);
        switch (mDirection.front())
        {
        case RIGHT: // Moving right currently
            new_piece.x = mPieces.front().x + mPieces.front().w - mLength;
            new_piece.y = mPieces.front().y;
            mPieces.front().w = mPieces.front().w - mLength;
            break;
        case DOWN: // Moving down currently
            new_piece.x = mPieces.front().x;
            new_piece.y = mPieces.front().y + mPieces.front().h - mLength;
            mPieces.front().h = mPieces.front().h - mLength;
            break;
        case LEFT:
            new_piece.x = mPieces.front().x;
            new_piece.y = mPieces.front().y;
            mPieces.front().x = mPieces.front().x + mLength;
            mPieces.front().w = mPieces.front().w - mLength;
            break;
        case UP:
            new_piece.x = mPieces.front().x;
            new_piece.y = mPieces.front().y;
            mPieces.front().y = mPieces.front().y + mLength;
            mPieces.front().h = mPieces.front().h - mLength;
            break;
        default:
            std::cerr << "Unknown direction in addPiece \n";
            std::terminate();
        }
        mPieces.push_front(new_piece);
    }

    int Snake::move(double deltaT, int new_direction)
    {
        double current_speed = deltaT * mSpeedFactor;
        int deltaXY{};
        if (current_speed > mSpeedMax || current_speed > std::numeric_limits<int>::max())
        {
            deltaXY = mSpeedMax;
        }
        else
        {
            deltaXY = static_cast<int>(current_speed);
            if (deltaXY == 0)
            {
                deltaXY = 1;
            }
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
            case RIGHT: // Moving right currently
                mTarget = bound + mLength;
                break;
            case DOWN: // Moving down currently
                mTarget = bound + mLength;
                break;
            case LEFT:
                mTarget = bound - mLength;
                break;
            case UP:
                mTarget = bound - mLength;
                break;
            default:
                std::cerr << "Unknown direction in move \n";
                std::terminate();
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
        case RIGHT: // Moving right currently
            movingBound = mPieces.front().x + mPieces.front().w;
            break;
        case DOWN: // Moving down currently
            movingBound = mPieces.front().y + mPieces.front().h;
            break;
        case LEFT:
            movingBound = mPieces.front().x;
            break;
        case UP:
            movingBound = mPieces.front().y;
            break;
        default:
            std::cerr << "Unknown direction in movingBound \n";
            std::terminate();
        }
        return movingBound;
    }

    Snake::Snake(const int &length, int direction, int window_width, int window_height, double speed_factor)
        : mLength{length}, mWindowWidth{std::move(window_width)}, mWindowHeight{std::move(window_height)}, mSpeedFactor{std::move(speed_factor)}, mSpeedMax{length}
    {
        SDL_Rect init_piece = sdl2_util::initRect(window_width / 2, window_height / 2, mLength, mLength);
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
                                    bool result_i = SDL_HasIntersection(&piece, food) == SDL_TRUE;
                                    return result_i; 
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
    void drawSnake(const Snake &snake_instance)
    {
        sdl2_util::setRenderDrawColor(sdl2_util::RenderColor::white);
        const std::deque<SDL_Rect> pieces = snake_instance.getPieces();
        std::for_each(std::execution::unseq, pieces.cbegin(), pieces.cend(), [&](SDL_Rect rect)
                      { sdl2_util::fillRect(&rect); });
    }
    void drawFood(const SDL_Rect *food)
    {
        sdl2_util::setRenderDrawColor(sdl2_util::RenderColor::green);
        sdl2_util::fillRect(food);
    }
}
