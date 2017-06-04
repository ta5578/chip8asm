#pragma once

#include "vm_def.h"
#include "asmparser.h"

typedef struct {
    char name[CHIP8_MAX_LABEL_NAME_LENGTH + 1];
    size_t len;
    uint16_t addr;    
} Label;

typedef enum { 
    START = 0, 
    OPERATOR 
} State;

typedef struct {
    AsmParser *parser;
    Op *op_table;
    Label labels[100];
    uint16_t current_addr;    
    uint8_t label_count;
    State state;
} BinGenerator;

/*
 * Initializes the label structure with the provided label name, lenght,
 * and the address for the label. Note that the ':' in the label name is
 * discarded as it serves to be a token identifier and not the actual
 * label name.
 */
void init_label(Label *label, const char *name, size_t len, uint16_t addr);

void init_generator(BinGenerator *gen, AsmParser *parser);

/*
 * Generates the binary ROM file given the generator and the program
 * arguments
 */
bool generate_bin(BinGenerator *gen, AsmOpts *opts);

bool process_token(BinGenerator *gen, const char *token, size_t token_size);

bool is_operator(const char *token, size_t size, const Op *ops);

bool is_label(const char *token, size_t size);

bool is_operand(const char *token, size_t size, const char *last_op, size_t op_size);
