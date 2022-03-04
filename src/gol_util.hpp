#ifndef GOL_UTIL_HPP
#define GOL_UTIL_HPP

#include <array>

#include <SDL2/SDL.h>

namespace gol_util {
    template <std::size_t n_cells>
    void next_state(const std::array<bool, n_cells> & old_state, std::array<bool, n_cells>& new_state, SDL_Renderer* renderer, std::array<SDL_Rect, n_cells> &rect){
        for (size_t i=0; i < old_state.size(); i++){
            
        }
    }
}

#endif