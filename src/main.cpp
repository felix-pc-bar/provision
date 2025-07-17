#include <iostream>
#include <SDL.h>
#include <cstdint>
#include <vector>
#include <cmath>
#include "render/CPURenderer.h"
#include "import3d.h"
#include "engconfig.h"

using std::endl, std::cout;

int main(int argc, char** args) {

	// Pointers to our window and surface
	SDL_Surface* winSurface = NULL;
	SDL_Window* window = NULL;
	SDL_Renderer* mainRenderer = NULL;

	int result;
	
	// Initialize SDL. SDL_Init will return -1 if it fails.
	result = SDL_Init(SDL_INIT_EVERYTHING);
	if (result < 0) 
	{
		cout << "Error initializing SDL: " << SDL_GetError() << endl;
		system("pause");
		// End the program
		return 1;
	}

	result = SDL_CreateWindowAndRenderer(screenwidth, screenheight, SDL_WINDOW_FULLSCREEN_DESKTOP, &window, &mainRenderer);
	if (result < 0)
	{
		cout << "Error creating window and renderer: " << SDL_GetError() << endl;
		return 1;
	}
	SDL_SetWindowTitle(window, "Barbershop");
	
	// Init
	CPURenderer vp(mainRenderer, screenwidth, screenheight); // Create viewport
	Scene mainScene; //Create main scene
	mainScene.cams.emplace_back(); // Add a camera to mainScene
	currentScene = &mainScene; // Set the current scene to mainScene
	mainScene.currentCam = &mainScene.cams[0]; // Set mainScene's current camera to the camera we just created
	//mainScene.meshes.emplace_back(importObj("F:/Creative raw/repos/barbershop/content/obj/ico2.obj"));
	mainScene.meshes.emplace_back(importObj("F:/Creative raw/repos/barbershop/content/obj/sz2.obj"));
	//mainScene.meshes[1].move({ 0.5, 2, 0 });
	const Uint8* gk; 
	SDL_Event event;
	float freecamspeed;
	mainScene.cams[0].pos.z -= 2;
	mainScene.cams[0].rot.pitch += pi / 2;
	int frame = 0;
	cout << mainScene.cams[0].rot.pitch << mainScene.cams[0].rot.yaw << mainScene.cams[0].rot.roll << endl;

	while (true)
	{
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				return 0;  
			}
		}
		// Handle inputs

		mainScene.cams[0].calcBaseVecs();

		gk = SDL_GetKeyboardState(NULL); 
		if (gk[SDL_SCANCODE_LSHIFT]) { freecamspeed = 0.05f; }
		else { freecamspeed = 0.01f; }
		if (gk[SDL_SCANCODE_W]) { mainScene.cams[0].pos += mainScene.cams[0].forward * freecamspeed; }
		if (gk[SDL_SCANCODE_S]) { mainScene.cams[0].pos -= mainScene.cams[0].forward * freecamspeed; }
		if (gk[SDL_SCANCODE_A]) { mainScene.cams[0].pos += mainScene.cams[0].left * freecamspeed; }
		if (gk[SDL_SCANCODE_D]) { mainScene.cams[0].pos -= mainScene.cams[0].left * freecamspeed; }
		if (gk[SDL_SCANCODE_Q]) { mainScene.cams[0].pos += mainScene.cams[0].up * freecamspeed; }
		if (gk[SDL_SCANCODE_E]) { mainScene.cams[0].pos -= mainScene.cams[0].up * freecamspeed; }

		Rotation3d& camRot = mainScene.cams[0].rot;
		if (gk[SDL_SCANCODE_J]) mainScene.cams[0].rot.yaw += 0.05f; // yaw left
		if (gk[SDL_SCANCODE_L]) mainScene.cams[0].rot.yaw -= 0.05f; // yaw right
		if (gk[SDL_SCANCODE_I] && camRot.pitch <  pi - 0.05f) mainScene.cams[0].rot.pitch += 0.05f; // pitch up
		if (gk[SDL_SCANCODE_K] && camRot.pitch > 0.05f) mainScene.cams[0].rot.pitch -= 0.05f; // pitch down


		//cout << mainScene.meshes[0].position.cameraspace() << endl;
		vp.drawScene(*currentScene);
		vp.Present();
		frame++;
		vp.Clear(0xFF000000);
		//mainScene.meshes[0].rotate({ 0.0f, 0.0f, 0.1f });
		//mainScene.meshes[0].setPos({ 0.0f, 0.0f, sin((float) frame / 10) });
	}

	system("pause");

	SDL_DestroyWindow(window);

	SDL_Quit();

	return 0;
}