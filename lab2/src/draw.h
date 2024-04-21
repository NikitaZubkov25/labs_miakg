#pragma once
#include <SDL.h>
#include <cassert>

const int SCREEN_WIDTH = 920;
const int SCREEN_HEIGHT = 640;

void draw(SDL_Surface *s, int vertexCount, float scale, SDL_FPoint moveCoords, int figuresCount, float alpha);
