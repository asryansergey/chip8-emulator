#pragma once

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <stack>
#include <thread>
#include <vector>

#include "vm_timer.h"
#include "vm_viewer.h"

// https://dev.krzaq.cc/post/you-dont-need-a-stateful-deleter-in-your-unique_ptr-usually/
struct FileDeleter {
    void operator()(FILE* ptr) const {
        fclose(ptr);
    }
};

class Chip8VM {
   private:
    struct OpcodeSchema {
        string name;
        uint16_t opcode;
        uint16_t mask;
        void (Chip8VM::*handler)(uint16_t);
    };

    const uint16_t kOffsetAddress{0x200};
    std::vector<uint8_t> mem_space;
    std::vector<uint8_t> frame_buffer;
    uint8_t v[16] = {};  // Registers
    std::stack<uint16_t> call_stack;
    uint16_t i{0};       // Address register;
    uint16_t pc{0x200};  // Program counter register
    atomic<bool> is_stopped{0};
    VMTimer timer{};

    const vector<uint8_t> fonts = {
        0xF0, 0x90, 0x90, 0x90, 0xF0,  // 0
        0x20, 0x60, 0x20, 0x20, 0x70,  // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0,  // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0,  // 3
        0x90, 0x90, 0xF0, 0x10, 0x10,  // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0,  // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0,  // 6
        0xF0, 0x10, 0x20, 0x40, 0x40,  // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0,  // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0,  // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90,  // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0,  // B
        0xF0, 0x80, 0x80, 0x80, 0xF0,  // C
        0xE0, 0x90, 0x90, 0x90, 0xE0,  // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0,  // E
        0xF0, 0x80, 0xF0, 0x80, 0x80,  // F
    };

    uint16_t GetValueX(uint16_t opcode) const;
    uint16_t GetValueY(uint16_t opcode) const;
    uint16_t GetValueN(uint16_t opcode) const;
    uint16_t GetValueNN(uint16_t opcode) const;
    uint16_t GetValueNNN(uint16_t opcode) const;

    std::vector<OpcodeSchema>
        opcode_entry = {
            {"00E0", 0x00E0, 0xFFFF, &Chip8VM::Opcode00E0},
            {"00EE", 0x00EE, 0xFFFF, &Chip8VM::Opcode00EE},
            {"0NNN", 0x0000, 0xF000, &Chip8VM::Opcode0NNN},
            {"1NNN", 0x1000, 0xF000, &Chip8VM::Opcode1NNN},
            {"2NNN", 0x2000, 0xF000, &Chip8VM::Opcode2NNN},
            {"3XNN", 0x3000, 0xF000, &Chip8VM::Opcode3XNN},
            {"4XNN", 0x4000, 0xF000, &Chip8VM::Opcode4XNN},
            {"5XY0", 0x5000, 0xF00F, &Chip8VM::Opcode5XY0},
            {"6XNN", 0x6000, 0xF000, &Chip8VM::Opcode6XNN},
            {"7XNN", 0x7000, 0xF000, &Chip8VM::Opcode7XNN},
            {"8XY0", 0x8000, 0xF00F, &Chip8VM::Opcode8XY0},
            {"8XY1", 0x8001, 0xF00F, &Chip8VM::Opcode8XY1},
            {"8XY2", 0x8002, 0xF00F, &Chip8VM::Opcode8XY2},
            {"8XY3", 0x8003, 0xF00F, &Chip8VM::Opcode8XY3},
            {"8XY4", 0x8004, 0xF00F, &Chip8VM::Opcode8XY4},
            {"8XY5", 0x8005, 0xF00F, &Chip8VM::Opcode8XY5},
            {"8XY6", 0x8006, 0xF00F, &Chip8VM::Opcode8XY6},
            {"8XY7", 0x8007, 0xF00F, &Chip8VM::Opcode8XY7},
            {"8XYE", 0x800E, 0xF00F, &Chip8VM::Opcode8XYE},
            {"9XY0", 0x9000, 0xF00F, &Chip8VM::Opcode9XY0},
            {"ANNN", 0xA000, 0xF000, &Chip8VM::OpcodeANNN},
            {"BNNN", 0xB000, 0xF000, &Chip8VM::OpcodeBNNN},
            {"CXNN", 0xC000, 0xF000, &Chip8VM::OpcodeCXNN},
            {"DXYN", 0xD000, 0xF000, &Chip8VM::OpcodeDXYN},
            {"EX9E", 0xE09E, 0xF0FF, &Chip8VM::OpcodeEX9E},
            {"EXA1", 0xE0A1, 0xF0FF, &Chip8VM::OpcodeEXA1},
            {"FX07", 0xF007, 0xF0FF, &Chip8VM::OpcodeFX07},
            {"FX0A", 0xF00A, 0xF0FF, &Chip8VM::OpcodeFX0A},
            {"FX15", 0xF015, 0xF0FF, &Chip8VM::OpcodeFX15},
            {"FX18", 0xF018, 0xF0FF, &Chip8VM::OpcodeFX18},
            {"FX1E", 0xF01E, 0xF0FF, &Chip8VM::OpcodeFX1E},
            {"FX29", 0xF029, 0xF0FF, &Chip8VM::OpcodeFX29},
            {"FX33", 0xF033, 0xF0FF, &Chip8VM::OpcodeFX33},
            {"FX55", 0xF055, 0xF0FF, &Chip8VM::OpcodeFX55},
            {"FX65", 0xF065, 0xF0FF, &Chip8VM::OpcodeFX65}};

