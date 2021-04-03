# Chip-8 Emulator

Implementing CHIP-8 emulator because I want to know how it works and also, it is fun.
By all means this is not supposed to be the most efficient implementation.



### Issues with using SDL2
#### MacOS
       
    When building SDL2 from source, you have probably got this issue after running the binary:

        dyld: Library not loaded: PATH_TO_LIB/libSDL2-2.0.0.dylib
        Referenced from: ./your_binary
        Reason: image not found

    To resolve this you can create symbolic link to libSDL2-2.0.0.dylib in /usr/local/lib
