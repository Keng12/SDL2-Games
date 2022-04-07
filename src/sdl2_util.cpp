#include <string>
#include <iostream>

#include "sdl2_util.hpp"

static int result{};

namespace sdl2_util
{
    static SDL_Window *window{};
    static SDL_Renderer *renderer{};
    static bool sdl_initialized = false;

    void initSDL(const uint32_t flags)
    {
        result = SDL_Init(flags); // Initialize SDL2
        if (0 != result)
        {
            std::cerr << "Error initialising SDL2: " << SDL_GetError() << '\n';
            std::terminate();
        }
        sdl_initialized = true;
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
        if (!renderer && sdl_initialized)
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
    void renderClear(const std::string &color)
    {
        setRenderDrawColor(color);
        result = SDL_RenderClear(renderer); // Clear to black screen
        if (0 != result)
        {
            std::cerr << "Failure clearing render: " << SDL_GetError() << '\n';
            std::terminate();
        }
    }
    void present(const std::string &color)
    {
        SDL_RenderPresent(renderer);
        renderClear(color);
    }
    void setRenderDrawColor(const std::string &color)
    {
        if (color == "white")
        {
            result = SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
        }
        else if (color == "black")
        {
            result = SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        }
        else if (color == "green")
        {
            result = SDL_SetRenderDrawColor(renderer, 0, 255, 0, 0);
        }
        else
        {
            std::cerr << "Unknown color for setRenderDrawColor: " << color << " unknown \n";
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
    void quitSDL()
    {
        destroyRenderer();
        destroyWindow();
        if (sdl_initialized)
        {
            SDL_Quit();
        }
    }
}