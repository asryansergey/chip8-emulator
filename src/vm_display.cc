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
    if (renderer == nullptr || window == nullptr) {
        throw("[-] Invalid renderer or window member variables.");
    }
    int is_running = 1;
    while (is_running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                is_running = 0;
                break;
            }
            if (event.key.keysym.sym == SDLK_ESCAPE && event.type == SDL_KEYDOWN) {
                is_running = 0;
                /**
                 * TODO(asryansergey): If no value is assigned to last_key_pressed
                 * thread is blocked when ESC is pressed first.
                 */
                keyboard.last_key_pressed = -2;
                cv.notify_one();
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
    }
}

void VMDisplayDrawer::RedrawScreen(const vector<uint8_t>& frame_buffer) {
    if (renderer == nullptr) {
        throw("[-] Renderer is not initialized.");
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, SDL_ALPHA_OPAQUE);
    SDL_Rect line{};
    for (auto i = 0; i < ROW_HEIGHT * ROW_WIDTH; ++i) {
        if (frame_buffer[i]) {
            uint16_t x = i % ROW_WIDTH;
            uint16_t y = floor(i * 1.0 / ROW_WIDTH);
            line = {x * kPixelSize, y * kPixelSize, kPixelSize - 1, kPixelSize - 1};
            SDL_RenderFillRect(renderer, &line);
        }
    }
    SDL_RenderPresent(renderer);
}
