#include "CHIP8.h"
#include <SDL2/SDL.h>
#include <iostream>

int main(int argc, char* argv[]) {
    //Screen dimension constants
    const int SCREEN_WIDTH = 640;
    const int SCREEN_HEIGHT = 480;

    // The window we'll be rendering to
    SDL_Window* window = NULL;

    // The surface contained by the window
    SDL_Surface* screenSurface = NULL;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout <<  "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
    } else {
        // Create window
        window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (window == NULL) {
            std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        } else {
            // Get window surface
            screenSurface = SDL_GetWindowSurface(window);

            // Fill the surface black
            SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0x00, 0x00, 0x00));

            // Update the surface
            SDL_UpdateWindowSurface(window);

            // Wait two seconds
            SDL_Delay(2000);

            // Destrou window
            SDL_DestroyWindow(window);

            // Quit SDL subsystems
            SDL_Quit();
        }
    }

    return 0;
}