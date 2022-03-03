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
        operator SDL_Window *() const { return mWindow; }
    };
    class Renderer
    {
        SDL_Renderer *mRenderer{};

    public:
        Renderer(SDL_Window *window,
                 int index = -1, unsigned long flags = 0);
        ~Renderer();
        operator SDL_Renderer *() const { return mRenderer; }
    };
    class Texture
    {
        SDL_Texture *mTexture{};

    public:
        Texture(SDL_Renderer *renderer,
                unsigned long format,
                int access, int width,
                int height);
        ~Texture();
        operator SDL_Texture *() const { return mTexture; }
    };
}

#endif