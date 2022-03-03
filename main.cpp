#include <iostream>
#include <SDL2/SDL.h>

using namespace std;

int main()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
    {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return 1;
    }
    cout << "Hello World!" << endl;
    SDL_Quit();
    return 0;
}
