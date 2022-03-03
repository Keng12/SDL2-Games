#ifndef VIDEO_HPP
#define VIDEO_HPP

#include <SDL2/SDL.h>

namespace sdl2_util
{
    class Window
    {
        SDL_Window *mWindow{};

    public:
        Window(const std::string &title, int x, int y, int w, int h, unsigned long flags);
        ~Window();
        get() { return mWindow; }
    };
    class Renderer
    {
        SDL_Renderer *mRenderer{};

    public:
        Renderer(const SDL_Window *window,
                 int index = -1, unsigned long flags = 0);
        ~Renderer();
        SDL_Window *get() { return mRenderer; }
    }
}

#endif