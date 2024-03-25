#pragma once
#include <SDL.h>
#include <cassert>

// Screen dimension constants
const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;

void draw(SDL_Surface *s, SDL_Point moveCoordsArg, SDL_Point mouseLastClickCoordsArg,float betaArg, float alphaArg, float scale);
