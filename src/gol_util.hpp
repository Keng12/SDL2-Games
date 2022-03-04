#ifndef GOL_UTIL_HPP
#define GOL_UTIL_HPP

#include <array>
#include <SDL2/SDL.h>

#include "sdl2_util/video.hpp"

namespace gol_util
{
    template <size_t n_row, size_t n_col>
    void next_state(const std::array<std::array<char, n_col + 2>, n_row + 2> &old_state, std::array<std::array<char, n_col + 2>, n_row + 2> &new_state, sdl2_util::Renderer &renderer, std::array<std::array<SDL_Rect, n_col>, n_row> &rect_array)
    {
        for (size_t row = 1; row < n_row + 1; row++)
        {
            for (size_t col = 1; col < n_col + 1; col++)
            {
                char live_neighbours = old_state.at(row - 1).at(col - 1) + old_state.at(row).at(col - 1) + old_state.at(row + 1).at(col - 1) + old_state.at(row + 1).at(col) + old_state.at(row + 1).at(col + 1) + old_state.at(row).at(col + 1) + old_state.at(row - 1).at(col + 1) + old_state.at(row - 1).at(col);
                if (old_state.at(row).at(col) == 1 && (live_neighbours == 2 || live_neighbours == 3))
                { // Cell survives, rerendered as it is cleared after each frame

                    new_state.at(row).at(col) = 1;
                    renderer.setRenderDrawColor("white");
                }
                else if (old_state.at(row).at(col) == 0 && (live_neighbours == 3))
                { // Cell is revived

                    new_state.at(row).at(col) = 1;
                    renderer.setRenderDrawColor("white");
                }
                else
                {
                    new_state.at(row).at(col) = 0;
                    renderer.setRenderDrawColor("black");
                }
                renderer.renderFillRect(&rect_array.at(row - 1).at(col - 1)); // Fill rectangle with white color
            }
        }
    }
}

#endif