#include <SDL2/SDL.h>

#include <cstdio>
#include <memory>
#include <thread>

#include "emulator_vm.h"

int main(int argc, char* argv[]) {
    // VMDisplayDrawer vm_display{};
    // vm_display.CreateDisplay();
    if (argc != 2) {
        printf("%s\n", "[!] Game image path is not specified.");
        return 1;
    }
    /*
     * TODO (asryansergey): Use different threads for Executing emulator
     * and Drawing onto screen
     **/
    Chip8VM vm;
    /**
     * TODO (asryansergey): Change the resource handling while
     * passing object to thread
    */
    if (!vm.ReadGameImage(argv[1])) {
        printf("%s\n", "[!] Couldn't read game image!");
        return 1;
    }
    thread vm_thread(&Chip8VM::ExecutionLoop, vm);
    VMDisplayDrawer vm_display{};
    vm_display.CreateDisplay();
    vm_thread.join();
    return 0;
}
