#include "CPURenderer.h"
#include <iostream>

using std::endl, std::cout;

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
	y = 1080 - y;
	if (x >= 0 && x < width && y >= 0 && y < height)
		pixelBuffer[y * width + x] = color;
}

void CPURenderer::drawMesh(Mesh& mesh) 
{
	//int numTris = (mesh.indices.size() + 1) / 3;
	for (int i = 0;i < mesh.indices.size();i+=3)
	{
		Vertex3d v1 = mesh.vertices.at(mesh.indices.at(i));
		Vertex3d v2 = mesh.vertices.at(mesh.indices.at(i+1));
		Vertex3d v3 = mesh.vertices.at(mesh.indices.at(i+2));
		drawTri(v1, v2, v3);
	}
}
void CPURenderer::drawTri(Vertex3d& v1, Vertex3d& v2, Vertex3d& v3)
{
	cout << "drawing tri:" << v1.position << v2.position << v3.position;
	Point2d p1(v1);
	Point2d p2(v2);
	Point2d p3(v3);
	bool valid1, valid2, valid3 = false; //Here we store the side of the point from each edge vector; false for negative edgeSideSS() return, and true for positive.
	for (int y = 0; y < 1080; y++)
	{
		for (int x = 0; x < 1920; x++)
		{
			valid1 = (edgeSideSS(p1, p2, Point2d{ x,y }) > 0) ? true : false;
			valid2 = (edgeSideSS(p2, p3, Point2d{ x,y }) > 0) ? true : false;
			valid3 = (edgeSideSS(p3, p1, Point2d{ x,y }) > 0) ? true : false;
			if (valid1 == valid2 && valid2 == valid3) { SetPixel(x, y, 0xFF20FF00); }
		}
	}
}
void CPURenderer::Present() 
{
	SDL_UpdateTexture(texture, nullptr, pixelBuffer.data(), width * sizeof(uint32_t));
	SDL_RenderCopy(sdlRenderer, texture, nullptr, nullptr);
	SDL_RenderPresent(sdlRenderer);
}
