#include "snake_util.hpp"
#include <cstdlib>
#include <algorithm>
#include <execution>
#include "sdl2_util/video.hpp"
#include <iostream>
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
    void Snake::addPiece()
    {
        SDL_Rect new_piece{};
        new_piece.w = mWidth;
        new_piece.h = mHeight;
        switch (mDirection)
        {
        case 1: // Move right
            new_piece.x = mPieces.back().x + mPieces.back().w - mWidth;
            new_piece.y = mPieces.back().y;
            break;
        case 2: // Move down
            new_piece.x = mPieces.back().x;
            new_piece.y = mPieces.back().y + mPieces.back().h - mHeight;
            break;
        default:
            new_piece.x = mPieces.back().x;
            new_piece.y = mPieces.back().y;
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

        if ((new_direction != 0) && std::abs(new_direction) != mDirectionAbs) // Same or opposite direction
        {
            addPiece();
            deltaXY = mWidth;
            mDirection = new_direction;
            mDirectionAbs = std::abs(mDirection);
        }
        std::cout << deltaXY << std::endl;
        char result{};
        switch (mDirection)
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
            throw std::runtime_error("Direction: " + std::to_string(mDirection) + " unknown");
        }
        if (mPieces.size() > 3 && hasHitSelf())
        {
            result = 2;
        }
        return result;
    }
    Snake::Snake(const int x, const int y, const int width, const int height, const char direction, const int window_width, const int window_height, const double speed, const double speedMax)
        : mHeight{height}, mWidth{width}, mWindowWidth{window_width}, mWindowHeight{window_height}, mDirection{direction}, mSpeed{speed}, mSpeedMax{speedMax}
    {
        SDL_Rect init_piece{};
        mDirectionAbs = std::abs(mDirection);
        init_piece.w = mWidth;
        init_piece.h = mHeight;
        init_piece.x = x;
        init_piece.y = y;
        mPieces.push_back(init_piece);
        addPiece();
        mPenultimate = 0;
    }
    bool Snake::hasHitSelf()
    {
        bool result{};
        for (int i = 0; i < mPieces.size() - 3; i++)
        {
            SDL_bool head_hit = SDL_HasIntersection(&mPieces.at(i), &mPieces.back());
            if (head_hit == SDL_TRUE)
            {
                result = true;
            }
        }
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
            food.x = col_dist(mt);
            food.y = row_dist(mt);
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
