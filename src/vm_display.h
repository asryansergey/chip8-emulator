#pragma once
#include <SDL2/SDL.h>

#include <algorithm>
#include <condition_variable>
#include <cstdint>
#include <unordered_map>
#include <vector>
using namespace std;

const uint8_t PIXEL_SIZE = 16;
const int ROW_WIDTH = 64;
const int ROW_HEIGHT = 32;

class VMKeyboard {
   private:
    unordered_map<uint16_t, uint8_t> keys{
        {SDLK_0, 0},
        {SDLK_1, 1},
        {SDLK_2, 2},
        {SDLK_3, 3},
        {SDLK_4, 4},
        {SDLK_5, 5},
        {SDLK_6, 6},
        {SDLK_7, 7},
        {SDLK_8, 8},
        {SDLK_9, 9},
        {SDLK_a, 10},
        {SDLK_b, 11},
        {SDLK_c, 12},
        {SDLK_d, 13},
        {SDLK_e, 14},
        {SDLK_f, 15},
    };
    std::vector<u_char> keys_pressed;

   public:
    std::atomic<int8_t> last_key_pressed{-1};

    VMKeyboard() {
        keys_pressed.resize(keys.size(), 0);
    }

    int8_t GetKeyIndex(uint16_t key_code) const;
    int MarkKeyAsPressed(SDL_Keycode key_code, uint8_t state);
    u_char KeyIsPressed(const int idx) const;
    void ReleaseAllKeys();
    void AnyKeyIsPressed();
    uint16_t GetSize() const;
};

class VMDisplayDrawer {
   private:
    const uint16_t kPixelSize{PIXEL_SIZE};
    const uint16_t kScreenWidth{ROW_WIDTH * PIXEL_SIZE};
    const uint16_t kScreenHeight{ROW_HEIGHT * PIXEL_SIZE};
    SDL_Window* window{nullptr};
    SDL_Surface* surface{nullptr};
    uint8_t* pixel_array{nullptr};

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
