#include "draw.h"
#include "helpers.h"

#include <SDL.h>
#include <stdio.h>
#include <string>
#include <cassert>

SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;
SDL_Texture *gTexture = NULL;
SDL_Surface *loadedSurface = NULL;

int main(int argc, char *argv[]) {
    float a = 100;
    int xm = SCREEN_WIDTH / 2, ym = SCREEN_HEIGHT / 2;
    if (!init(&gWindow, &gRenderer)) {
        printf("Failed to initialize!\n");
    } else {
        loadedSurface = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32,
            0x00FF0000,// R
            0x0000FF00,// G
            0x000000FF,// B
            0x00000000);// alpha

        gTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);

        if (NULL == gTexture) {
            printf("Failed to load media!\n");
        } else {
            bool quit = false;
            SDL_Event e;

            while (!quit) {
                while (SDL_PollEvent(&e) != 0) {
                    if (SDL_QUIT == e.type) {
                        quit = true;
                    }
                    if (SDL_KEYDOWN == e.type) {
                        switch (e.key.keysym.scancode) {
                        case SDL_SCANCODE_0:
                            a++;
                            break;
                        case SDL_SCANCODE_1:
                            a--;
                            break;
                        case SDL_SCANCODE_DOWN:
                            ym = std::max(ym-10, 0);
                            break;
                        case SDL_SCANCODE_UP:
                            ym = std::min(ym+10, SCREEN_HEIGHT);
                            break;
                        case SDL_SCANCODE_RIGHT:
                            xm = std::max(xm-10,0);
                            break;
                        case SDL_SCANCODE_LEFT:
                            xm = std::min(xm+10,SCREEN_WIDTH);
                            break;
                        case SDL_SCANCODE_ESCAPE:
                            quit = true;
                            break;
                        default:
                            break;
                        }
                    }
                }
                SDL_RenderClear(gRenderer);

                draw(loadedSurface, a, xm, ym);

                SDL_UpdateTexture(gTexture, NULL, loadedSurface->pixels, loadedSurface->pitch);
                SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);
                SDL_RenderPresent(gRenderer);
            }
        }
    }
    close(&gWindow, &gRenderer, &gTexture);
    return 0;
}
