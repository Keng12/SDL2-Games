#ifndef SDL_UTIL_HPP
#define SDL_UTIL_HPP

#include <SDL2/SDL.h>

namespace sdl2_util
{
    void initSDL(const uint32_t flags);
    SDL_Rect initRect(int x, int y, int width, int height);
    void createWindow(const std::string &title, const int x, const int y, const int w, const int h, const uint32_t flags);
    void destroyWindow();
    void quitSDL();
    void createRenderer(const int index, const uint32_t flags);
    void destroyRenderer();
    void renderClear(const std::string &color);
    void present(const std::string &color);
    void setRenderDrawColor(const std::string &color);
    inline void setLiveColor() { setRenderDrawColor("white"); }
    inline void setDeadColor() { setRenderDrawColor("black"); }
    void fillRect(const SDL_Rect *rect);
}
#endif