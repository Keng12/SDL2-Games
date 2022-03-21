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
    void Renderer::setRenderTarget(SDL_Texture *texture)
    {
        int result = SDL_SetRenderTarget(mRenderer, texture);
        if (result != 0)
        {
            throw std::runtime_error("Failure setting render target: " + std::string{SDL_GetError()});
        }
    }
    void Renderer::renderCopy(SDL_Texture *texture)
    {
        int result = SDL_RenderCopy(mRenderer, texture, nullptr, nullptr);
        if (result != 0)
        {
            throw std::runtime_error("Failure copying texture: " + std::string{SDL_GetError()});
        }
    }
    void Renderer::renderClear(const std::string &color)
    {
        setRenderDrawColor(color);
        int result = SDL_RenderClear(mRenderer); // Clear to black screen
        if (result != 0)
        {
            throw std::runtime_error("Failure clearing render: " + std::string{SDL_GetError()});
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
            throw std::runtime_error("Failure setting render color: " + std::string{SDL_GetError()});
        }
    }
    void Renderer::fillRect(const SDL_Rect *rect)
    {
        int result = SDL_RenderFillRect(mRenderer, rect);
        if (result != 0)
        {
            throw std::runtime_error("Failure filling rectangle: " + std::string{SDL_GetError()});
        }
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