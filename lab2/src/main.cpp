#include "draw.h"

#include <SDL.h>
#include <stdio.h>
#include <string>
#include <cassert>

bool init();
void close();

SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;
SDL_Texture *gTexture = NULL;
SDL_Surface *loadedSurface = NULL;

bool init()
{
    bool success = true;
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        success = false;
    } else {
        gWindow = SDL_CreateWindow("AOKG Lab 2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == NULL) {
            printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
            success = false;
        } else {
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_SOFTWARE);
            if (gRenderer == NULL) {
                printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                success = false;
            } else {
                SDL_SetRenderDrawColor(gRenderer, 0x00, 0xFF, 0xFF, 0xFF);
            }
        }
    }
    return success;
}

void close()
{
    SDL_DestroyTexture(gTexture);
    gTexture = NULL;
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;
    SDL_Quit();
}

int main(int argc, char *argv[])
{
    if (!init()) {
        printf("Failed to initialize!\n");
        close();
        return 0;
    }
    loadedSurface = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32,
        0x00FF0000,// R
        0x0000FF00,// G
        0x000000FF,// B
        0x00000000);// alpha

    gTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
    SDL_FillRect(loadedSurface, NULL, 0x00FFFFFF);

    if (NULL == gTexture) {
        printf("Failed to load media!\n");
        close();
        return 0;
    }
    bool quit = false;
    SDL_Event e;

    float scale = 100, alpha = 0;
    int figuresCount = 10, vertexCount = 3;
    SDL_FPoint moveCoords = {
        x : SCREEN_WIDTH / 2,
        y : SCREEN_HEIGHT / 2,
    };

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (SDL_QUIT == e.type) {
                quit = true;
            }
            if (SDL_KEYDOWN == e.type) {
                switch (e.key.keysym.scancode) {
                case SDL_SCANCODE_Q:
                    alpha += 10;
                    break;
                case SDL_SCANCODE_E:
                    alpha -= 10;
                    break;
                case SDL_SCANCODE_UP:
                    moveCoords.y -= 5;
                    break;
                case SDL_SCANCODE_DOWN:
                    moveCoords.y += 5;
                    break;
                case SDL_SCANCODE_LEFT:
                    moveCoords.x -= 5;
                    break;
                case SDL_SCANCODE_RIGHT:
                    moveCoords.x += 5;
                    break;
                case SDL_SCANCODE_KP_PLUS:
                    scale += 2;
                    break;
                case SDL_SCANCODE_KP_MINUS:
                    scale = std::max(0, (int)scale - 2);
                    break;
                case SDL_SCANCODE_W:
                    figuresCount += 1;
                    break;
                case SDL_SCANCODE_S:
                    figuresCount = std::max(figuresCount - 1, 1);
                    break;
                case SDL_SCANCODE_1:
                    vertexCount = std::max(vertexCount - 1, 3);
                    break;
                case SDL_SCANCODE_2:
                    vertexCount += 1;
                    break;
                default:
                    break;
                }
            }
        }
        SDL_RenderClear(gRenderer);

        draw(loadedSurface, vertexCount, scale, moveCoords, figuresCount, alpha * M_PI / 180);

        SDL_UpdateTexture(gTexture, NULL, loadedSurface->pixels, loadedSurface->pitch);
        SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);
        SDL_RenderPresent(gRenderer);
    }
    close();
    return 0;
}
