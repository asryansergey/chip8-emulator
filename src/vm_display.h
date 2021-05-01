#pragma once
#include <algorithm>
#include <cstdint>
#include <vector>

const uint8_t PIXEL_SIZE = 16;
const int SCREEN_WIDTH = 64;
const int SCREEN_HEIGHT = 32;

class VMDisplayDrawer {
   private:
    uint16_t pixel_size{PIXEL_SIZE};
    uint16_t screen_width{SCREEN_WIDTH};
    uint16_t screen_height{SCREEN_HEIGHT};
    std::vector<uint8_t> buffer;
    uint8_t pixel_flipped{};

   public:
    VMDisplayDrawer() {
        buffer.resize(screen_height * screen_width);
    }

    void ClearScreen() {
        std::for_each(buffer.begin(), buffer.end(), [](uint8_t& elem) {
            elem = 0;
        });
    }

    // TODO (asryansergey): Function for setting display pixel value
    void SetPixelValueAt(uint8_t position, uint16_t pixel_value) {
    }
};