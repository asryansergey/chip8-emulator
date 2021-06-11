#include "vm_display.h"

int8_t VMKeyboard::GetKeyIndex(uint16_t key_code) const {
    try {
        return keys.at(key_code);
    } catch (const std::out_of_range& e) {
        printf("%s\n", e.what());
        return -1;
    }
}

int VMKeyboard::MarkKeyAsPressed(SDL_Keycode key_code, uint8_t state) {
    const int key_idx = GetKeyIndex(key_code);
    if (key_idx != -1) {
        keys_pressed[key_idx] = (SDL_PRESSED == state);
    }
    return key_idx;
}

u_char VMKeyboard::KeyIsPressed(const int idx) const {
    try {
        return keys_pressed.at(idx);
    } catch (const std::out_of_range& e) {
        printf("%s\n", e.what());
        return 0;
    }
}

void VMKeyboard::ReleaseAllKeys() {
    std::for_each(keys_pressed.begin(), keys_pressed.end(), [](u_char& elem) {
        elem = false;
    });
}

void VMKeyboard::AnyKeyIsPressed() {
    bool all_zero = std::any_of(keys_pressed.begin(), keys_pressed.end(), [](const u_char& elem) {
        return elem == 1;
    });
    if (!all_zero) {
        last_key_pressed = -1;
    }
}

uint16_t VMKeyboard::GetSize() const {
    return keys.size();
}

void VMDisplayDrawer::CreateDisplay() {
    if (surface == nullptr || window == nullptr) {
        throw("[-] Invalid surface or window member variables.");
    }
    int is_running = 1;
    while (is_running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                is_running = 0;
                break;
            }
            if (event.key.keysym.sym == SDLK_ESCAPE && event.key.state == SDL_PRESSED) {
                is_running = 0;
                break;
            }
            if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
                const int idx = keyboard.MarkKeyAsPressed(event.key.keysym.sym, event.key.state);
                std::lock_guard<std::mutex> lk(cv_m);
                if (keyboard.KeyIsPressed(idx)) {
                    keyboard.last_key_pressed = idx;
                } else {
                    keyboard.AnyKeyIsPressed();
                }
                cv.notify_one();
            }
        }
        SDL_UpdateWindowSurface(window);
    }
}

void VMDisplayDrawer::DrawScaledPixelsAt(uint8_t x, uint8_t y, uint8_t value) {
    for (auto i = y * kPixelSize; i < (y + 1) * kPixelSize; ++i) {
        for (auto j = x * kPixelSize; j < (x + 1) * kPixelSize; ++j) {
            // TODO(asryansergey): Might need to correct byte calculation
            pixel_array[(i * kScreenWidth + j) * 4 + 0] = value;
            pixel_array[(i * kScreenWidth + j) * 4 + 1] = value;
            pixel_array[(i * kScreenWidth + j) * 4 + 2] = value;
            pixel_array[(i * kScreenWidth + j) * 4 + 3] = value;
        }
    }
}

void VMDisplayDrawer::RedrawScreen(const vector<uint8_t>& frame_buffer) {
    if (surface == nullptr) {
        throw("[-] Surface is not initialized.");
    }
    for (auto y = 0; y < ROW_HEIGHT; ++y) {
        for (auto x = 0; x < ROW_WIDTH; ++x) {
            DrawScaledPixelsAt(x, y, frame_buffer[y * 64 + x] * 0xff);
        }
    }
}
