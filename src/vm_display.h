#pragma once
#include <SDL2/SDL.h>

#include <algorithm>
#include <cstdint>
#include <vector>
using namespace std;

const uint8_t PIXEL_SIZE = 16;
const int ROW_WIDTH = 64;
const int ROW_HEIGHT = 32;

class VMDisplayDrawer {
   private:
    const uint16_t kPixelSize{PIXEL_SIZE};
    const uint16_t kScreenWidth{ROW_WIDTH * PIXEL_SIZE};
    const uint16_t kScreenHeight{ROW_HEIGHT * PIXEL_SIZE};
    SDL_Window* window{nullptr};
    SDL_Surface* surface{nullptr};
    uint8_t* pixel_array{nullptr};

   public:
    VMDisplayDrawer() {
        window = SDL_CreateWindow("CHIP-8 Emulator",
                                  SDL_WINDOWPOS_CENTERED,
                                  SDL_WINDOWPOS_CENTERED,
                                  kScreenWidth,
                                  kScreenHeight,
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
