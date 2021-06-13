# Chip-8 Emulator

Implementing CHIP-8 emulator because I want to know how it works and also, it is fun.
By all means, this is not supposed to be the most efficient implementation.

## How to run
This code was developed with `C++17`, however it wasn't meant to use all bleeding edge features of that specification.

To build the project
```bash
mkdir build
cd build
cmake .. && make # Use `make clean` for clean up.
```
*This will create `chip8Eml` binary in `/bin` directory inside `root` directory.*

Run
```bash
./chip8Eml PATH_TO_ROM
```

## Functionality
There are just 2 basic operations you can do after running the Emulator
* Press `ESC` to exit the emulator
* Press `SPACE` to reload the ROM

## Graphical Library

SDL2 was used as the library to draw graphics on the screen. Inside subdirectory `libs/sdl2-2.0.14` the
local build of SDL2 library should be placed *(in case of issues on MacOS see below)*.

*The code isn't tightly coupled with SDL2, so changing it to different library shouldn't be
difficult.*


## Resources used

* [Chip-8 Wiki](https://en.wikipedia.org/wiki/CHIP-8)
* [Cowgod's Chip-8 Technical Reference](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM)
* [Gynvael Coldwind's livestreams](https://www.youtube.com/channel/UCCkVMojdBWS-JtH7TliWkVg)
* [Guide to making a CHIP-8 emulator](https://tobiasvl.github.io/blog/write-a-chip-8-emulator/)

## Samples

The `sample_roms` directory contains games and programs that are using the small number of opcodes and just drawing text on the screen.
More examples could be found [here](https://github.com/kripod/chip8-roms/tree/master/games).

## Issues with using SDL2
#### MacOS
       
    When building SDL2 from source, you have probably got this issue after running the binary:

        dyld: Library not loaded: PATH_TO_LIB/libSDL2-2.0.0.dylib
        Referenced from: ./your_binary
        Reason: image not found

    To resolve this, you can create a symbolic link to libSDL2-2.0.0.dylib in /usr/local/lib

## Todo
* *Add sound timer*
* *Add support for debugging the emulator.*

