#include "draw.h"

#include <math.h>
#include "SDL_surface.h"
#include <iostream>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#define RGB32(r, g, b) static_cast<uint32_t>((((static_cast<uint32_t>(r) << 8) | g) << 8) | b)

static SDL_Point moveCoords, mouseLastClickCoords;
static float scale = 0, beta = 0, alpha = 0;
static SDL_Surface *surface;

void put_pixel(int x, int y, Uint32 pixel)
{
    assert(NULL != surface);

    if (x > SCREEN_WIDTH && x >= 0) return;
    if (y > SCREEN_HEIGHT && y >= 0) return;

    Uint32 *pixels = (Uint32 *)surface->pixels;
    pixels[(y * surface->w) + x] = pixel;
}

void affineTransformPoint(SDL_FPoint &point, float alpha)
{
    point = {
        x: point.x * cos(alpha) + point.y * sin(alpha),
        y: -point.x * sin(alpha) + point.y * cos(alpha)
    };
}

void spinAroundPoint(SDL_FPoint &pointToSpin, SDL_Point pointAround,float beta)
{
    pointToSpin.x -= pointAround.x;
    pointToSpin.y -= pointAround.y;

    affineTransformPoint(pointToSpin, beta);

    pointToSpin.x += pointAround.x;
    pointToSpin.y += pointAround.y;
}

template <typename T>
void movePoint(T &point, SDL_Point moveCoords)
{
    point = {
        x: point.x + moveCoords.x,
        y: point.y + moveCoords.y
    };
}

void static clearSurface(SDL_Surface *s)
{
    SDL_FillRect(s, NULL, 0x00000000);
}

void static drawCircle(float scale)
{
    SDL_FPoint circlePoint;
    for (float t = 0; t < (M_PI * 2); t += 0.01) {
        circlePoint.x = scale * cos(t) + scale;
        circlePoint.y = scale * sin(t);

        affineTransformPoint(circlePoint, alpha);
        movePoint<SDL_FPoint>(circlePoint, moveCoords);
        spinAroundPoint(circlePoint, mouseLastClickCoords, beta);

        put_pixel(circlePoint.x, circlePoint.y, RGB32(0, 255, 0));
    }
}

void static drawLine45deg(float scale)
{
    SDL_FPoint linePoint;
    for (float t = -4; t < 4; t += 0.0001) {
        linePoint.x = t*scale;
        linePoint.y = -t*scale;

        affineTransformPoint(linePoint, alpha);
        movePoint<SDL_FPoint>(linePoint, moveCoords);
        spinAroundPoint(linePoint, mouseLastClickCoords, beta);

        put_pixel(linePoint.x, linePoint.y, RGB32(140, 140, 200));
    }
}

void static drawCisoid(float scale)
{
    SDL_FPoint cisoidPoint;
    for (float t = -3; t < 3; t += 0.0001) {
        cisoidPoint.x = scale * t * t / (1 + t * t);
        cisoidPoint.y = scale * t * t * t / (1 + t * t);

        affineTransformPoint(cisoidPoint,alpha);
        movePoint(cisoidPoint, moveCoords);
        spinAroundPoint(cisoidPoint, mouseLastClickCoords, beta);

        put_pixel(cisoidPoint.x, cisoidPoint.y, RGB32(255, 255, 255));
    }
}

void static drawTwoDots(float scale)
{
    SDL_FPoint dotPoint;
    for (float t = 0; t <= 2 * M_PI; t += 0.01) {
        dotPoint.x = scale / 30 * cos(t) + 11 * scale / 20;
        dotPoint.y = scale / 30 * sin(t) - scale * 11 / 20;

        affineTransformPoint(dotPoint,alpha);
        movePoint<SDL_FPoint>(dotPoint, moveCoords);
        spinAroundPoint(dotPoint, mouseLastClickCoords, beta);

        put_pixel(dotPoint.x, dotPoint.y, RGB32(255, 0, 0));
    }
    for (float t = 0; t <= 2 * M_PI; t += 0.01) {
        dotPoint.x = scale / 30 * cos(t);
        dotPoint.y = scale / 30 * sin(t);

        affineTransformPoint(dotPoint,alpha);
        movePoint<SDL_FPoint>(dotPoint, moveCoords);
        spinAroundPoint(dotPoint, mouseLastClickCoords, beta);

        put_pixel(dotPoint.x, dotPoint.y, RGB32(255, 0, 0));
    }
}

void static drawAxises()
{
    for (int i = 0; i < SCREEN_WIDTH; i++){
        put_pixel(i, SCREEN_HEIGHT/2, RGB32(255, 255, 255));
    }

    for (int j = 0; j < SCREEN_HEIGHT; j++){
        put_pixel(SCREEN_WIDTH/2, j, RGB32(255, 255, 255));
    }
}

void draw(SDL_Surface *s, SDL_Point moveCoordsArg, SDL_Point mouseLastClickCoordsArg, float betaArg, float alphaArg, float scale)
{
    surface = s; moveCoords = moveCoordsArg; mouseLastClickCoords = mouseLastClickCoordsArg; beta = betaArg; alpha = alphaArg;

    clearSurface(s);
    drawAxises();
    drawCisoid(scale);
    drawCircle(scale);
    drawLine45deg(scale);
    drawTwoDots(scale);
}
