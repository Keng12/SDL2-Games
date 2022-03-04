#ifndef VIDEO_HPP
#define VIDEO_HPP

#include <SDL2/SDL.h>

namespace sdl2_util
{
    class Window
    {
        SDL_Window *mWindow{};

    public:
        Window(const std::string &title, const int x, const int y, const int w, const int h,  const unsigned long flags=0);
        ~Window();
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
        void presentTexture(SDL_Texture * texture);
    };
    class Texture
    {
        SDL_Texture *mTexture{};

    public:
        Texture(SDL_Renderer *renderer,
                const unsigned long format,
                const int access,const int width,
                const int height);
        ~Texture();
        operator SDL_Texture *() const { return mTexture; }
    };
}

#endif