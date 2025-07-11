#include "CPURenderer.h"
#include "../engconfig.h"
#include <iostream>
#include <vector>
#include <cstdint>

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
		Vertex3d v1 = mesh.vertices[mesh.indices[i]];
		Vertex3d v2 = mesh.vertices[mesh.indices[i+1]];
		Vertex3d v3 = mesh.vertices[mesh.indices[i+2]];
		drawTri(v1, v2, v3);
	}
}

void CPURenderer::drawScene(Scene& scene) 
{

}

void CPURenderer::drawTri(Vertex3d& v1, Vertex3d& v2, Vertex3d& v3)
{
	// Calculate Triangle colour; here we have O(n)=1 but for pixel shaders we have O(n)=n :/
	// 3D positions of triangle corners
	Position3d& loc1 = v1.position;
	Position3d& loc2 = v2.position;
	Position3d& loc3 = v3.position;

	Position3d edge1vec = loc2 - loc1;
	Position3d edge2vec = loc3 - loc1;

	Position3d normal = edge1vec.cross(edge2vec); // Find normal by cross product of 2 edge vectors
	normal.normalise(); // lol
	
	// Test for backface
	Position3d triCentre = (v1.position + v2.position + v3.position) / 3.0f;
	Position3d viewDir = (currentScene->currentCam->pos - triCentre);
	viewDir.normalise();
	//if (normal.dot(viewDir) <= 0.0f) return;
	
	float dot = normal.dot(lightNormal);

	// Valve diffuse shading: map dot to intesity 0.5->1 (i luv gaben)
	float value = 0.25f * dot + 0.75f;
	uint8_t intensity = static_cast<uint8_t>(value * 255);
	// Weird way of doing ARGB, but it works i guess
	uint32_t colour = (0xFF << 24) | (intensity << 16) | (intensity << 8) | intensity;

	Point2d p1(v1);
	Point2d p2(v2);
	Point2d p3(v3);
	Rect2d bb(v1, v2, v3);

	// Compute edge function coefficients (A, B, C) for each edge: Ax + By + C = 0
	int A12 = p1.y - p2.y;
	int B12 = p2.x - p1.x;
	int C12 = p1.x * p2.y - p2.x * p1.y;

	int A23 = p2.y - p3.y;
	int B23 = p3.x - p2.x;
	int C23 = p2.x * p3.y - p3.x * p2.y;

	int A31 = p3.y - p1.y;
	int B31 = p1.x - p3.x;
	int C31 = p3.x * p1.y - p1.x * p3.y;

	// Loop over bounding box
	for (int y = bb.min.y; y < bb.max.y; y++)
	{
		// Evaluate edge functions at start of row (x = bb.min.x, y = y)
		int w1 = A12 * bb.min.x + B12 * y + C12;
		int w2 = A23 * bb.min.x + B23 * y + C23;
		int w3 = A31 * bb.min.x + B31 * y + C31;

		// These are the deltas for each edge as we increment x by 1
		int dw1 = A12;
		int dw2 = A23;
		int dw3 = A31;

		for (int x = bb.min.x; x < bb.max.x; x++)
		{
			// Check if point is inside triangle (all edge values >= 0)
			if (w1 >= 0 && w2 >= 0 && w3 >= 0)
			{
				SetPixel(x, y, colour);
			}

			w1 += dw1;
			w2 += dw2;
			w3 += dw3;
		}
	}
}
void CPURenderer::Present()
{
	SDL_UpdateTexture(texture, nullptr, pixelBuffer.data(), width * sizeof(uint32_t));
	SDL_RenderCopy(sdlRenderer, texture, nullptr, nullptr);
	SDL_RenderPresent(sdlRenderer);
}
