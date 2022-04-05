#ifndef VIDEO_HPP
#define VIDEO_HPP

#include <SDL2/SDL.h>

namespace sdl2_util
{
    SDL_Rect initRect(int x, int y, int width, int height);
    class Window
    {
        SDL_Window *mWindow{};

    public:
        Window() = default;
        Window(const std::string &title, const int x, const int y, const int w, const int h, const unsigned long flags = 0);
        ~Window();
        void DestroyWindow();
        operator SDL_Window *() const { return mWindow; }
    };
    class Renderer
    {
        SDL_Renderer *mRenderer{};

    public:
        Renderer(SDL_Window *window,
                 const int index = -1, const unsigned long flags = 0);
        ~Renderer();
        operator SDL_Renderer *() const { return mRenderer; }
        void renderClear(const std::string &color);
        void present(const std::string &color);
        void setRenderDrawColor(const std::string &color);
        void setLiveColor() { setRenderDrawColor("white"); };
        void setDeadColor() { setRenderDrawColor("black"); };
        void fillRect(const SDL_Rect *rect);
        void DestroyRenderer();
    };
}

#endif