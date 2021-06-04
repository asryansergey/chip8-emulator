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

    void CreateDisplay();
    void DrawScaledPixelsAt(uint8_t x, uint8_t y, uint8_t value);
    void RedrawScreen(const vector<uint8_t>& frame_buffer);
};
