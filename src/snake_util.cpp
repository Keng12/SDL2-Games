#include "snake_util.hpp"

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
            mPieces.back().w = pieces.back().w + deltaXY;
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
            mPieces.at(mPenultimate).w = mPieces.at(penultimate).w + deltaXY;
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
            mPieces.back().h = mPieces.back().h + deltaXY;
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
    int Snake::addPiece(){
        SDL_Rect new_piece{};
        new_piece.x = mPieces.back().x;
        new_piece.y = mPieces.back().y;
        new_piece.w = mWidth;
        new_piece.h = mHeight;
    }
    int Snake::move(double deltaT)
    {
        int deltaXY = static_cast<int>(deltaT * mSpeed);
    }
}