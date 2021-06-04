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

void Chip8VM::ExecutionLoop() {
    for (;;) {
        if (is_stopped) {
            break;
        }
        if (pc + 1 > 4096) {
            printf("PC is out of bound (%.4x)\n", pc);
            return;
        }
        uint16_t opcode = mem_space.at(pc) << 8;  // To ensure Big Endianness
        opcode |= mem_space.at(pc + 1);
        pc += 2;  // Increment PC to point to next instruction
        for (const auto& entry : opcode_entry) {
            if ((opcode & entry.mask) == entry.opcode) {
                (this->*(entry.handler))(opcode);
                break;
            }
        }
        this_thread::sleep_for(2ms);
    }
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
    pc = call_stack.top();
    call_stack.pop();
}

void Chip8VM::Opcode1NNN(uint16_t opcode) {
    uint16_t jump_addr = GetValueNNN(opcode);
    pc = jump_addr;
}

void Chip8VM::Opcode2NNN(uint16_t opcode) {
    uint16_t value = GetValueNNN(opcode);
    call_stack.push(pc);
    pc = (value & 0xfff);
}
void Chip8VM::Opcode3XNN(uint16_t opcode) {
    unsigned id_x = GetValueX(opcode);
    if (v[id_x] == GetValueNN(opcode)) {
        pc += 2;
    }
}
void Chip8VM::Opcode4XNN(uint16_t opcode) {
    unsigned id_x = GetValueX(opcode);
    if (v[id_x] != GetValueNN(opcode)) {
        pc += 2;
    }
}
void Chip8VM::Opcode5XY0(uint16_t opcode) {
    unsigned id_x = GetValueX(opcode);
    unsigned id_y = GetValueY(opcode);
    if (v[id_x] == v[id_y]) {
        pc += 2;
    }
}

void Chip8VM::Opcode6XNN(uint16_t opcode) {
    unsigned id_x = GetValueX(opcode);
    v[id_x] = GetValueNN(opcode);
}

void Chip8VM::Opcode7XNN(uint16_t opcode) {
    unsigned id_x = GetValueX(opcode);
    v[id_x] += GetValueNN(opcode);
}

void Chip8VM::Opcode8XY0(uint16_t opcode) {
    unsigned id_x = GetValueX(opcode);
    unsigned id_y = GetValueY(opcode);
    v[id_x] = v[id_y];
}
void Chip8VM::Opcode8XY1(uint16_t opcode) {
    unsigned id_x = GetValueX(opcode);
    unsigned id_y = GetValueY(opcode);
    v[id_x] |= v[id_y];
}
void Chip8VM::Opcode8XY2(uint16_t opcode) {
    unsigned id_x = GetValueX(opcode);
    unsigned id_y = GetValueY(opcode);
    v[id_x] &= v[id_y];
}
void Chip8VM::Opcode8XY3(uint16_t opcode) {
    unsigned id_x = GetValueX(opcode);
    unsigned id_y = GetValueY(opcode);
    v[id_x] ^= v[id_y];
}
void Chip8VM::Opcode8XY4(uint16_t opcode) {
    unsigned id_x = GetValueX(opcode);
    unsigned id_y = GetValueY(opcode);
    uint16_t res = static_cast<uint16_t>(v[id_x]) + static_cast<uint16_t>(v[id_y]);
    v[0xf] = (res >= 0x100);
    v[id_x] = res & 0xff;
}
void Chip8VM::Opcode8XY5(uint16_t opcode) {
    unsigned id_x = GetValueX(opcode);
    unsigned id_y = GetValueY(opcode);
    v[0xf] = (v[id_x] >= v[id_y]);
    v[id_x] -= v[id_y];
}
void Chip8VM::Opcode8XY6(uint16_t opcode) {
    unsigned id_x = GetValueX(opcode);
    unsigned id_y = GetValueY(opcode);
    v[0xf] = v[id_x] & 0x1;
    v[id_x] = v[id_y] >> 1;  // FIXME (asryansergey): Some confusion around whether shift vx or vy
}
void Chip8VM::Opcode8XY7(uint16_t opcode) {
    unsigned id_x = GetValueX(opcode);
    unsigned id_y = GetValueY(opcode);
    v[0xf] = (v[id_y] >= v[id_x]);
    v[id_x] = v[id_y] - v[id_x];
}
void Chip8VM::Opcode8XYE(uint16_t opcode) {
    unsigned id_x = GetValueX(opcode);
    unsigned id_y = GetValueY(opcode);
    v[0xf] = v[id_x] >> 7;
    v[id_x] = v[id_y] << 1;  // FIXME (asryansergey): Some confusion around whether shift vx or vy
}
void Chip8VM::Opcode9XY0(uint16_t opcode) {
    unsigned id_x = GetValueX(opcode);
    unsigned id_y = GetValueY(opcode);
    if (v[id_x] != v[id_y]) {
        pc += 2;
    }
}