    void LoadFontsIntoMemory() {
        for (uint8_t i = 0; i < fonts.size(); ++i) {
            mem_space[i] = fonts[i];
        }
    }

   public:
    VMViewer viewer_manager{};

    Chip8VM() {
        frame_buffer.resize(64 * 32, 0);  // Chip8 display resolution is 64x32
        mem_space.resize(4096, 0);
        LoadFontsIntoMemory();
    }

    ~Chip8VM() {}

    bool ReadGameImage(const char*);
    void ExecutionLoop();

    void StopExecutionLoop() {
        is_stopped = 1;
    }

    bool IsReloadRequested() {
        return viewer_manager.keyboard.request_reload;
    }

    /** Opcode handler functions
     * Opcode0NNN is ignored by modern interpreters:
     */
    void Opcode0NNN(uint16_t opcode);
    void Opcode00E0(uint16_t opcode);
    void Opcode00EE(uint16_t opcode);
    void Opcode1NNN(uint16_t opcode);
    void Opcode2NNN(uint16_t opcode);
    void Opcode3XNN(uint16_t opcode);
    void Opcode4XNN(uint16_t opcode);
    void Opcode5XY0(uint16_t opcode);
    void Opcode6XNN(uint16_t opcode);
    void Opcode7XNN(uint16_t opcode);
    void Opcode8XY0(uint16_t opcode);
    void Opcode8XY1(uint16_t opcode);
    void Opcode8XY2(uint16_t opcode);
    void Opcode8XY3(uint16_t opcode);
    void Opcode8XY4(uint16_t opcode);
    void Opcode8XY5(uint16_t opcode);
    void Opcode8XY6(uint16_t opcode);
    void Opcode8XY7(uint16_t opcode);
    void Opcode8XYE(uint16_t opcode);
    void Opcode9XY0(uint16_t opcode);
    void OpcodeANNN(uint16_t opcode);
    void OpcodeBNNN(uint16_t opcode);
    void OpcodeCXNN(uint16_t opcode);
    void OpcodeDXYN(uint16_t opcode);
    void OpcodeEX9E(uint16_t opcode);
    void OpcodeEXA1(uint16_t opcode);
    void OpcodeFX07(uint16_t opcode);
    void OpcodeFX0A(uint16_t opcode);
    void OpcodeFX15(uint16_t opcode);
    void OpcodeFX18(uint16_t opcode);
    void OpcodeFX1E(uint16_t opcode);
    void OpcodeFX29(uint16_t opcode);
    void OpcodeFX33(uint16_t opcode);
    void OpcodeFX55(uint16_t opcode);
    void OpcodeFX65(uint16_t opcode);
};