#include "vm_viewer.h"

void VMViewer::CreateDisplay() {
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
            if (event.key.keysym.sym == SDLK_SPACE && event.type == SDL_KEYDOWN) {
                is_running = 0;
                keyboard.request_reload = 1;
                break;
            }
            if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
                const int idx = keyboard.MarkKeyAsPressed(event.key.keysym.sym, event.key.state);
                if (keyboard.KeyIsPressed(idx)) {
                    std::lock_guard<std::mutex> lk(cv_m);
                    keyboard.last_key_pressed = idx;
                } else {
                    std::lock_guard<std::mutex> lk(cv_m);
                    keyboard.AnyKeyIsPressed();
                }
                cv.notify_one();
            }
        }
    }
}

void VMViewer::RedrawScreen(const vector<uint8_t>& frame_buffer) {
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
