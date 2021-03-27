cmake_minimum_required(VERSION 3.10)

project(chip8Eml)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

list(APPEND SDL2_LIBS ${PROJECT_SOURCE_DIR}/libs/sdl2-2.0.14/lib)
list(APPEND SDL2_INCLUDES ${PROJECT_SOURCE_DIR}/libs/sdl2-2.0.14/include)

link_directories(${SDL2_LIBS})

add_executable(chip8Eml ${PROJECT_SOURCE_DIR}/src/main.cpp)

target_link_libraries(chip8Eml PUBLIC SDL2-2.0.0)
target_include_directories(chip8Eml PUBLIC ${SDL2_INCLUDES})