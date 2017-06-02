cmake_minimum_required(VERSION 3.2)

project(chip8asm)

add_subdirectory(src)
add_subdirectory(test)

enable_testing()

add_test(NAME Test COMMAND chip8asmTest)

set(CMAKE_C_FLAGS "--std=c99 -Wall -Wextra")

message("Compiling with: ${CMAKE_C_COMPILER} ${CMAKE_C_FLAGS}")
