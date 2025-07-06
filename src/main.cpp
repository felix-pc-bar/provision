#include <iostream>
#include <SDL.h>
#include <cstdint>
#include <vector>
#include "render/CPURenderer.h"

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

	system("pause");

	SDL_DestroyWindow(window);

	SDL_Quit();

	return 0;
}