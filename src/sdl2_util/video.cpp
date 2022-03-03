#include <string>
#include <stdexcept>

#include "video.hpp"

namespace sdl2_util
{
    Window::Window(const std::string &title, int x, int y, int w, int h, unsigned long flags)
    {
        mWindow = SDL_CreateWindow(title.c_str(),
                                   x, y, w,
                                   h, flags);
        if (nullptr == mWindow)
        {
            throw std::runtime_error("Creating window failed with error: " + std::string{SDL_GetError()});
        }
    }
    Window::~Window()
    {
        if (mWindow != nullptr)
        {
            SDL_DestroyWindow(mWindow);
        }
    }
    Renderer::Renderer(const SDL_Window *window,
                       int index, unsigned long flags)
    {
        mRenderer = SDL_CreateRenderer(window, index, flags);
        if (nullptr == mRenderer)
        {
            throw std::runtime_error("Creating renderer failed with error: " + std::string{SDL_GetError()});
        }
    }
}