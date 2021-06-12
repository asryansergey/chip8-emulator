#include <SDL2/SDL.h>

#include <cstdio>

#include "vm_cpu.h"

int RunVMThread(void* vm_ptr) {
    Chip8VM* vm = static_cast<Chip8VM*>(vm_ptr);
    vm->ExecutionLoop();
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("%s\n", "[!] Game image path is not specified.");
        return 1;
    }
    /**
     * TODO(asryansergey): Can create an abstraction layer for managing
     * the emulator (start, pause/stop, reset, etc.) insted of doing like this.
     * [For now, just simply press SPACE to reload the game without rerunning the binary)]
     */
    for (;;) {
        auto vm = make_unique<Chip8VM>();
        if (!vm->ReadGameImage(argv[1])) {
            printf("%s\n", "[!] Couldn't read game image!");
            return 1;
        }

        thread vm_thread(RunVMThread, vm.get());
        vm->viewer_manager.CreateDisplay();

        vm->StopExecutionLoop();
        vm_thread.join();
        if (!vm->IsReloadRequested()) {
            break;
        }
    }
    return 0;
}
