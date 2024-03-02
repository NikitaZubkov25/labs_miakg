#include "helpers.h"
#include "draw.h"

bool init(
    SDL_Window **gWindow,
    SDL_Renderer **gRenderer) {
    bool success = true;
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        return false;
    }
    *gWindow = SDL_CreateWindow("AOKG Lab 1", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (*gWindow == NULL) {
        printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
        return false;
    }
    *gRenderer = SDL_CreateRenderer(*gWindow, -1, SDL_RENDERER_SOFTWARE);
    if (*gRenderer == NULL) {
        printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
        return false;
    }
    SDL_SetRenderDrawColor(*gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    return success;
}

void close(SDL_Window **gWindow,
    SDL_Renderer **gRenderer,
    SDL_Texture **gTexture) {
    SDL_DestroyTexture(*gTexture);
    *gTexture = NULL;
    SDL_DestroyRenderer(*gRenderer);
    SDL_DestroyWindow(*gWindow);
    *gWindow = NULL;
    *gRenderer = NULL;
    SDL_Quit();
}
