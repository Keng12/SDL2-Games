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

#ifndef SDL_UTIL_HPP
#define SDL_UTIL_HPP

#include <SDL2/SDL.h>

namespace sdl2_util
{   
    enum class RenderColor{white, black, green};
    void initSDL(const uint32_t flags);
    SDL_Rect initRect(int x, int y, int width, int height);
    void createWindow(const std::string &title, const int x, const int y, const int w, const int h, const uint32_t flags);
    void destroyWindow();
    void quitSDL();
    void createRenderer(const int index, const uint32_t flags);
    void destroyRenderer();
    void renderClear(const RenderColor color);
    void present(const RenderColor color);
    void setRenderDrawColor(const RenderColor color);
    void fillRect(const SDL_Rect *rect);
}
#endif