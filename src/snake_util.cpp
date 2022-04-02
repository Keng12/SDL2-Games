#include "snake_util.hpp"
#include <cstdlib>
#include <algorithm>
#include <execution>
#include "sdl2_util/video.hpp"
#include <iostream>
#include <deque>
#include <unordered_map>

static std::unordered_map<char, std::string> mapDirection{{1, "right"}, {2, "up"}, {-1, "left"}, {-2, "down"}};
namespace snake
{
    char Snake::checkHeadBoundary()
    {
        char hit = 0;
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

    void Snake::growHead()
    {
        growSnake(mPieces.front(), mDirection.front(), mDeltaXY);
    }

    void Snake::growSnake(SDL_Rect &piece, char direction, int increment)
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
            std::terminate();
        }
    };
    void Snake::shrinkTail()
    {
        switch (mDirection.back())
        {
        case 1:
            mPieces.back().x = mPieces.back().x + mDeltaXY;
            mPieces.back().w = mPieces.back().w - mDeltaXY;
            break;
        case 2:
            mPieces.back().y = mPieces.back().y + mDeltaXY;
            mPieces.back().h = mPieces.back().h - mDeltaXY;
            break;
        case -1:
            mPieces.back().w = mPieces.back().w - mDeltaXY;
            break;
        case -2:
            mPieces.back().h = mPieces.back().h - mDeltaXY;
            break;
        default:
            std::terminate();
        }
        if ((mDirectionAbs.back() == 1 && mPieces.back().w <= 0) || (mDirectionAbs.back() == 2 && mPieces.back().h <= 0))
        {
            mPieces.pop_back();
            mDirection.pop_back();
            mDirectionAbs.pop_back();
        }
    };
    void Snake::changeDirection()
    {
        addPiece();
        mDirection.push_front(mNewDirection);
        mDirectionAbs.push_front(std::abs(mNewDirection));
    }
    void Snake::addPiece()
    {
        // Call BEFORE setting new direction
        SDL_Rect new_piece{.w = mWidth, .h = mHeight};
        switch (mDirection.front())
        {
        case 1: // Moving right currently
            new_piece.x = mPieces.front().x + mPieces.front().w - mWidth;
            new_piece.y = mPieces.front().y;
            mPieces.front().w = mPieces.front().w - mWidth;
            break;
        case 2: // Moving down currently
            new_piece.x = mPieces.front().x;
            new_piece.y = mPieces.front().y + mPieces.front().h - mHeight;
            mPieces.front().h = mPieces.front().h - mHeight;
            break;
        case -1:
            new_piece.x = mPieces.front().x;
            new_piece.y = mPieces.front().y;
            mPieces.front().x = mPieces.front().x + mWidth;
            mPieces.front().w = mPieces.front().w - mWidth;
            break;
        case -2:
            new_piece.x = mPieces.front().x;
            new_piece.y = mPieces.front().y;
            mPieces.front().y = mPieces.front().y + mHeight;
            mPieces.front().h = mPieces.front().h - mHeight;
            break;
        }
        mPieces.push_front(new_piece);
    }

    char Snake::move(double deltaT, char new_direction)
    {
        mDeltaXY = static_cast<int>(deltaT * mSpeed);
        if (mDeltaXY > mSpeedMax)
        {
            mDeltaXY = mSpeedMax;
        }
        else if (mDeltaXY == 0)
        {
            mDeltaXY = 1;
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
        growHead();
        shrinkTail();
        char result = checkHeadBoundary();
        if (mPieces.size() > 3 && hasHitSelf())
        {
            result = 2;
        }
        return result;
    }

    int Snake::getMovingBound()
    {
        unsigned int movingBound{};
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
        }
        return movingBound;
    }

    Snake::Snake(const int x, const int y, const int width, const int height, const char direction, const int window_width, const int window_height, const double speed, const double speedMax)
        : mHeight{height}, mWidth{width}, mWindowWidth{window_width}, mWindowHeight{window_height}, mSpeed{speed}, mSpeedMax{speedMax}
    {
        SDL_Rect init_piece{.x = x, .y = y, .w = mWidth, .h = mHeight};
        mDirection.push_front(direction);
        mDirectionAbs.push_front(std::abs(direction));
        mPieces.push_front(init_piece);
    }
    bool Snake::hasHitSelf()
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
    bool Snake::hasHitFood(SDL_Rect *food)
    {
        bool result = std::any_of(std::execution::unseq, mPieces.crbegin(), mPieces.crend(), [&](SDL_Rect piece)
                                  // clang-format off
                                {
                                    bool result = SDL_HasIntersection(&piece, food) == SDL_TRUE;
                                    return result; 
                                }
                                  // clang-format on
        );
        if (result)
        {
            growTail();
        }
        return result;
    }

    void Snake::growTail()
    {
        std::cout<<"grow tail" << std::endl;
        growSnake(mPieces.back(), (-1) * mDirection.back(), mWidth / 2);
    }

    void setFood(SDL_Rect &food, std::mt19937_64 &mt, std::uniform_int_distribution<> &col_dist, std::uniform_int_distribution<> &row_dist, Snake snake_instance)
    {
        bool food_hit{};
        do
        {
            int x = col_dist(mt)  * snake_instance.mWidth;
            int y = row_dist(mt)  * snake_instance.mHeight;
            food.x = x;
            food.y = y;
            food_hit = snake_instance.hasHitFood(&food);
        } while (food_hit);
    }
    void drawSnake(sdl2_util::Renderer &renderer, Snake snake_instance)
    {
        renderer.setLiveColor();
        std::for_each(std::execution::unseq, snake_instance.mPieces.crbegin(), snake_instance.mPieces.crend(), [&](SDL_Rect rect)
                      { renderer.fillRect(&rect); });
    }
    void drawFood(sdl2_util::Renderer &renderer, SDL_Rect *food)
    {
        renderer.setRenderDrawColor("green");
        renderer.fillRect(food);
    }
}
