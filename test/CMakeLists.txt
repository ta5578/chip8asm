include_directories(${chip8asm_SOURCE_DIR}/src ${chip8asm_SOURCE_DIR}/include)

file(GLOB TEST_SOURCES "*.c")
add_definitions(-DDEBUG)
add_executable(chip8asmTest ${TEST_SOURCES})
target_link_libraries(chip8asmTest libchip8asm)
