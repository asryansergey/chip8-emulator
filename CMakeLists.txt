cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(chip8Eml)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

list(APPEND SDL2_LIBS ${PROJECT_SOURCE_DIR}/libs/sdl2-2.0.14/lib)
list(APPEND SDL2_INCLUDES ${PROJECT_SOURCE_DIR}/libs/sdl2-2.0.14/include)

list(APPEND VM_INCLUDES ${PROJECT_SOURCE_DIR}/src/)

link_directories(${SDL2_LIBS})
file(GLOB SOURCES "${PROJECT_SOURCE_DIR}/src/*.cc")

add_executable(chip8Eml ${SOURCES})

target_link_libraries(chip8Eml PUBLIC SDL2-2.0.0)
target_include_directories(chip8Eml PUBLIC ${SDL2_INCLUDES})
target_include_directories(chip8Eml INTERFACE PRIVATE ${EmulatorVM})

# Add CMakeCache.txt as a target for make clean
set_target_properties(chip8Eml PROPERTIES ADDITIONAL_CLEAN_FILES
    "${CMAKE_BINARY_DIR}/CMakeCache.txt")