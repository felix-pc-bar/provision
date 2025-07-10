#include <iostream>
#include <SDL.h>
#include <cstdint>
#include <vector>
#include "render/CPURenderer.h"
#include "import3d.h"

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

	result = SDL_CreateWindowAndRenderer(1920, 1080, SDL_WINDOW_FULLSCREEN_DESKTOP, &window, &mainRenderer);
	if (result < 0)
	{
		cout << "Error creating window and renderer: " << SDL_GetError() << endl;
		return 1;
	}
	SDL_SetWindowTitle(window, "Barbershop");
	
	CPURenderer vp(mainRenderer, 1920, 1080);	
	Camera mainCam;
	currentCam = &mainCam;
	Mesh triangleMesh;
	Mesh cube = importObj("F:/Creative raw/repos/barbershop/content/obj/suzanne.obj");
	const Uint8* gk; 
	SDL_Event event;
	
	while (true)
	{
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				return 0;  // or set a `running` flag to false
			}
		}
		// Handle inputs
		gk = SDL_GetKeyboardState(NULL); 
		if (gk[SDL_SCANCODE_W]) { mainCam.pos.y += 0.05f; }
		if (gk[SDL_SCANCODE_S]) { mainCam.pos.y -= 0.05f; }
		if (gk[SDL_SCANCODE_A]) { mainCam.pos.x -= 0.05f; }
		if (gk[SDL_SCANCODE_D]) { mainCam.pos.x += 0.05f; }
		vp.drawMesh(cube);
		vp.Present();
		//triangleMesh.move(Position3d{ 0.01f,0.0f,0.0f });
		vp.Clear(0xFF000000);
	}

	system("pause");

	SDL_DestroyWindow(window);

	SDL_Quit();

	return 0;
}