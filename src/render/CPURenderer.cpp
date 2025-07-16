#include "CPURenderer.h"
#include "../engconfig.h"
#include "../engTools.h"
#include <iostream>
#include <vector>
#include <cstdint>
#include <algorithm>

using std::endl, std::cout;

TriangleToRender::TriangleToRender(const Vertex3d& a, const Vertex3d& b, const Vertex3d& c, const Position3d& camPos) : v1(a), v2(b), v3(c)
	{
		Position3d center = (a.position + b.position + c.position) / 3.0f;
		Position3d diff = center - camPos;
		distanceToCamera = diff.lengthSquared(); // faster than .length()
	}


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

inline void CPURenderer::SetPixel(int x, int y, uint32_t color)
{
	int screenY = screenheight - y;
	if ((unsigned)x < (unsigned)width && (unsigned)screenY < (unsigned)height)
		pixelBuffer[screenY * width + x] = color;
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
	std::vector<TriangleToRender> triangles;

	if (!scene.currentCam) return;
	Position3d camPos = scene.currentCam->pos;

	for (Mesh& mesh : scene.meshes)
	{
		Position3d pos = mesh.position;
		Rotation3d rot = mesh.rotation;
		for (size_t i = 0; i + 2 < mesh.indices.size(); i += 3)
		{
			Vertex3d& v1 = mesh.vertices[mesh.indices[i]];
			Vertex3d& v2 = mesh.vertices[mesh.indices[i + 1]];
			Vertex3d& v3 = mesh.vertices[mesh.indices[i + 2]];
			//v1.offsetPosition(pos);
			//v2.offsetPosition(pos);
			//v3.offsetPosition(pos);
			//v1.rotatePosition(rot, pos);
			//v2.rotatePosition(rot, pos);
			//v3.rotatePosition(rot, pos);
			triangles.emplace_back(v1, v2, v3, camPos);
		}
	}

	// Depth occlusion is dodgy (I'm ignoring it)
	std::sort(triangles.begin(), triangles.end(),
		[](const TriangleToRender& a, const TriangleToRender& b) {
			return a.distanceToCamera > b.distanceToCamera;
		});

	// Draw
	for (TriangleToRender& tri : triangles)
	{
		drawTri(tri.v1, tri.v2, tri.v3);
	}
}

void CPURenderer::drawTri(Vertex3d& v1, Vertex3d& v2, Vertex3d& v3)
{
	Point2d p1(v1);
	Point2d p2(v2);
	Point2d p3(v3);
	
	// Cull tris behind the camera viewplane 
	// TODO: cull more agressively to frustrum?
	if (v1.position.cameraspace().z <= 0 && v2.position.cameraspace().z <= 0 && v3.position.cameraspace().z <= 0)
	{
		//cout << v1.position.cameraspace() << endl;
		return;
	}

	Position3d& loc1 = v1.position;
	Position3d& loc2 = v2.position;
	Position3d& loc3 = v3.position;

	Position3d edge1vec = loc2 - loc1;
	Position3d edge2vec = loc3 - loc1;
	Position3d normal = edge1vec.cross(edge2vec);
	normal.normalise();

	Position3d triCentre = (v1.position + v2.position + v3.position) / 3.0f;
	Position3d viewDir = (currentScene->currentCam->pos - triCentre);
	viewDir.normalise();

	float dot = normal.dot(lightNormal);
	float value = 0.25f * dot + 0.75f;
	uint8_t intensity = static_cast<uint8_t>(value * 255);
	uint32_t colour = (0xFF << 24) | (intensity << 16) | (intensity << 8) | intensity;

	if (!isTriangleOnScreen(p1, p2, p3, screenwidth, screenheight))
		return;

	Rect2d bb(v1, v2, v3);

	int A12 = p1.y - p2.y, B12 = p2.x - p1.x, C12 = p1.x * p2.y - p2.x * p1.y;
	int A23 = p2.y - p3.y, B23 = p3.x - p2.x, C23 = p2.x * p3.y - p3.x * p2.y;
	int A31 = p3.y - p1.y, B31 = p1.x - p3.x, C31 = p3.x * p1.y - p1.x * p3.y;

	uint32_t* pixels = pixelBuffer.data();

	for (int y = bb.min.y; y < bb.max.y; y++)
	{
		int flippedY = screenheight - y;
		if (flippedY < 0 || flippedY >= screenheight) continue;

		int baseIndex = flippedY * width;

		int w1 = A12 * bb.min.x + B12 * y + C12;
		int w2 = A23 * bb.min.x + B23 * y + C23;
		int w3 = A31 * bb.min.x + B31 * y + C31;

		int dw1 = A12, dw2 = A23, dw3 = A31;

		for (int x = bb.min.x; x < bb.max.x; x++)
		{
			if ((unsigned)x < (unsigned)width && w1 >= 0 && w2 >= 0 && w3 >= 0)
			{
				pixels[baseIndex + x] = colour;
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
