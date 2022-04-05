#ifndef VIDEO_HPP
#define VIDEO_HPP

#include <SDL2/SDL.h>

namespace sdl2_util
{
    SDL_Rect initRect(int x, int y, int width, int height);
    SDL_Window *createWindow(const std::string &title, const int x, const int y, const int w, const int h, const uint32_t flags);
    void destroyWindow(SDL_Window *window);
    SDL_Renderer *createRenderer(SDL_Window *window,
                                 const int index = -1, const uint32_t flags = 0);
    void destroyRenderer(SDL_Renderer * renderer);
    void renderClear(SDL_Renderer * renderer, const std::string &color);
    void present(SDL_Renderer * renderer, const std::string &color);
    void setRenderDrawColor(SDL_Renderer * renderer, const std::string &color);
    inline void setLiveColor(SDL_Renderer * renderer) { setRenderDrawColor(renderer, "white"); }
    inline void setDeadColor(SDL_Renderer * renderer) { setRenderDrawColor(renderer, "black"); }
    void fillRect(SDL_Renderer * renderer, const SDL_Rect *rect);
    void quitSDL(SDL_Window *window, SDL_Renderer * renderer);
}
#endif