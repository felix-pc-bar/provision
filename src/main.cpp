#include <iostream>
#include <SDL.h>
#include <cstdint>
#include <vector>
#include "render/CPURenderer.h"

using std::endl, std::cout;

// You must include the command line parameters for your main function to be recognized by SDL
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

	// Create our window
	//window = SDL_CreateWindow("Barbershop", 0, 0, 1920, 1080, SDL_WINDOW_FULLSCREEN_DESKTOP);
	// Make sure creating the window succeeded
	//if (!window) 
	//{
	//	cout << "Error creating window: " << SDL_GetError() << endl;
	//	system("pause");
	//	// End the program
	//	return 1;
	//}
	//
	result = SDL_CreateWindowAndRenderer(1920, 1080, SDL_WINDOW_FULLSCREEN_DESKTOP, &window, &mainRenderer);
	if (result < 0)
	{
		cout << "Error creating window and renderer: " << SDL_GetError() << endl;
		return 1;
	}
	SDL_SetWindowTitle(window, "Barbershop");
	//// Get the surface from the window
	//winSurface = SDL_GetWindowSurface(window);

	//// Make sure getting the surface succeeded
	//if (!winSurface) 
	//{
	//	cout << "Error getting surface: " << SDL_GetError() << endl;
	//	system("pause");
	//	// End the program
	//	return 1;
	//}

	//// Fill the window with a white rectangle
	//SDL_FillRect(winSurface, NULL, SDL_MapRGB(winSurface->format, 255, 255, 255));

	//// Update the window display
	//SDL_UpdateWindowSurface(window);

	// Wait
	system("pause");

	// Destroy the window. This will also destroy the surface
	SDL_DestroyWindow(window);

	// Quit SDL
	SDL_Quit();

	// End the program
	return 0;
}