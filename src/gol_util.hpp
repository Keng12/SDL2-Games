#ifndef GOL_UTIL_HPP
#define GOL_UTIL_HPP

#include <array>
#include <SDL2/SDL.h>

#include "sdl2_util/video.hpp"

namespace gol_util
{
    template <std::size_t n_cells>
    void next_state(const std::array<char, n_cells> &old_state, std::array<char, n_cells> &new_state, sdl2_util::Renderer &renderer, std::array<SDL_Rect, n_cells> &rect_array)
    {
        for (size_t i = 0; i < old_state.size(); i++)
        {
            int row = i / N_COLUMNS;
            int col = i % N_COLUMNS;
            if (old_state.at(i) == 1)
            { // Cell does not survive
                new_state.at(i) = 0;
                renderer.setRenderDrawColor("black");
            }
            else if (old_state.at(i) == 0)
            { // Cell is revived
                new_state.at(i) = 1;
                renderer.setRenderDrawColor("white");
            }
            renderer.renderFillRect(&rect_array.at(i)); // Fill rectangle with white color
        }
    }
}

#endif