#pragma once
#include <SDL.h>
#include <cassert>

const int SCREEN_WIDTH = 920;
const int SCREEN_HEIGHT = 640;

enum Mode {
    clipInside,
    clipOutside,
    clipVisible
};

void draw(SDL_Surface *s, float radius, SDL_FPoint moveCoords, Mode currentMode);
