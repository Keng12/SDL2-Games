#include "snake_util.hpp"
#include <cstdlib>
#include <algorithm>
#include <execution>

namespace snake
{
    int Snake::moveLeft(int deltaXY)
    {
        int new_x = pieces.back().x - deltaXY;
        if (new_x < 0)
        {
            return 1
        }
        else
        {
            mPieces.back().x = new_x;
            mPieces.back().w = mWidth + deltaXY;
            return 0;
        }
    };
    int Snake::moveRight(int deltaXY)
    {
        int new_x = pieces.back().x + deltaXY;
        if (new_x + mWidth >= mRightBound)
        {
            return 1
        }
        else
        {
            mPieces.back().x = new_x;
            mPieces.at(mPenultimate).w = mPieces.at(mPenultimate).w + deltaXY;
            return 0;
        }
    };
    int Snake::moveUp(int deltaXY)
    {
        int new_y = pieces.back().y - deltaXY;
        if (new_y < 0)
        {
            return 1
        }
        else
        {
            mPieces.back().y = new_y;
            mPieces.back().h = mHeight + deltaXY;
            return 0;
        }
    };
    int Snake::moveDown(int deltaXY)
    {
        int new_y = pieces.back().y + deltaXY;
        if (new_y + mHeight >= mBottomBound)
        {
            return 1
        }
        else
        {
            mPieces.back().y = new_y;
            mPieces.at(mPenultimate).h = mPieces.at(mPenultimate).h + deltaXY;
            return 0;
        }
    };
    int Snake::addPiece()
    {
        SDL_Rect new_piece{};
        new_piece.x = mPieces.back().x;
        new_piece.y = mPieces.back().y;
        new_piece.w = mWidth;
        new_piece.h = mHeight;
        mPieces.push_back(new_piece);
    }
    int Snake::move(double deltaT, char new_direction)
    {
        if (std::abs(new_direction) != mDirectionAbs) // Same or opposite direction
        {
            addPiece();
            mDirection = new_direction;
            mDirectionAbs = std::abs(mDirection);
        }
        int deltaXY = static_cast<int>(deltaT * mSpeed);
        int result{};
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
        }
        return result;
    }
    void Snake::Snake(const int x, const int y, const int width, const int height, const int length_factor, const char direction)
    {
        SDL_Rect init_piece{};
        mHeight = height;
        mWidth = width;
        if (direction == 1)
        {
            throw std::runtime_error("Must not move right initially")
        }
        swith(std::abs(direction))
        {
        case 1:
            init_piece.w = mWidth * length_factor;
            init_piece.h = mHeight;
            break;
        case 2:
            init_piece.w = mWidth;
            init_piece.h = mHeight * length_factor;
            break;
        }
        init_piece.w = mWidth;
        init_piece.h = mHeight;
        init_piece.x = x;
        init_piece.y = y;
        mPieces.push_back(init_piece);
        addPiece();
    }
    bool Snake::checkHitSelf()
    {
        int length = mPieces.size() - 2;
        bool result{};
        SDL_Rect * head = &(mPieces.back());
        SDL_Rect * penultimate = &(mPieces.at(mPenultimate));
        std::for_each_n(mPieces.cbegin(), mPieces.size() - 2, [&](SDL_Rect rect) mutable
                        // clang-format off
                        {
                            SDL_bool head_hit = SDL_HasIntersection(&rect, head);
                            if (head_hit == SDL_TRUE){
                                result = true;
                                return;
                            } else {
                                SDL_bool penultimate_hit = SDL_HasIntersection(&rect, penultimate);
                                if (penultimate_hit == SDL_TRUE){
                                    result = true;
                                }
                            }
                        }
            // clang-format on 
            );
        return result;
    }
}
