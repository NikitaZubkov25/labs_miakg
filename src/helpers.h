#pragma once

#include <SDL.h>
#include <stdio.h>

bool init(
    SDL_Window **gWindow,
    SDL_Renderer **gRenderer);
void close(SDL_Window **gWindow,
    SDL_Renderer **gRenderer,
    SDL_Texture **gTexture);
