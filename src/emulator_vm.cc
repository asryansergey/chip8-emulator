#include "emulator_vm.h"

bool Chip8VM::ReadGameImage(const char* input_name) {
    std::unique_ptr<FILE, FileDeleter> f;
    f.reset(fopen(input_name, "rb"));
    if (f == nullptr) {
        printf("%s %d\n", "Could not open input image! Error code: ", errno);
        return false;
    }
    return fread(&mem_space[0] + kOffsetAddress, 1, 4096 - kOffsetAddress, f.get()) > 0;
}

uint16_t Chip8VM::GetValueX(uint16_t opcode) const {
    return (opcode & 0xf00) >> 8;
}

uint16_t Chip8VM::GetValueY(uint16_t opcode) const {
    return (opcode & 0xf0) >> 4;
}

uint16_t Chip8VM::GetValueN(uint16_t opcode) const {
    return opcode & 0xf;
}

uint16_t Chip8VM::GetValueNN(uint16_t opcode) const {
    return opcode & 0xff;
}

uint16_t Chip8VM::GetValueNNN(uint16_t opcode) const {
    return opcode & 0xfff;
}

void Chip8VM::Opcode00E0(uint16_t /*opcode*/) {
    std::for_each(frame_buffer.begin(), frame_buffer.end(), [](uint8_t& elem) {
        elem = 0;
    });
}

void Chip8VM::Opcode00EE(uint16_t /*opcode*/) {
    call_stack.push(pc);
}

void Chip8VM::Opcode1NNN(uint16_t opcode) {
    uint16_t jump_addr = GetValueNNN(opcode);
    pc = jump_addr;
}

void Chip8VM::Opcode6XNN(uint16_t opcode) {
    unsigned int idx = GetValueX(opcode);
    v[idx] = GetValueNN(opcode);
}

void Chip8VM::Opcode7XNN(uint16_t opcode) {
    unsigned int idx = GetValueX(opcode);
    v[idx] += GetValueNN(opcode);
}

void Chip8VM::OpcodeANNN(uint16_t opcode) {
    i = GetValueNNN(opcode);
}

void Chip8VM::OpcodeDXYN(uint16_t opcode) {
    const int nibble{GetValueN(opcode)};
    const uint8_t vx = v[GetValueX(opcode)] % 64;  // Wrapping around to the
    uint8_t vy = v[GetValueY(opcode)] % 32;        // opposite side of the screen.

    bool is_flipped = false;
    uint8_t pixel_value{};
    for (int i = 0; i <= nibble; ++i, vy++) {
        pixel_value = mem_space.at(this->i + i);
        for (int j = 0; j < 8; ++j) {
            uint8_t bit = ((pixel_value >> j) & 1);
            if (bit && frame_buffer.at(vx + j + vy * 64)) {
                is_flipped |= true;
            }
            frame_buffer.at(vx + j + vy * 64) ^= bit;
        }
    }
    v[0xf] = is_flipped;

    screen_map.RedrawScreen(frame_buffer);
    // TODO(asryansergey): Probably need to redraw the whole screen in VMDisplayDrawer.
}