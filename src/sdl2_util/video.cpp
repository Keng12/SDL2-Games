#include <string>
#include <iostream>

#include "video.hpp"

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
    Window::Window(const std::string &title, const int x, const int y, const int w, const int h, const unsigned long flags)
    {
        mWindow = SDL_CreateWindow(title.c_str(),
                                   x, y, w,
                                   h, flags);
        if (nullptr == mWindow)
        {
            std::cerr << "Creating window failed with error: " << SDL_GetError() << '\n';
            std::terminate();
        }
    }
    Window::~Window()
    {
        DestroyWindow();
    }
    void Window::DestroyWindow()
    {
        if (mWindow != nullptr)
        {
            SDL_DestroyWindow(mWindow);
        }
    }
    Renderer::Renderer(SDL_Window *window,
                       const int index, const unsigned long flags)
    {
        mRenderer = SDL_CreateRenderer(window, index, flags);
        if (nullptr == mRenderer)
        {
            std::cerr << "Creating renderer failed with error: " << SDL_GetError() << '\n';
            std::terminate();
        }
    }
    Renderer::~Renderer()
    {
        DestroyRenderer();
    }
    void Renderer::DestroyRenderer()
    {
        if (mRenderer != nullptr)
        {
            SDL_DestroyRenderer(mRenderer);
        }
    }
    void Renderer::renderClear(const std::string &color)
    {
        setRenderDrawColor(color);
        int result = SDL_RenderClear(mRenderer); // Clear to black screen
        if (result != 0)
        {
            std::cerr << "Failure clearing render: " << SDL_GetError() << '\n';
            std::terminate();
        }
    }
    void Renderer::present(const std::string &color)
    {
        SDL_RenderPresent(mRenderer);
        renderClear(color);
    }
    void Renderer::setRenderDrawColor(const std::string &color)
    {
        int result{};
        if (color == "white")
        {
            result = SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 0);
        }
        else if (color == "black")
        {
            result = SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 0);
        }
        else if (color == "green")
        {
            result = SDL_SetRenderDrawColor(mRenderer, 0, 255, 0, 0);
        }
        if (result != 0)
        {
            std::cerr << "Failure setting render color: " << SDL_GetError() << '\n';
            std::terminate();
        }
    }
    void Renderer::fillRect(const SDL_Rect *rect)
    {
        int result = SDL_RenderFillRect(mRenderer, rect);
        if (result != 0)
        {
            std::cerr << "Failure filling rectangle: " << SDL_GetError() << '\n';
            std::terminate();
        }
    }
}