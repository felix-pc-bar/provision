#pragma once
#include <SDL.h>
#include <vector>
#include <cstdint>
//#include "../engTools.h" //Nasty
#include "../logic2d.h"

class CPURenderer {
public:
	CPURenderer(SDL_Renderer* renderer, int width, int height); //constructer
	~CPURenderer();

	void Clear(uint32_t color);
	void Present(); // push pixels to texture and draw to screen
	void drawMesh(Mesh& mesh);
	void drawTri(Vertex3d& v1,Vertex3d& v2,Vertex3d& v3); // Draw a triangle in screen space
private:
	SDL_Renderer* sdlRenderer;
	SDL_Texture* texture;
	std::vector<uint32_t> pixelBuffer;
	int width;
	int height;
	void SetPixel(int x, int y, uint32_t color);
};
