#pragma once
#include <SDL.h>
#include <vector>
#include <cstdint>

class CPURenderer {
public:
    CPURenderer(SDL_Renderer* renderer, int width, int height); //constructer
    ~CPURenderer();

    void Clear(uint32_t color);
    void SetPixel(int x, int y, uint32_t color);
    void Present(); // push pixels to texture and draw to screen

private:
    SDL_Renderer* sdlRenderer;
    SDL_Texture* texture;
    std::vector<uint32_t> pixelBuffer;
    int width;
    int height;
};
