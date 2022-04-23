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

#include <string>
#include <iostream>

#include "sdl2_util.hpp"

namespace sdl2_util
{
    static SDL_Window *window{};
    static SDL_Renderer *renderer{};
    static SDL_Texture *texture{};
    static int result{};

    void initSDL(const uint32_t flags)
    {
        result = SDL_Init(flags); // Initialize SDL2
        if (0 != result)
        {
            std::cerr << "Error initialising SDL2: " << SDL_GetError() << '\n';
            std::terminate();
        }
    }

    SDL_Rect initRect(const int x, const int y, const int width, const int height)
    {
        SDL_Rect rect{};
        rect.x = x;
        rect.y = y;
        rect.w = width;
        rect.h = height;
        return rect;
    }

    void createWindow(const std::string &title, const int x, const int y, const int w, const int h, const uint32_t flags)
    {
        if (!window)
        {
            window = SDL_CreateWindow(title.c_str(),
                                      x, y, w,
                                      h, flags);
        }
        if (!window)
        {
            std::cerr << "Creating window failed with error: " << SDL_GetError() << '\n';
            std::terminate();
        }
    }

    void destroyWindow()
    {
        if (window)
        {
            SDL_DestroyWindow(window);
        }
    }
    void createRenderer(const int index, const uint32_t flags)
    {
        if (!renderer)
        {
            renderer = SDL_CreateRenderer(window, index, flags);
        }
        if (!renderer)
        {
            std::cerr << "Creating renderer failed with error: " << SDL_GetError() << '\n';
            std::terminate();
        }
    }
    void destroyRenderer()
    {
        if (renderer)
        {
            SDL_DestroyRenderer(renderer);
        }
    }

    void createTexture(const uint32_t format, const int access, const int w, const int h)
    {
        texture = SDL_CreateTexture(renderer, format, access, w, h);
        if (!texture)
        {
            std::cerr << "Creating texture failed with error: " << SDL_GetError() << '\n';
            std::terminate();
        }
    }

    void destroyTexture()
    {
        if (texture)
        {
            SDL_DestroyTexture(texture);
        }
    }

    void renderClear(const RenderColor color)
    {
        setRenderDrawColor(color);
        result = SDL_RenderClear(renderer); // Clear to black screen
        if (0 != result)
        {
            std::cerr << "Failure clearing render: " << SDL_GetError() << '\n';
            std::terminate();
        }
    }
    void present(const RenderColor color)
    {

        setRenderTarget(nullptr);
        result = SDL_RenderCopy(renderer, texture, nullptr, nullptr);
        if (0 != result)
        {
            std::cerr << "Failure copying texture : " << SDL_GetError() << '\n';
            std::terminate();
        }
        SDL_RenderPresent(renderer);
        setRenderTarget(texture);
        renderClear(color);
    }
    void setRenderDrawColor(const RenderColor color)
    {
        if (color == RenderColor::white)
        {
            result = SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
        }
        else if (color == RenderColor::black)
        {
            result = SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        }
        else if (color == RenderColor::green)
        {
            result = SDL_SetRenderDrawColor(renderer, 0, 255, 0, 0);
        }
        else
        {
            std::cerr << "Unknown color for setRenderDrawColor\n";
            std::terminate();
        }
        if (0 != result)
        {
            std::cerr << "Failure setting render color: " << SDL_GetError() << '\n';
            std::terminate();
        }
    }
    void fillRect(const SDL_Rect *rect)
    {
        result = SDL_RenderFillRect(renderer, rect);
        if (0 != result)
        {
            std::cerr << "Failure filling rectangle: " << SDL_GetError() << '\n';
            std::terminate();
        }
    }

    void setRenderTarget(SDL_Texture *l_texture)
    {
        result = SDL_SetRenderTarget(renderer, l_texture);
        if (0 != result)
        {
            std::cerr << "Failure setting render target: " << SDL_GetError() << '\n';
            std::terminate();
        }
    }

    void setRenderTexture()
    {
        setRenderTarget(texture);
    }

    void quitSDL()
    {
        destroyRenderer();
        destroyWindow();
        destroyTexture();
        SDL_Quit();
    }
}