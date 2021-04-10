#pragma once
#include <cstdint>
#include <vector>

const uint8_t PIXEL_SIZE = 16;
const int SCREEN_WIDTH = 64 * PIXEL_SIZE;
const int SCREEN_HEIGHT = 32 * PIXEL_SIZE;

class VMDisplayDrawer {
   private:
    uint16_t pixel_size{PIXEL_SIZE};
    uint16_t screen_width{SCREEN_WIDTH};
    uint16_t screen_hwight{SCREEN_HEIGHT};
    std::vector<uint8_t> buffer;

   public:
    VMDisplayDrawer() {
        buffer.resize(screen_hwight * screen_width);
    }
};