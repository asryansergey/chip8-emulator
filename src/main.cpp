#include <SDL2/SDL.h>
#include <stdio.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main(int argc, char* argv[]) {
    SDL_Window* window = nullptr;

    if (SDL_Init(SDL_INIT_VIDEO)) {
        printf("%s %s\n", "Could not initialize SDL! Error code: ", SDL_GetError());
    } else {
        window = SDL_CreateWindow("CHIP-8 Emulator",
                                  SDL_WINDOWPOS_UNDEFINED,
                                  SDL_WINDOWPOS_UNDEFINED,
                                  SCREEN_WIDTH,
                                  SCREEN_HEIGHT,
                                  SDL_WINDOW_SHOWN);
        if (window == NULL) {
            printf("%s %s\n", "Could not create SDL window! Error code: ", SDL_GetError());
        } else {
            SDL_Surface* screen_surface = SDL_GetWindowSurface(window);
            if (screen_surface == NULL) {
                printf("%s %s\n", "Could not get window surface! Error code: ", SDL_GetError());
            } else {
                SDL_Event event;
                int is_running = 1;
                while (is_running) {
                    while (SDL_PollEvent(&event)) {
                        if (event.type == SDL_QUIT) {
                            is_running = 0;
                        }
                    }
                    SDL_FillRect(screen_surface, NULL,
                                 SDL_MapRGB(screen_surface->format, 0xFF, 0xFF, 0xFF));
                    SDL_UpdateWindowSurface(window);
                }
            }
        }
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    return 0;
}
