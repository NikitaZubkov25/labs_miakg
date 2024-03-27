#include "draw.h"
#include "helpers.h"

#include <SDL.h>
#include <stdio.h>
#include <string>
#include <cassert>
#include <iostream>

SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;
SDL_Texture *gTexture = NULL;
SDL_Surface *loadedSurface = NULL;

int main(int argc, char *argv[]) {
    float scale = 40;
    float alpha = 0;
    float beta = 0, prevBeta = 0;
    SDL_Point mouseLastClickCoords = {
        x: SCREEN_WIDTH / 2,
        y: SCREEN_HEIGHT / 2
    };
    SDL_Point moveCoords = {
        x: SCREEN_WIDTH / 2,
        y: SCREEN_HEIGHT / 2
    };
    SDL_FPoint diffCords = {
        x: 0,
        y: 0
    };
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
                    if (SDL_MOUSEBUTTONDOWN == e.type) {
                        alpha += beta;
                        beta = 0;

                        std::cout << moveCoords.x << " " <<  moveCoords.y << std::endl;
                        std::cout << diffCords.x << " " <<  diffCords.y << std::endl;

                        moveCoords.x += round( diffCords.x);
                        moveCoords.y += round( diffCords.y);
                        diffCords = {
                            x: 0,
                            y: 0
                        };

                        std::cout << moveCoords.x << " " <<  moveCoords.y << std::endl;
                        std::cout << diffCords.x << " " <<  diffCords.y << std::endl;

                        mouseLastClickCoords.x = e.button.x;
                        mouseLastClickCoords.y = e.button.y;
                    }
                    if (SDL_KEYDOWN == e.type) {
                        switch (e.key.keysym.scancode) {
                        case SDL_SCANCODE_0:
                            scale = std::min((int)scale+4,200);
                            break;
                        case SDL_SCANCODE_1:
                            scale = std::max((int)scale-4,20);
                            break;
                        case SDL_SCANCODE_DOWN:
                            moveCoords.y = std::min(moveCoords.y + 10, SCREEN_HEIGHT);
                            break;
                        case SDL_SCANCODE_UP:
                            moveCoords.y = std::max(moveCoords.y - 10, 0);
                            break;
                        case SDL_SCANCODE_RIGHT:
                            moveCoords.x = std::min(moveCoords.x + 10, SCREEN_WIDTH);
                            break;
                        case SDL_SCANCODE_LEFT:
                            moveCoords.x = std::max(moveCoords.x - 10, 0);
                            break;
                        case SDL_SCANCODE_Q:
                            alpha -= 0.1;
                            break;
                        case SDL_SCANCODE_E:
                            alpha += 0.1;
                            break;
                        case SDL_SCANCODE_ESCAPE:
                            quit = true;
                            break;
                        case SDL_SCANCODE_Z:
                            beta -= 0.1;
                            break;
                        case SDL_SCANCODE_X:
                            beta += 0.1;
                            break;
                        default:
                            break;
                        }
                    }
                }
                SDL_RenderClear(gRenderer);

                draw(loadedSurface, moveCoords, mouseLastClickCoords, diffCords, beta, alpha, scale);

                SDL_UpdateTexture(gTexture, NULL, loadedSurface->pixels, loadedSurface->pitch);
                SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);
                SDL_RenderPresent(gRenderer);
            }
        }
    }
    close(&gWindow, &gRenderer, &gTexture);
    return 0;
}
