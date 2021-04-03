#pragma once

#include <cerrno>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <stack>

class Chip8VM {
   private:
    const uint16_t kOffsetAddress = 0x200;
    uint8_t memory[4096] = {};
    uint16_t ip{};
    uint8_t v[16] = {};
    std::stack<uint16_t> stack;
    uint16_t i{};   // Address register;
    uint16_t pc{};  // Program counter register

    void FetchInstruction();
    void DecodeInstruction();
    void ExecuteInstruction();

   public:
    bool ReadGameImage(const char*);
    void Start();
};