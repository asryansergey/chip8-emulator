#include "vm_display.h"

void VMDisplayDrawer::CreateDisplay() {
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

void VMDisplayDrawer::DrawScaledPixelsAt(uint8_t x, uint8_t y, uint8_t value) {
    for (auto i = y * pixel_size; i < (y + 1) * pixel_size; ++i) {
        for (auto j = x * pixel_size; j < (x + 1) * pixel_size; ++j) {
            // TODO(asryansergey): Might need to correct byte calculation
            pixel_array[(i * screen_width + j) * 4 + 0] = value;
            pixel_array[(i * screen_width + j) * 4 + 1] = value;
            pixel_array[(i * screen_width + j) * 4 + 2] = value;
            pixel_array[(i * screen_width + j) * 4 + 3] = value;
        }
    }
}

void VMDisplayDrawer::RedrawScreen(const vector<uint8_t>& frame_buffer) {
    if (surface == nullptr) {
        throw("[-] Surface is not initialized.");
    }
    for (auto y = 0; y < SCREEN_HEIGHT; ++y) {
        for (auto x = 0; x < SCREEN_WIDTH; ++x) {
            DrawScaledPixelsAt(x, y, frame_buffer[y * 64 + x] * 0xff);
        }
    }
}