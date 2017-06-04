#pragma once

#include "vm_def.h"
#include "asmparser.h"

typedef struct {
    char name[CHIP8_MAX_LABEL_NAME_LENGTH + 1];
    uint16_t addr;    
} Label;

typedef struct {
    AsmParser *parser;
    Op *op_table;
    Label labels[100];
    uint16_t current_addr;    
    uint8_t label_count;
    const char *out_file;
} BinGenerator;

void init_label(Label *label, const char *name, size_t len, uint16_t addr);

void init_generator(BinGenerator *gen, AsmParser *parser);

bool generate_bin(BinGenerator *gen, AsmOpts *opts);

bool process_token(BinGenerator *gen, const char *token, size_t token_size);

bool is_operator(const char *token, size_t size, const Op *ops);

bool is_label(const char *token, size_t size);

bool is_operand(const char *token, size_t size, const char *last_op, size_t op_size);
