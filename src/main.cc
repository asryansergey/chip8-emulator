#include <SDL2/SDL.h>

#include <cstdio>

#include "emulator_vm.h"

int main() {
    VMDisplayDrawer vm_display{};
    vm_display.CreateDisplay();

    return 0;
}
