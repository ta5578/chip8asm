cmake_minimum_required(VERSION 3.2)
project(chip8asm)

set(CMAKE_CXX_STANDARD 14)

# Set additional compiler flags and link directories
if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /WX /D_CRT_SECURE_NO_WARNINGS")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Werror -Wno-unused-result")
else()
    message(FATAL_ERROR "${CMAKE_CXX_COMPILER_ID} is not supported!")
endif()

# Set the build type (this only affects single output generators)
if(CMAKE_GENERATOR MATCHES "Unix Makefiles")
    if(CMAKE_BUILD_TYPE MATCHES "")
        message(WARNING "CMAKE_BUILD_TYPE was not specified. The build will default to a 'release' build type. Note that this only affects single configuration generators like Make and Ninja.")
        set(CMAKE_BUILD_TYPE "release")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE}")
    else()
        message(STATUS "CMAKE_BUILD_TYPE set to ${CMAKE_BUILD_TYPE}")
        if(CMAKE_BUILD_TYPE MATCHES "debug")
            set(CMAKE_BUILD_TYPE "debug")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG}")
        elseif(CMAKE_BUILD_TYPE MATCHES "release")
            set(CMAKE_BUILD_TYPE "release")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE}")
        else()
            message(FATAL_ERROR "${CMAKE_BUILD_TYPE} is not supported for this generator! The only accepted values are 'debug' or 'release'!")
        endif()
    endif()
endif()

message("Compiling with: ${CMAKE_CXX_COMPILER} ${CMAKE_CXX_FLAGS}")

# Set up the output directory for builds
set(CHIP8ASM_OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/build")

# Set the generic runtime output directory for single generators like "make"
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CHIP8ASM_OUTPUT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CHIP8ASM_OUTPUT_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CHIP8ASM_OUTPUT_DIR})

# Set the generic runtime output directory for multi generators like "msvc"
foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CHIP8ASM_OUTPUT_DIR})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CHIP8ASM_OUTPUT_DIR})
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CHIP8ASM_OUTPUT_DIR})
endforeach(OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES)

message("Current output directory ${CHIP8ASM_OUTPUT_DIR}")

# Gather source files
include_directories(include)
include_directories(.)
file(GLOB SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/*.cpp")
file(GLOB HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "include/*.h")

# Create a static library from source
add_library(libchip8asm STATIC ${HEADERS} ${SOURCES})

enable_testing()

# Build the test suite
set(CHIP8ASM_TEST_SOURCES "test/catch.hpp" "test/tests.cpp")
add_executable(testchip8asm ${HEADERS} ${CHIP8ASM_TEST_SOURCES})
target_link_libraries(testchip8asm libchip8asm)

add_test(chip8asm-test ${CHIP8ASM_OUTPUT_DIR}/testchip8asm)

add_executable(chip8asm ${HEADERS} ${SOURCES})
