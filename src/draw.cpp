#include "draw.h"

#include <math.h>
#include "SDL_surface.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#define RGB32(r, g, b) static_cast<uint32_t>((((static_cast<uint32_t>(r) << 8) | g) << 8) | b)

void put_pixel32(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    assert(NULL != surface);
    if (x > SCREEN_WIDTH) return;
    if (y > SCREEN_HEIGHT) return;

    Uint32 *pixels = (Uint32 *)surface->pixels;
    pixels[(y * surface->w) + x] = pixel;
}

Uint32 get_pixel32(SDL_Surface *surface, int x, int y)
{
    assert(NULL != surface);

    Uint32 *pixels = (Uint32 *)surface->pixels;
    return pixels[(y * surface->w) + x];
}

void static clear_surface(SDL_Surface *s)
{
    for (int x = 0; x <= SCREEN_WIDTH; x++)
        for (int y = 0; y <= SCREEN_HEIGHT; y++)
            put_pixel32(s, x, y, RGB32(0, 0, 0));
}

void static draw_circle(SDL_Surface *s, float a, int xm, int ym)
{
    float x, y;
    for (float t = 0; t < (M_PI * 2); t += 0.01) {
        x = a * cos(t) + a;
        y = a * sin(t);
        put_pixel32(s, x + xm, y + ym, RGB32(0, 255, 0));
    }
}

void static draw_line(SDL_Surface *s, float a)
{
    for (int y = 0; y <= SCREEN_HEIGHT; y++) put_pixel32(s, round(a), y, RGB32(0, 0, 255));
}

void static draw_line45deg(SDL_Surface *s, int xm, int ym)
{
    int x = xm;
    int y = ym;
    int length = abs(xm) > abs(ym) ? abs(xm) : abs(ym);
    for (int i = 0; i < length; i++) {
        put_pixel32(s, x + i, y - i, RGB32(140, 140, 200));
    }
}

void static draw_cisoid(SDL_Surface *s, float a, int xm, int ym)
{
    float x = NAN;
    float y = NAN;
    for (float t = -3; t < 3; t += 0.0001) {
        x = a * t * t / (1 + t * t);
        y = a * t * t * t / (1 + t * t);
        int xr = round(x) + 3;
        int yr = round(y) + 3;
        put_pixel32(s, xr + xm, yr + ym, RGB32(255, 255, 255));
    }
}

void static draw_two_dots(SDL_Surface *s, float a, int xm, int ym)
{
    for (float t = 0; t <= 2 * M_PI; t += 0.01) {
        float x = a / 30 * cos(t) + 11 * a / 20 + xm;
        float y = a / 30 * sin(t) + ym - a * 11 / 20;
        put_pixel32(s, x, y, RGB32(255, 0, 0));
    }
    for (float t = 0; t <= 2 * M_PI; t += 0.01) {
        float x = a / 30 * cos(t) + xm;
        float y = a / 30 * sin(t) + ym;
        put_pixel32(s, x, y, RGB32(255, 0, 0));
    }
}

void draw(SDL_Surface *s, float a, int xm, int ym)
{
    clear_surface(s);
    draw_cisoid(s, a, xm, ym);
    draw_circle(s, a, xm, ym);
    draw_line(s, a * 2 + xm);
    draw_line45deg(s, xm, ym);
    draw_two_dots(s,a,xm,ym);
}
