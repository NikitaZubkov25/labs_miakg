#include "draw.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <vector>

#define RGB32(r, g, b) static_cast<uint32_t>((((static_cast<uint32_t>(r) << 8) | g) << 8) | b)

void put_pixel32(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    if ((x > 0 && x < SCREEN_WIDTH) && (y > 0 && y < SCREEN_HEIGHT)) {
        Uint32 *pixels = (Uint32 *)surface->pixels;
        pixels[(y * surface->w) + x] = pixel;
    }
}

Uint32 get_pixel32(SDL_Surface *surface, int x, int y)
{
    assert(NULL != surface);
    assert(x < SCREEN_WIDTH);
    assert(y < SCREEN_HEIGHT);

    Uint32 *pixels = (Uint32 *)surface->pixels;
    return pixels[(y * surface->w) + x];
}

void drawAxises(SDL_Surface *s)
{
    for (int i = 0; i < SCREEN_WIDTH; i++)
        put_pixel32(s, i, SCREEN_HEIGHT / 2, RGB32(140, 140, 140));

    for (int j = 0; j < SCREEN_HEIGHT; j++)
        put_pixel32(s, SCREEN_WIDTH / 2, j, RGB32(140, 140, 140));
}

void convertCoordinates(std::vector<SDL_FPoint> &points, int vertexCount, float q)
{
    std::vector<SDL_FPoint> new_points (vertexCount);

    for (int i = 0; i < vertexCount; i++) {
        new_points[i].x = (1 - q) * points[i].x + q * points[(i + 1) % vertexCount].x;
        new_points[i].y = (1 - q) * points[i].y + q * points[(i + 1) % vertexCount].y;
    }

    for (int i = 0; i < vertexCount; i++) {
        points[i].x = new_points[i].x;
        points[i].y = new_points[i].y;
    }
}

void affineTransformation(std::vector<SDL_FPoint> &points, int vertexCount, float alpha, SDL_FPoint moveCoords)
{
    for (int i = 0; i < vertexCount; i++) {
        float rotate_x = points[i].x * cos(alpha) + points[i].y * sin(alpha);
        float rotate_y = -points[i].x * sin(alpha) + points[i].y * cos(alpha);
        points[i].x = rotate_x;
        points[i].y = rotate_y;
        points[i].x += moveCoords.x;
        points[i].y += moveCoords.y;
    }
}

void drawFigure(SDL_Surface *s, std::vector<SDL_FPoint> &points, int vertexCount)
{
    for (int i = 0; i < vertexCount; i++) {
        for (float t = 0; t <= 1; t += 0.001) {
            float x = (points[(i + 1) % vertexCount].x - points[i].x) * t + points[i].x;
            float y = (points[(i + 1) % vertexCount].y - points[i].y) * t + points[i].y;
            put_pixel32(s, x, y, RGB32(40, 255, 40));
        }
    }
}

void initializeVertices(std::vector<SDL_FPoint> &points, int vertexCount, float scale) {
  for (int i = 0; i < vertexCount; i++) {
        points[i].x = scale * cos(2 * M_PI * i / vertexCount);
        points[i].y = scale * sin(2 * M_PI * i / vertexCount);
  }
}

void draw(SDL_Surface *s, int vertexCount, float scale, SDL_FPoint moveCoords, int figuresCount, float alpha)
{
    SDL_FillRect(s, NULL, RGB32(0, 0, 0));

    std::vector<SDL_FPoint> points(vertexCount);
    initializeVertices(points, vertexCount, scale);

    drawAxises(s);
    affineTransformation(points, vertexCount, alpha, moveCoords);
    drawFigure(s, points, vertexCount);

    for (int i = 1; i <= figuresCount; i++) {
        float q = tan((i - 1) * M_PI / (4 * (figuresCount - 1))) / (tan((i - 1) * M_PI / (4 * (figuresCount - 1))) + 1);
        convertCoordinates(points, vertexCount, q);

        drawFigure(s, points, vertexCount);
    }
}
