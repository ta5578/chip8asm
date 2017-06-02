#pragma once

#include <stdint.h>
#include <stdbool.h>

/* Define some useful macros */
#define COMBINE(A,B) A##B
#define CONCAT(A,B) COMBINE(A,B)

#define CHIP8_MAX_OPCODE_NAME_LENGTH (5)

#define CHIP8_MAX_OPCODE_ARG_LENGTH (3)

/* Although the official VM supports 35 instructions, 
 * I've added an additional convenience instruction (LB).
 */
#define CHIP8_OPCODE_COUNT (36)

typedef struct {
    char name[CHIP8_MAX_OPCODE_NAME_LENGTH + 1];
    uint8_t name_len;
    uint8_t argc;
} Op;


typedef struct {
    char *in_file, *out_file;
    bool dump_ops, dump_asm;    
} AsmOpts;
