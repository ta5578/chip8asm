cmake_minimum_required(VERSION 3.2)

project(chip8asm)

add_subdirectory(src)
#add_subdirectory(test)

#enable_testing()

#add_test(NAME Test COMMAND chip8asmTest)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "-std=c++14 -Wall -Wextra -Wpedantic")

message("Compiling with: ${CMAKE_CXX_COMPILER} ${CMAKE_CXX_FLAGS}")
