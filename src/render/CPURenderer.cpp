#include "CPURenderer.h"

CPURenderer::CPURenderer(SDL_Renderer* renderer, int w, int h): sdlRenderer(renderer), width(w), height(h) //Member initialiser list; constructer sets ob vars 
{
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);
    pixelBuffer.resize(width * height, 0xFF000000); // opaque black
}

CPURenderer::~CPURenderer() 
{
    SDL_DestroyTexture(texture);
}

void CPURenderer::Clear(uint32_t color) 
{
    std::fill(pixelBuffer.begin(), pixelBuffer.end(), color);
}

void CPURenderer::SetPixel(int x, int y, uint32_t color)  //translate to 1D array co-ords
{
    if (x >= 0 && x < width && y >= 0 && y < height)
        pixelBuffer[y * width + x] = color;
}

void CPURenderer::Present() 
{
    SDL_UpdateTexture(texture, nullptr, pixelBuffer.data(), width * sizeof(uint32_t));
    SDL_RenderCopy(sdlRenderer, texture, nullptr, nullptr);
}
