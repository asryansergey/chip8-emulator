#pragma once
#include <chrono>
#include <ratio>
using namespace std::chrono;

class VMTimer {
   private:
    const duration<double, std::ratio<1, 60>> kHz60{1};
    const duration<double, std::milli> rate_ms{};
    time_point<std::chrono::system_clock> start_time{};

   public:
    uint8_t delay_timer{};
    uint8_t sound_timer{};

    VMTimer() : rate_ms(kHz60){};

    void UpdateStartTime() {
        start_time = system_clock::now();
    }

    bool IsDelayTimerActive() const {
        return delay_timer != 0;
    }

    auto GetCurrentTime() const {
        return system_clock::now();
    }

    void UpdateDelayTimeAndDuration() {
        auto now = system_clock::now();
        auto diff_ms = duration_cast<milliseconds>(now - start_time);
        if (diff_ms >= rate_ms) {
            delay_timer -= 1;
            start_time += duration_cast<milliseconds>(rate_ms);
        }
    }
};