void Chip8VM::OpcodeANNN(uint16_t opcode) {
    i = GetValueNNN(opcode);
}

void Chip8VM::OpcodeBNNN(uint16_t opcode) {
    const uint16_t value = GetValueNNN(opcode);
    pc = (v[0] + value) & 0xfff;
}
void Chip8VM::OpcodeCXNN(uint16_t opcode) {
    unsigned id_x = GetValueX(opcode);
    const uint16_t value = GetValueNN(opcode);
    v[id_x] = (rand() & value) & 0xff;
}

void Chip8VM::OpcodeDXYN(uint16_t opcode) {
    const int nibble{GetValueN(opcode)};
    const uint8_t vx = v[GetValueX(opcode)] % 64;  // Wrapping around to the
    uint8_t vy = v[GetValueY(opcode)] % 32;        // opposite side of the screen.

    bool is_flipped = false;
    uint8_t pixel_value{};
    for (int i = 0; i < nibble; ++i, vy++) {
        pixel_value = mem_space.at(this->i + i);
        for (int j = 0; j < 8; ++j) {
            uint8_t bit = ((pixel_value >> (7 - j)) & 1);
            if (bit && frame_buffer.at(vx + j + vy * 64)) {
                is_flipped |= true;
            }
            frame_buffer.at(vx + j + vy * 64) ^= bit;
        }
    }
    v[0xf] = is_flipped;
    screen_map.RedrawScreen(frame_buffer);
}

void Chip8VM::OpcodeEX9E(uint16_t /*opcode*/) {}
void Chip8VM::OpcodeEXA1(uint16_t /*opcode*/) {}
void Chip8VM::OpcodeFX07(uint16_t /*opcode*/) {}
void Chip8VM::OpcodeFX0A(uint16_t /*opcode*/) {}
void Chip8VM::OpcodeFX15(uint16_t /*opcode*/) {}
void Chip8VM::OpcodeFX18(uint16_t /*opcode*/) {}
void Chip8VM::OpcodeFX1E(uint16_t /*opcode*/) {}
void Chip8VM::OpcodeFX29(uint16_t /*opcode*/) {}
void Chip8VM::OpcodeFX33(uint16_t opcode) {
    unsigned id_x = GetValueX(opcode);
    uint8_t bcd = v[id_x];
    int insert_pos = i + 2;
    while (bcd) {
        mem_space.at(insert_pos--) = bcd % 10;
        bcd /= 10;
    }
}
void Chip8VM::OpcodeFX55(uint16_t opcode) {
    unsigned id_x = GetValueX(opcode);
    for (unsigned i = 0; i <= id_x; ++i) {
        mem_space.at(this->i + i) = v[i];
    }
}
void Chip8VM::OpcodeFX65(uint16_t opcode) {
    unsigned id_x = GetValueX(opcode);
    for (unsigned i = 0; i <= id_x; ++i) {
        v[i] = mem_space.at(this->i + i);
    }
}