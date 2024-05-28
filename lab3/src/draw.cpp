#include "draw.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <vector>

#define RGB32(r, g, b) static_cast<uint32_t>((((static_cast<uint32_t>(r) << 8) | g) << 8) | b)

void put_pixel32(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    if ((x >= 0 && x < SCREEN_WIDTH) && (y >= 0 && y < SCREEN_HEIGHT)) {
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

struct LineSegment
{
    SDL_FPoint p0, p1;
};

std::vector<LineSegment> generateCircleLineSegments(float radius, SDL_FPoint moveCoords)
{
    std::vector<LineSegment> segments;
    const float step = 0.01;
    for (float t = 0; t < 2 * M_PI; t += step) {
        SDL_FPoint p0 = { moveCoords.x + radius * cos(t), moveCoords.y + radius * sin(t) };
        SDL_FPoint p1 = { moveCoords.x + radius * cos(t + step), moveCoords.y + radius * sin(t + step) };
        segments.push_back({ p0, p1 });
    }
    return segments;
}

float dotProduct(const SDL_FPoint &a, const SDL_FPoint &b)
{
    return a.x * b.x + a.y * b.y;
}

bool cyrusBeckClip(const LineSegment &segment, SDL_FPoint *clippedStart, SDL_FPoint *clippedEnd)
{
    SDL_FPoint d = { segment.p1.x - segment.p0.x, segment.p1.y - segment.p0.y };
    float tE = 0.0;
    float tL = 1.0;

    std::vector<SDL_FPoint> normals = {
        { 0, 1 }, { -1, 0 }, { 0, -1 }, { 1, 0 }
    };

    std::vector<SDL_FPoint> points = {
        { X_MIN, Y_MAX }, { X_MAX, Y_MAX }, { X_MAX, Y_MIN }, { X_MIN, Y_MIN }
    };

    for (int i = 0; i < 4; ++i) {
        SDL_FPoint w = { segment.p0.x - points[i].x, segment.p0.y - points[i].y };
        float dn = dotProduct(d, normals[i]);
        float wn = dotProduct(w, normals[i]);

        if (dn != 0.0) {
            float t = -wn / dn;
            if (dn > 0.0) {
                if (t > tL) return false;
                if (t > tE) tE = t;
            } else {
                if (t < tE) return false;
                if (t < tL) tL = t;
            }
        } else if (wn < 0.0) {
            return false;
        }
    }

    *clippedStart = { segment.p0.x + tE * d.x, segment.p0.y + tE * d.y };
    *clippedEnd = { segment.p1.x + tL * d.x, segment.p1.y + tL * d.y };
    return true;
}

void draw(SDL_Surface *s, float radius, SDL_FPoint moveCoords, Mode currentMode)
{
    SDL_FillRect(s, NULL, RGB32(0, 0, 0));

    std::vector<LineSegment> circleSegments = generateCircleLineSegments(radius, moveCoords);

    for (int i = 0; i < circleSegments.size(); i++) {
        LineSegment segment = circleSegments[i];
        SDL_FPoint clippedStart, clippedEnd;
        Uint32 color = currentMode == clipOutside ? RGB32(250, 50, 50) : RGB32(100, 50, 240);

        if (cyrusBeckClip(segment, &clippedStart, &clippedEnd)) {
            if (currentMode != clipInside) {
                for (float t = 0; t <= 1; t += 0.001) {
                    float x = (clippedEnd.x - clippedStart.x) * t + clippedStart.x;
                    float y = (clippedEnd.y - clippedStart.y) * t + clippedStart.y;
                    put_pixel32(s, x, y, color);
                }
            }
        } else {
            if (currentMode != clipOutside) {
                for (float t = 0; t <= 1; t += 0.001) {
                    float x = (segment.p1.x - segment.p0.x) * t + segment.p0.x;
                    float y = (segment.p1.y - segment.p0.y) * t + segment.p0.y;
                    put_pixel32(s, x, y, RGB32(250, 50, 50));
                }
            }
        }
    }

    std::vector<SDL_FPoint> rectanglePoints = {
        { X_MIN, Y_MAX },
        { X_MAX, Y_MAX },
        { X_MAX, Y_MIN },
        { X_MIN, Y_MIN },
    };

    drawFigure(s, rectanglePoints);
}