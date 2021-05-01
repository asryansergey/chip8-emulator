#include <SDL2/SDL.h>

#include <cstdio>

#include "emulator_vm.h"

int main(int argc, char* argv[]) {
    SDL_Window* window = nullptr;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER | SDL_INIT_EVENTS)) {
        printf("%s %s\n", "Could not initialize SDL! Error code: ", SDL_GetError());
    } else {
        window = SDL_CreateWindow("CHIP-8 Emulator",
                                  SDL_WINDOWPOS_CENTERED,
                                  SDL_WINDOWPOS_CENTERED,
                                  64 * 16,
                                  32 * 16,
                                  SDL_WINDOW_SHOWN);
        if (window == nullptr) {
            printf("%s %s\n", "Could not create SDL window! Error code: ", SDL_GetError());
        } else {
            SDL_Surface* screen_surface = SDL_GetWindowSurface(window);
            if (screen_surface == nullptr) {
                printf("%s %s\n", "Could not get window surface! Error code: ", SDL_GetError());
            } else {
                SDL_FillRect(screen_surface, nullptr,
                             SDL_MapRGB(screen_surface->format, 0xFF, 0xFF, 0xFF));
                SDL_UpdateWindowSurface(window);
                SDL_Event event;
                int is_running = 1;
                while (is_running) {
                    while (SDL_PollEvent(&event)) {
                        if (event.type == SDL_QUIT) {
                            is_running = 0;
                        }
                    }
                }
            }
        }
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    return 0;
}
