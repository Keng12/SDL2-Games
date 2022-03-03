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
    Renderer::Renderer(SDL_Window *window,
                       int index, unsigned long flags)
    {
        mRenderer = SDL_CreateRenderer(window, index, flags);
        if (nullptr == mRenderer)
        {
            throw std::runtime_error("Creating renderer failed with error: " + std::string{SDL_GetError()});
        }
    }
    Renderer::~Renderer()
    {
        if (mRenderer != nullptr)
        {
            SDL_DestroyRenderer(mRenderer);
        }
    }
    Texture::Texture(SDL_Renderer *renderer,
                     unsigned long format,
                     int access, int width,
                     int height)
    {
        mTexture = SDL_CreateTexture(renderer, format, access, width,
                                     height);
        if (nullptr == mTexture)
        {
            throw std::runtime_error("Creating texture failed with error: " + std::string{SDL_GetError()});
        }
    }
    Texture::~Texture()
    {
        if (mTexture != nullptr)
        {
            SDL_DestroyTexture(mTexture);
        }
    }
}