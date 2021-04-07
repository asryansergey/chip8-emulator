#include "emulator_vm.h"

bool Chip8VM::ReadGameImage(const char* input_name) {
    std::unique_ptr<FILE, FileDeleter> f;
    f.reset(fopen(input_name, "rb"));
    if (f == nullptr) {
        printf("%s %d\n", "Could not open input image! Error code: ", errno);
        return false;
    }
    return fread(mem_space + kOffsetAddress, 1, 4096 - kOffsetAddress, f.get()) > 0;
}