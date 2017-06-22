#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#define CHIP8_MAX_OPCODE_NAME_LENGTH (5)

#define CHIP8_MAX_OPCODE_ARG_LENGTH (3)

/* Although the official VM supports 35 instructions,
 * I've added an additional convenience instruction (LB).
 */
#define CHIP8_OPCODE_COUNT (36)

#define CHIP8_MAX_LABEL_NAME_LENGTH (32)


struct AsmOpts {
    const char *in_file, *out_file;
    bool dump_asm, show_help;

    AsmOpts(const char *in = "", const char *out = "", bool dump_asm = false, bool show_help = false) :
        in_file(in), out_file(out), dump_asm(dump_asm), show_help(show_help) {}
};