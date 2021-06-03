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
    [[maybe_unused]] uint16_t screen_height{SCREEN_HEIGHT * PIXEL_SIZE};
    SDL_Window* window{nullptr};
    SDL_Surface* surface{nullptr};
    uint8_t* pixel_array{nullptr};

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
        pixel_array = static_cast<uint8_t*>(surface->pixels);
    }

    ~VMDisplayDrawer() {
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    void CreateDisplay() {
        if (surface == nullptr || window == nullptr) {
            throw("[-] Invalid surface or window member variables.");
        }
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

    void DrawScaledPixelsAt(uint8_t x, uint8_t y, uint8_t value) {
        for (auto i = y * pixel_size; i < (y + 1) * pixel_size; ++i) {
            for (auto j = x * pixel_size; j < (x + 1) * pixel_size; ++j) {
                // TODO(asryansergey): Might need to correct byte calculation
                pixel_array[(i * screen_width + j) * 4 + 0] = value;
                pixel_array[(i * screen_width + j) * 4 + 1] = value;
                pixel_array[(i * screen_width + j) * 4 + 2] = value;
                pixel_array[(i * screen_width + j) * 4 + 1] = value;
            }
        }
    }

    void RedrawScreen(const vector<uint8_t>& frame_buffer) {
        if (surface == nullptr) {
            throw("[-] Surface is not initialized.");
        }
        for (auto y = 0; y < SCREEN_HEIGHT; ++y) {
            for (auto x = 0; x < SCREEN_WIDTH; ++x) {
                DrawScaledPixelsAt(x, y, frame_buffer[y * 64 + x] * 0xff);
            }
        }
    }
};
