#include "draw.h"

#include <math.h>
#include "SDL_surface.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#define RGB32(r, g, b) static_cast<uint32_t>((((static_cast<uint32_t>(r) << 8) | g) << 8) | b)

void put_pixel32(SDL_Surface *surface, int x, int y, Uint32 pixel, float alpha, int xm, int ym)
{
    assert(NULL != surface);
    float rotate_x = x * cos(alpha) + y * sin(alpha);
    float rotate_y = -x * sin(alpha) + y * cos(alpha);
    x = rotate_x + xm;
    y = rotate_y + ym;
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
    SDL_FillRect(s, NULL, 0x00000000);
}

void static draw_circle(SDL_Surface *s, float a, int xm, int ym, float alpha)
{
    float x, y;
    for (float t = 0; t < (M_PI * 2); t += 0.01) {
        x = a * cos(t) + a;
        y = a * sin(t);
        put_pixel32(s, x, y, RGB32(0, 255, 0), alpha, xm, ym);
    }
}

void static draw_line45deg(SDL_Surface *s, int xm, int ym, float alpha, float a)
{
    for (float t = -4; t < 4; t += 0.0001) {
        int x = t*a;
        int y = -t*a;
        put_pixel32(s, x, y, RGB32(140, 140, 200), alpha, xm, ym);
    }
}

void static draw_cisoid(SDL_Surface *s, float a, int xm, int ym, float alpha)
{
    float x = NAN;
    float y = NAN;
    for (float t = -3; t < 3; t += 0.0001) {
        x = a * t * t / (1 + t * t);
        y = a * t * t * t / (1 + t * t);
        int xr = round(x) + 3;
        int yr = round(y) + 3;
        put_pixel32(s, xr, yr, RGB32(255, 255, 255), alpha, xm, ym);
    }
}

void static draw_two_dots(SDL_Surface *s, float a, int xm, int ym, float alpha)
{
    for (float t = 0; t <= 2 * M_PI; t += 0.01) {
        float x = a / 30 * cos(t) + 11 * a / 20;
        float y = a / 30 * sin(t) - a * 11 / 20;
        put_pixel32(s, x, y, RGB32(255, 0, 0), alpha, xm, ym);
    }
    for (float t = 0; t <= 2 * M_PI; t += 0.01) {
        float x = a / 30 * cos(t);
        float y = a / 30 * sin(t);
        put_pixel32(s, x, y, RGB32(255, 0, 0), alpha, xm, ym);
    }
}

void static draw_axises(SDL_Surface *s)
{
    for (int i = 0; i < SCREEN_WIDTH; i++){
        put_pixel32(s, i, SCREEN_HEIGHT/2, RGB32(255, 255, 255), 0, 0 ,0);
    }

    for (int j = 0; j < SCREEN_HEIGHT; j++){
        put_pixel32(s, SCREEN_WIDTH/2, j, RGB32(255, 255, 255), 0, 0, 0);
    }
}

void draw(SDL_Surface *s, float a, int xm, int ym, float alpha)
{
    clear_surface(s);
    draw_axises(s);
    draw_cisoid(s, a, xm, ym, alpha);
    draw_circle(s, a, xm, ym, alpha);
    draw_line45deg(s, xm, ym, alpha, a);
    draw_two_dots(s,a,xm,ym, alpha);
}
