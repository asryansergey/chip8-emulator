#include "vm_keyboard.h"

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
