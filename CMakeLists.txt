cmake_minimum_required(VERSION 3.2)

project(chip8asm)

include_directories(include)

file(GLOB_RECURSE SOURCES "src/*.c")

add_executable(chip8asm ${SOURCES})

set(CMAKE_C_FLAGS "--std=c99 -Wall -Wextra")

message("Compiling with: ${CMAKE_C_COMPILER} ${CMAKE_C_FLAGS}")
