#include <string>
#include <stdexcept>

#include "video.hpp"

namespace sdl2_util
{
    Window::Window(const std::string &title, const int x, const int y, const int w, const int h, const unsigned long flags)
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
                       const int index, const unsigned long flags)
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
    void Renderer::presentTexture(SDL_Texture *texture)
    {
        SDL_SetRenderTarget(mRenderer, nullptr);
        SDL_RenderCopy(mRenderer, texture, nullptr, nullptr);
        SDL_RenderPresent(mRenderer);
        SDL_RenderClear(mRenderer); // Clear to black screen
    }
    Texture::Texture(SDL_Renderer *renderer,
                     const unsigned long format,
                     const int access, const int width,
                     const int height)
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