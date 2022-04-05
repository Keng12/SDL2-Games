#include <string>
#include <iostream>

#include "video.hpp"

static int result{};

namespace sdl2_util
{
    SDL_Rect initRect(const int x, const int y, const int width, const int height)
    {
        SDL_Rect rect{};
        rect.x = x;
        rect.y = y;
        rect.w = width;
        rect.h = height;
        return rect;
    }

    SDL_Window *createWindow(const std::string &title, const int x, const int y, const int w, const int h, const uint32_t flags)
    {
        SDL_Window *window = SDL_CreateWindow(title.c_str(),
                                              x, y, w,
                                              h, flags);
        if (!window)
        {
            std::cerr << "Creating window failed with error: " << SDL_GetError() << '\n';
            std::terminate();
        }
        return window;
    }

    void destroyWindow(SDL_Window *window)
    {
        if (window)
        {
            SDL_DestroyWindow(window);
        }
    }
    SDL_Renderer* createRenderer(SDL_Window *window,
                       const int index, const uint32_t flags)
    {
        SDL_Renderer* renderer = SDL_CreateRenderer(window, index, flags);
        if (nullptr == renderer)
        {
            std::cerr << "Creating renderer failed with error: " << SDL_GetError() << '\n';
            std::terminate();
        }
        return renderer;
    }
    void destroyRenderer(SDL_Renderer* renderer)
    {
        if (renderer)
        {
            SDL_DestroyRenderer(renderer);
        }
    }
    void renderClear(SDL_Renderer* renderer, const std::string &color)
    {
        setRenderDrawColor(renderer, color);
        result = SDL_RenderClear(renderer); // Clear to black screen
        if (0 != result)
        {
            std::cerr << "Failure clearing render: " << SDL_GetError() << '\n';
            std::terminate();
        }
    }
    void present(SDL_Renderer* renderer, const std::string &color)
    {
        SDL_RenderPresent(renderer);
        renderClear(renderer, color);
    }
    void setRenderDrawColor(SDL_Renderer* renderer, const std::string &color)
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
        else{
            std::cerr<<"Error in setRenderDrawColor: Color " << color << " unknown \n";
            std::terminate();
        }
        if (0 != result)
        {
            std::cerr << "Failure setting render color: " << SDL_GetError() << '\n';
            std::terminate();
        }
    }
    void fillRect(SDL_Renderer* renderer, const SDL_Rect *rect)
    {
        result = SDL_RenderFillRect(renderer, rect);
        if (0 != result)
        {
            std::cerr << "Failure filling rectangle: " << SDL_GetError() << '\n';
            std::terminate();
        }
    }
    void quitSDL(SDL_Window *window, SDL_Renderer * renderer){
        destroyRenderer(renderer);
        destroyWindow(window);
        SDL_Quit();
    }
}