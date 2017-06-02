include_directories(${chip8asm_SOURCE_DIR}/include)

file(GLOB_RECURSE SOURCES "*.c")
file(GLOB_RECURSE INCLUDES "${chip8asm_SOURCE_DIR}/*.h")

add_library(libchip8asm ${SOURCES} ${INCLUDES})
add_executable(chip8asm ${SOURCES})
target_link_libraries(chip8asm libchip8asm)
