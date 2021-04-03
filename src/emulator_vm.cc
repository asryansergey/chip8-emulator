#include "emulator_vm.h"

bool Chip8VM::ReadGameImage(const char* input_name) {
    // https://dev.krzaq.cc/post/you-dont-need-a-stateful-deleter-in-your-unique_ptr-usually/
    struct FileDeleter {
        void operator()(FILE* ptr) const {
            fclose(ptr);
        }
    };

    std::unique_ptr<FILE, FileDeleter> f;
    f.reset(fopen(input_name, "rb"));
    if (f == nullptr) {
        printf("%s %d\n", "Could not open input image! Error code: ", errno);
        return false;
    }
    return fread(memory + kOffsetAddress, 1, 4096 - kOffsetAddress, f.get()) > 0;
}