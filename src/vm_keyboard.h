#pragma once
#include <SDL2/SDL.h>

#include <cstdint>
#include <unordered_map>
#include <vector>

class VMKeyboard {
   private:
    std::unordered_map<uint16_t, uint8_t> keys{
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
