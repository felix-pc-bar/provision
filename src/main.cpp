#include <SDL.h>
#include <iostream>

int main(int argc, char* argv[]) {
    if (SDL_Init(0) != 0) {
        std::cout << "SDL failed to init: " << SDL_GetError() << std::endl;
        return 1;
    }

    std::cout << "SDL initialized, cout works" << std::endl;
    SDL_Quit();
    return 0;
}
