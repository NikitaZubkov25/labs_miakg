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

void drawFigure(SDL_Surface *s, std::vector<SDL_FPoint> &points)
{
    int vertexCount = points.size();
    for (int i = 0; i < vertexCount; i++) {
        for (float t = 0; t <= 1; t += 0.001) {
            float x = (points[(i + 1) % vertexCount].x - points[i].x) * t + points[i].x;
            float y = (points[(i + 1) % vertexCount].y - points[i].y) * t + points[i].y;
            put_pixel32(s, x, y, RGB32(40, 255, 40));
        }
    }
}

static int X_MIN = SCREEN_WIDTH / 2 - 200;
static int Y_MIN = SCREEN_HEIGHT / 2 + 80;
static int X_MAX = SCREEN_WIDTH / 2 + 200;
static int Y_MAX = SCREEN_HEIGHT / 2 - 80;

void draw(SDL_Surface *s, float radius, SDL_FPoint moveCoords)
{
    SDL_FillRect(s, NULL, RGB32(0, 0, 0));

    for (float t = 0; t <= 2 * M_PI; t += 0.001) {
        int x = moveCoords.x + radius * cos(t);
        int y = moveCoords.y + radius * sin(t);
        bool fitInWindow = (x < X_MAX && x > X_MIN && y > Y_MAX && y < Y_MIN);
        if (fitInWindow && (x + y) % 10 > 5) {
            put_pixel32(s, x, y, RGB32(100, 50, 240));
        } else if (!fitInWindow) {
            put_pixel32(s, x, y, RGB32(250, 50, 50));
        }
    }

    std::vector<SDL_FPoint> rectanglePoints = {
        { x : X_MIN, y : Y_MAX },
        { x : X_MAX, y : Y_MAX },
        { x : X_MAX, y : Y_MIN },
        { x : X_MIN, y : Y_MIN },
    };

    drawFigure(s, rectanglePoints);
}
