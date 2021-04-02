#include "EmulatorVM.hpp"

struct FileDeleter {
    void operator()(FILE* ptr) const {
        fclose(ptr);
    }
};

bool Chip8VM::ReadGameImage(const char* input_name) {
    std::unique_ptr<FILE, FileDeleter> f;
    f.reset(fopen(input_name, "rb"));
    if (f == nullptr) {
        printf("%s %d\n", "Could not open input image! Error code: ", errno);
        return false;
    }
    return fread(this->memory + this->offset_address, 1, 4096 - this->offset_address, f.get()) > 0;
}