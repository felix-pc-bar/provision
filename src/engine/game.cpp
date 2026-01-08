#include <iostream>
#include <vector>
#include <cstdlib>
#include <chrono>
#include <cmath>

#include <SDL_events.h>
#include <SDL_keyboard.h>
#include <SDL_scancode.h>
#include <SDL_stdinc.h>
#include <SDL_timer.h>

#include "import3d.h"
#include "engconfig.h"
#include "game.h"
#include "engTools.h"
#include "render/render.h"

using std::endl, std::cout;
using dtclock = std::chrono::steady_clock;

Game::Game()
{
	this->renderer = new cRenderer();
}

void Game::run()
{
	const Uint8* gk; // Used to read off inputs
	SDL_Event event; // SDL event buffer


	// ====
	// TIME
	// ====

	int frame = 0;
	auto lastTime = dtclock::now();
	int fpsLimit = 0;
	float fpsLimTick = 1.0f / fpsLimit;
	float gameTime = 0.0f; //Time since game start, use for framerate independent motion eg trig anim
	float dtFac = 1.0f; // dt as ratio; shouldn't change anything if you multiply with it and you're running at 60fps

	// ===========
	// SCENE SETUP
	// ===========

	Scene mainScene; //Create main scene
	currentScene = &mainScene; // Set the current scene to mainScene
	
	mainScene.cams.emplace_back(); // Add a camera to mainScene
	mainScene.currentCam = &mainScene.cams[0]; // Set mainScene's current camera to the camera we just created

	// Add the ground points object
	mainScene.objects.emplace_back();
	mainScene.objects[0].materials.emplace_back(0.5f, 0.5f, 0.5f, 3, false);
	mainScene.objects[0].name = mainScene.getName("Ground plane points");

	const int gridSize = 100;
	for (int x = 0; x < gridSize; x++)
	{
		for (int y = 0; y < gridSize; y++)
		{
			mainScene.objects[0].points.emplace_back(x - gridSize/2, 0, y - gridSize/2);
		}
	}

	mainScene.addObject(importObj("content/obj/raven.obj"));
	mainScene.objects[1].materials.emplace_back(0.15f, 0.15f, 0.15f);
	mainScene.addObject(importObj("content/obj/fTerrain.obj"));
	mainScene.objects[2].materials.emplace_back(1.0f, 0.7f, 0.5f);
	mainScene.addObject(importObj("content/obj/fCacti.obj"));
	mainScene.objects[3].materials.emplace_back(0.25f, 0.95f, 0.3f);
	mainScene.addObject(importObj("content/obj/fRocks.obj"));
	mainScene.objects[4].materials.emplace_back(0.33f, 0.33f, 0.33f);
	mainScene.addObject(importObj("content/obj/credit.obj"));
	mainScene.objects[5].materials.emplace_back(0.8f, 0.8f, 0.5f);
	mainScene.addObject(importObj("content/obj/godray.obj"));
	mainScene.objects[6].materials.emplace_back(1.0f, 1.0f, 1.0f, 0.5f);

	mainScene.objects[1].mesh->move({ -100,40,0 });

	Quaternion qDelta(pi / 200, { 0,1,0 });
	Quaternion camLookOffset(0.2f, { 1, 0, 0 }); // no rotation, or maybe slight pitch down if needed
	mainScene.cams[0].quatIdentity = mainScene.objects[1].mesh->quatIdentity * camLookOffset;
	float freecamspeedbase = 0.01f;
	float freecamspeed = 0.0f;
	currentScene->currentCam->pos = currentScene->currentCam->pos - Position3d(0, 0, 10);

	float flightSpeed = 0.2f;
	float roll = 0.0f;
	float pitch = 0.0f;

	do
	{
		auto currentTime = dtclock::now();
		std::chrono::duration<float> elapsed = currentTime - lastTime;
		float dt = elapsed.count(); // Raw frametime (s)
		gameTime += dt;
		if (fpsLimit != 0 && dt < fpsLimTick)
		{
			SDL_Delay((fpsLimTick - dt) * 1000);
			dt += fpsLimTick - dt;
			currentTime = dtclock::now();
		}
		float dtMulti = dt / (1.0f / fpsTarget);
		lastTime = currentTime;
		float fps = 1.0f / dt;
		dtFac = dt * 60.0f;
		if (frame % 10 == 0)
		{
			cout << fps << endl;
		}

		// Handle inputs
		mainScene.cams[0].calcBaseVecs();
		gk = SDL_GetKeyboardState(NULL); 
		while (SDL_PollEvent(&event)){
			if (event.type == SDL_QUIT || gk[SDL_SCANCODE_ESCAPE]) {
				break;  
			}
			if (event.type == SDL_MOUSEMOTION && freecam)
			{
				mainScene.cams[0].rotateCam((float)event.motion.xrel / 1000.0f, { 0,1,0 });
				mainScene.cams[0].rotateCam((float)event.motion.yrel / 1000.0f, mainScene.cams[0].right);
			}
			if (event.type == SDL_MOUSEWHEEL && freecam)
			{
				if (event.wheel.y > 0) { freecamspeedbase += 0.01f; }
				if (event.wheel.y < 0 && freecamspeedbase > 0.01f) { freecamspeedbase -= 0.01f; }
			}
			if (event.type == SDL_KEYDOWN && event.key.repeat == 0)
			{
				// Keypress events here
				if (event.key.keysym.sym == SDLK_f) {drawPoints = !drawPoints;}
				if (event.key.keysym.sym == SDLK_v) {wireframe = !wireframe;}
				if (event.key.keysym.sym == SDLK_c) {freecam = !freecam;}
			}
		}
		if (freecam)
		{
			// Key hold events here
			if (gk[SDL_SCANCODE_LSHIFT]) { freecamspeed = freecamspeedbase * 5 * dtFac; }
			else { freecamspeed = freecamspeedbase * dtFac; }
			if (gk[SDL_SCANCODE_W]) { mainScene.cams[0].pos += mainScene.cams[0].forward * freecamspeed; }
			if (gk[SDL_SCANCODE_S]) { mainScene.cams[0].pos -= mainScene.cams[0].forward * freecamspeed; }
			if (gk[SDL_SCANCODE_D]) { mainScene.cams[0].pos += mainScene.cams[0].right * freecamspeed; }
			if (gk[SDL_SCANCODE_A]) { mainScene.cams[0].pos -= mainScene.cams[0].right * freecamspeed; }
			if (gk[SDL_SCANCODE_E]) { mainScene.cams[0].pos += mainScene.cams[0].up * freecamspeed; }
			if (gk[SDL_SCANCODE_Q]) { mainScene.cams[0].pos -= mainScene.cams[0].up * freecamspeed; }
		}
		else
		{
			mainScene.objects[1].mesh->calcBaseVecs();
			float turnSpeed = 0.05f * dtMulti;

			if (gk[SDL_SCANCODE_A]) 
			{
				if (roll <= 0.32f) { mainScene.objects[1].mesh->rotateAxis(turnSpeed, { mainScene.objects[1].mesh->forward }); roll += turnSpeed; }
			}
			else if (gk[SDL_SCANCODE_D]) 
			{
				if (roll >= -0.32f) { mainScene.objects[1].mesh->rotateAxis(-turnSpeed, { mainScene.objects[1].mesh->forward }); roll -= turnSpeed; }
			}
			else if (roll <= 0.1f && roll > 0.0f) // reset from left roll to level
			{
				float rsSpeed = std::max(turnSpeed, 0.0f - roll);
				mainScene.objects[1].mesh->rotateAxis(-rsSpeed, { mainScene.objects[1].mesh->forward }); 
				roll -= rsSpeed;
			}
			else if (roll >= -0.1f && roll < 0.0f)
			{
				float rsSpeed = std::min(turnSpeed, 0.0f - roll);
				mainScene.objects[1].mesh->rotateAxis(rsSpeed, { mainScene.objects[1].mesh->forward }); 
				roll += rsSpeed;
			}

			mainScene.objects[1].mesh->rotateAxis((roll * dtMulti) / -15.0f, { 0, 1, 0 }); 

			if (gk[SDL_SCANCODE_S] && pitch >= -0.32f) 
			{	
				mainScene.objects[1].mesh->rotateAxis(-turnSpeed / 2.0f, {mainScene.objects[1].mesh->right}); 
				pitch -= turnSpeed / 2.0f;
			}

			if (gk[SDL_SCANCODE_W] && pitch <= 0.80f) 
			{	
				mainScene.objects[1].mesh->rotateAxis(turnSpeed / 2.0f, {mainScene.objects[1].mesh->right}); 
				pitch += turnSpeed / 2.0f;
			}

			flightSpeed = (pitch + 0.8f) * 1.2f;

			mainScene.cams[0].quatIdentity = mainScene.objects[1].mesh->quatIdentity * camLookOffset;

			mainScene.objects[1].mesh->move(mainScene.objects[1].mesh->forward * flightSpeed * dtMulti);

			Position3d camOffset(0, 4, -10);
			camOffset.rotateQuat(mainScene.objects[1].mesh->quatIdentity);
			mainScene.cams[0].pos = mainScene.objects[1].mesh->position + camOffset;
		}
		this->renderer->renderScene(*currentScene);
		frame++;
	}
	while (event.type != SDL_QUIT && !gk[SDL_SCANCODE_ESCAPE]);

	return;
	// lol, lmao even
	system("pause");

	//SDL_Quit();

	return;
} 

Game::~Game()
{
	//SDL_DestroyTexture(this->screenTexture);
}
