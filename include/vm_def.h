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


typedef struct {
    char name[CHIP8_MAX_OPCODE_NAME_LENGTH + 1];
    uint8_t name_len;
    uint8_t argc;
} Op;


typedef struct {
    char *in_file, *out_file;
    bool dump_asm, show_help;    
} AsmOpts;

static void init_opts(AsmOpts *opts)
{
    assert(opts);
    opts->in_file = NULL;
    opts->out_file = "a.rom";
    opts->dump_asm = false;
    opts->show_help = false;
}
