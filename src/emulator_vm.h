#pragma once

#include <cerrno>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <stack>
#include <vector>

// https://dev.krzaq.cc/post/you-dont-need-a-stateful-deleter-in-your-unique_ptr-usually/
struct FileDeleter {
    void operator()(FILE* ptr) const {
        fclose(ptr);
    }
};

class Chip8VM {
   private:
    struct OpcodeSchema {
        uint16_t opcode;
        uint16_t mask;
        void (Chip8VM::*handler)(uint16_t);
    };

    const uint16_t kOffsetAddress = 0x200;
    uint8_t mem_space[4096] = {};
    uint16_t ip{};
    uint8_t v[16] = {};
    std::stack<uint16_t> call_stack;
    uint16_t i{};   // Address register;
    uint16_t pc{};  // Program counter register

    std::vector<OpcodeSchema> opcode_entry = {
        /* {0NNN 0x0000, 0xF000, &Chip8VM::opcode0NNN }, */
        {/* 00E0 */ 0x00E0, 0xFFFF, &Chip8VM::opcode00E0},
        {/* 00EE */ 0x00EE, 0xFFFF, &Chip8VM::opcode00EE},
        {/* 1NNN */ 0x1000, 0xF000, &Chip8VM::opcode1NNN},
        {/* 2NNN */ 0x2000, 0xF000, &Chip8VM::opcode2NNN},
        {/* 3XNN */ 0x3000, 0xF000, &Chip8VM::opcode3XNN},
        {/* 4XNN */ 0x4000, 0xF000, &Chip8VM::opcode4XNN},
        {/* 5XY0 */ 0x5000, 0xF00F, &Chip8VM::opcode5XY0},
        {/* 6XNN */ 0x6000, 0xF000, &Chip8VM::opcode6XNN},
        {/* 7XNN */ 0x7000, 0xF000, &Chip8VM::opcode7XNN},
        {/* 8XY0 */ 0x8000, 0xF00F, &Chip8VM::opcode8XY0},
        {/* 8XY1 */ 0x8001, 0xF00F, &Chip8VM::opcode8XY1},
        {/* 8XY2 */ 0x8002, 0xF00F, &Chip8VM::opcode8XY2},
        {/* 8XY3 */ 0x8003, 0xF00F, &Chip8VM::opcode8XY3},
        {/* 8XY4 */ 0x8004, 0xF00F, &Chip8VM::opcode8XY4},
        {/* 8XY5 */ 0x8005, 0xF00F, &Chip8VM::opcode8XY5},
        {/* 8XY6 */ 0x8006, 0xF00F, &Chip8VM::opcode8XY6},
        {/* 8XY7 */ 0x8007, 0xF00F, &Chip8VM::opcode8XY7},
        {/* 8XYE */ 0x800E, 0xF00F, &Chip8VM::opcode8XYE},
        {/* 9XY0 */ 0x9000, 0xF00F, &Chip8VM::opcode9XY0},
        {/* ANNN */ 0xA000, 0xF000, &Chip8VM::opcodeANNN},
        {/* BNNN */ 0xB000, 0xF000, &Chip8VM::opcodeBNNN},
        {/* CXNN */ 0xC000, 0xF000, &Chip8VM::opcodeCXNN},
        {/* DXYN */ 0xD000, 0xF000, &Chip8VM::opcodeDXYN},
        {/* EX9E */ 0xE09E, 0xF0FF, &Chip8VM::opcodeEX9E},
        {/* EXA1 */ 0xE0A1, 0xF0FF, &Chip8VM::opcodeEXA1},
        {/* FX07 */ 0xF007, 0xF0FF, &Chip8VM::opcodeFX07},
        {/* FX0A */ 0xF00A, 0xF0FF, &Chip8VM::opcodeFX0A},
        {/* FX15 */ 0xF015, 0xF0FF, &Chip8VM::opcodeFX15},
        {/* FX18 */ 0xF018, 0xF0FF, &Chip8VM::opcodeFX18},
        {/* FX1E */ 0xF01E, 0xF0FF, &Chip8VM::opcodeFX1E},
        {/* FX29 */ 0xF029, 0xF0FF, &Chip8VM::opcodeFX29},
        {/* FX33 */ 0xF033, 0xF0FF, &Chip8VM::opcodeFX33},
        {/* FX55 */ 0xF055, 0xF0FF, &Chip8VM::opcodeFX55},
        {/* FX65 */ 0xF065, 0xF0FF, &Chip8VM::opcodeFX65}};

    void FetchInstruction();
    void DecodeInstruction();
    void ExecuteInstruction();

   public:
    bool ReadGameImage(const char*);
    void Start();

    /* Opcode handler functions */
    void opcode0NNN(uint16_t opcode);  // It is ignored by modern interpreters
    void opcode00E0(uint16_t opcode);
    void opcode00EE(uint16_t opcode);
    void opcode1NNN(uint16_t opcode);
    void opcode2NNN(uint16_t opcode);
    void opcode3XNN(uint16_t opcode);
    void opcode4XNN(uint16_t opcode);
    void opcode5XY0(uint16_t opcode);
    void opcode6XNN(uint16_t opcode);
    void opcode7XNN(uint16_t opcode);
    void opcode8XY0(uint16_t opcode);
    void opcode8XY1(uint16_t opcode);
    void opcode8XY2(uint16_t opcode);
    void opcode8XY3(uint16_t opcode);
    void opcode8XY4(uint16_t opcode);
    void opcode8XY5(uint16_t opcode);
    void opcode8XY6(uint16_t opcode);
    void opcode8XY7(uint16_t opcode);
    void opcode8XYE(uint16_t opcode);
    void opcode9XY0(uint16_t opcode);
    void opcodeANNN(uint16_t opcode);
    void opcodeBNNN(uint16_t opcode);
    void opcodeCXNN(uint16_t opcode);
    void opcodeDXYN(uint16_t opcode);
    void opcodeEX9E(uint16_t opcode);
    void opcodeEXA1(uint16_t opcode);
    void opcodeFX07(uint16_t opcode);
    void opcodeFX0A(uint16_t opcode);
    void opcodeFX15(uint16_t opcode);
    void opcodeFX18(uint16_t opcode);
    void opcodeFX1E(uint16_t opcode);
    void opcodeFX29(uint16_t opcode);
    void opcodeFX33(uint16_t opcode);
    void opcodeFX55(uint16_t opcode);
    void opcodeFX65(uint16_t opcode);
};