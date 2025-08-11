#include <iostream>
#include <SDL.h>
#include <cstdint>
#include <vector>
#include <cmath>
#include <filesystem>
#include "engine/render/CPURenderer.h"
#include "engine/import3d.h"
#include "engine/engconfig.h"
#include <chrono>

using std::endl, std::cout;
using dtclock = std::chrono::steady_clock;

int main(int argc, char** args) {
	// =========
	// SDL SETUP
	// =========

	// Pointers to our window and surface
	SDL_Surface* winSurface = NULL;
	SDL_Window* window = NULL;
	SDL_Renderer* mainRenderer = NULL;
	int result;
	result = SDL_Init(SDL_INIT_EVERYTHING);
	if (result < 0) 
	{
		cout << "Error initializing SDL: " << SDL_GetError() << endl;
		system("pause");
		return 1;
	}
	result = SDL_CreateWindowAndRenderer(screenwidth, screenheight, SDL_WINDOW_FULLSCREEN_DESKTOP, &window, &mainRenderer);
	if (result < 0)
	{
		cout << "Error creating window and renderer: " << SDL_GetError() << endl;
		return 1;
	}
	SDL_SetWindowTitle(window, "Provision");
	SDL_ShowCursor(SDL_DISABLE); // Hide cursor
	SDL_SetRelativeMouseMode(SDL_TRUE); // Lock cursor to window
	const Uint8* gk; // Used to read off inputs
	SDL_Event event; // SDL event buffer
	CPURenderer vp(mainRenderer, screenwidth, screenheight); // Create viewport
	vp.Clear(0xFF5792FF); // sky blue
	
	// ===========
	// SCENE SETUP
	// ===========

	Scene mainScene; //Create main scene
	currentScene = &mainScene; // Set the current scene to mainScene
	
	mainScene.cams.emplace_back(); // Add a camera to mainScene
	mainScene.currentCam = &mainScene.cams[0]; // Set mainScene's current camera to the camera we just created

	mainScene.addMesh(importObj("content/obj/raven.obj"));
	mainScene.meshes[0].materials.emplace_back(0.15f, 0.15f, 0.15f);
	mainScene.addMesh(importObj("content/obj/fTerrain.obj"));
	mainScene.meshes[1].materials.emplace_back(1.0f, 0.7f, 0.5f);
	mainScene.addMesh(importObj("content/obj/fCacti.obj"));
	mainScene.meshes[2].materials.emplace_back(0.25f, 0.95f, 0.3f);
	mainScene.addMesh(importObj("content/obj/fRocks.obj"));
	mainScene.meshes[3].materials.emplace_back(0.33f, 0.33f, 0.33f);
	mainScene.addMesh(importObj("content/obj/credit.obj"));
	mainScene.meshes[4].materials.emplace_back(0.8f, 0.8f, 0.5f);
	mainScene.addMesh(importObj("content/obj/godray.obj"));
	mainScene.meshes[5].materials.emplace_back(1.0f, 1.0f, 1.0f, 0.5f);

	mainScene.meshes[0].move({ -100,40,0 });

	// ==============
	// GAME VARIABLES
	// ==============

	Quaternion qDelta(pi / 200, { 0,1,0 });
	Quaternion camLookOffset(0.2f, { 1, 0, 0 }); // no rotation, or maybe slight pitch down if needed
	mainScene.cams[0].quatIdentity = mainScene.meshes[0].quatIdentity * camLookOffset;
	
	float flightSpeed = 0.2f;
	float turnSpeed = 0.02f;
	float roll = 0.0f;
	float pitch = 0.0f;

	// ====
	// TIME
	// ====

	int frame = 0;
	auto lastTime = dtclock::now();

	while (true)
	{
		auto currentTime = dtclock::now();
		std::chrono::duration<float> elapsed = currentTime - lastTime;
		float deltaTime = elapsed.count();
		lastTime = currentTime;
		float fps = 1.0f / deltaTime;
		cout << fps << endl;

		mainScene.meshes[0].calcBaseVecs();
		Rotation3d& camRot = mainScene.cams[0].rot;
		// Handle inputs
		mainScene.cams[0].calcBaseVecs();
		gk = SDL_GetKeyboardState(NULL); 
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || gk[SDL_SCANCODE_ESCAPE]) {
				return 0;  
			}
			if (event.type == SDL_MOUSEMOTION)
			{
				//mainScene.meshes[0].rotateAxis((float)event.motion.xrel / 1000.0f, mainScene.meshes[0].up);
				//mainScene.meshes[0].rotateAxis((float)event.motion.yrel / 1000.0f, mainScene.meshes[0].right);
				//mainScene.cams[0].quatIdentity = mainScene.meshes[0].quatIdentity * camLookOffset;
			}
		}

		if (gk[SDL_SCANCODE_A]) 
		{
			if (roll <= 0.32f) { mainScene.meshes[0].rotateAxis(turnSpeed, { mainScene.meshes[0].forward }); roll += turnSpeed; }
		}
		else if (gk[SDL_SCANCODE_D]) 
		{
			if (roll >= -0.32f) { mainScene.meshes[0].rotateAxis(-turnSpeed, { mainScene.meshes[0].forward }); roll -= turnSpeed; }
		}
		else if (roll <= 0.1f && roll > 0.0f) // reset from left roll to level
		{
			mainScene.meshes[0].rotateAxis(-turnSpeed, { mainScene.meshes[0].forward }); 
			roll -= turnSpeed;
		}
		else if (roll >= -0.1f && roll < 0.0f)
		{
			mainScene.meshes[0].rotateAxis(turnSpeed, { mainScene.meshes[0].forward }); 
			roll += turnSpeed;
		}

		mainScene.meshes[0].rotateAxis(roll / -30.0f, { 0, 1, 0 }); 

		if (gk[SDL_SCANCODE_S] && pitch >= -0.32f) 
		{	
			mainScene.meshes[0].rotateAxis(-turnSpeed, {mainScene.meshes[0].right}); 
			pitch -= turnSpeed;
		}

		if (gk[SDL_SCANCODE_W] && pitch <= 0.80f) 
		{	
			mainScene.meshes[0].rotateAxis(turnSpeed, {mainScene.meshes[0].right}); 
			pitch += turnSpeed;
		}

		flightSpeed = (pitch + 0.8f) / 2.0f;

		mainScene.cams[0].quatIdentity = mainScene.meshes[0].quatIdentity * camLookOffset;

		mainScene.meshes[0].move(mainScene.meshes[0].forward * flightSpeed);
		
		Position3d camOffset(0, 4, -10);
		camOffset.rotateQuat(mainScene.meshes[0].quatIdentity);
		mainScene.cams[0].pos = mainScene.meshes[0].position + camOffset;

		vp.drawScene(*currentScene);
		vp.Present();
		frame++;
		vp.Clear(0xFF5792FF); // sky blue
	}

	system("pause");

	SDL_DestroyWindow(window);

	SDL_Quit();

	return 0;
}