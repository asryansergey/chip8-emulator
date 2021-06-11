#pragma once
#include <SDL2/SDL.h>

#include <algorithm>
#include <condition_variable>
#include <cstdint>
#include <vector>

#include "vm_keyboard.h"
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
    SDL_Renderer* renderer{nullptr};

   public:
    SDL_Event event{};
    VMKeyboard keyboard{};
    // To ensure thread safety.
    std::condition_variable cv;
    std::mutex cv_m;

    VMDisplayDrawer() {
        window = SDL_CreateWindow("CHIP-8 Emulator",
                                  SDL_WINDOWPOS_CENTERED,
                                  SDL_WINDOWPOS_CENTERED,
                                  kScreenWidth,
                                  kScreenHeight,
                                  SDL_WINDOW_SHOWN);
        assert(window != nullptr);
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        assert(renderer != nullptr);
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
        SDL_RenderSetLogicalSize(renderer, kScreenWidth, kScreenHeight);
    }

    ~VMDisplayDrawer() {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    void CreateDisplay();
    void RedrawScreen(const vector<uint8_t>& frame_buffer);
};
