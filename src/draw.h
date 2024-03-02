#pragma once
#include <SDL.h>
#include <cassert>

// Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

void draw(SDL_Surface *s, float a, int xm, int ym);
