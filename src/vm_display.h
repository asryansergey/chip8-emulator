#pragma once
#include <SDL2/SDL.h>

#include <algorithm>
#include <cstdint>
#include <vector>
using namespace std;

const uint8_t PIXEL_SIZE = 16;
const int SCREEN_WIDTH = 64;
const int SCREEN_HEIGHT = 32;

class VMDisplayDrawer {
   private:
    [[maybe_unused]] uint16_t pixel_size{PIXEL_SIZE};
    uint16_t screen_width{SCREEN_WIDTH * PIXEL_SIZE};
    uint16_t screen_height{SCREEN_HEIGHT * PIXEL_SIZE};
    SDL_Window* window{nullptr};
    SDL_Surface* surface{nullptr};

   public:
    bool pixel_flipped{false};

    VMDisplayDrawer() {
        window = SDL_CreateWindow("CHIP-8 Emulator",
                                  SDL_WINDOWPOS_CENTERED,
                                  SDL_WINDOWPOS_CENTERED,
                                  64 * 16,
                                  32 * 16,
                                  SDL_WINDOW_SHOWN);
        assert(window != nullptr);
        surface = SDL_GetWindowSurface(window);
        assert(surface != nullptr);
    }

    ~VMDisplayDrawer() {
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    void CreateDisplay() {
        if (surface == nullptr || window == nullptr) {
            throw("[-] Invalid surface or window member variables.");
        }
        uint8_t* px = static_cast<uint8_t*>(surface->pixels);
        px[screen_height / 2 * surface->pitch + screen_width * 2] = 0xff;  // Just for testing.
        SDL_Event event;
        int is_running = 1;
        while (is_running) {
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    is_running = 0;
                }
            }
            SDL_UpdateWindowSurface(window);
        }
    }

    void SetPixelValueAt(/*uint8_t position, uint16_t single_pixel_value*/);

    void RedrawScreen(vector<uint8_t>& /*frame_buffer*/) {
        // TODO (asryansergey): Redraw the window with SDL2
    }
};