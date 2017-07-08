cmake_minimum_required(VERSION 3.2)
project(chip8asm)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic")
message("Compiling with: ${CMAKE_CXX_COMPILER} ${CMAKE_CXX_FLAGS}")

# Gather source files
include_directories(include)
file(GLOB SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/*.cpp")

# Look for CMake and decide whether to run unit tests
find_package(UnitTest++)

# Build the test suite if we found UnitTest++
if (${UnitTest++_FOUND})
    message("UnitTest++ found: ${UnitTest++_DIR}. Building unit tests...")

    file(GLOB TEST_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} test/*.cpp test/*.h)
    include_directories(${UTPP_INCLUDE_DIRS})
    add_executable(Testchip8asm ${TEST_SRCS})
    target_link_libraries(Testchip8asm UnitTest++)

    # run unit tests as a post build step
    add_custom_command(TARGET Testchip8asm POST_BUILD COMMAND Testchip8asm
        COMMENT "Running unit tests...")

else ()
    message("UnitTest++ was not found so unit tests will not be built and run!")
endif()

add_executable(chip8asm ${SOURCES})
