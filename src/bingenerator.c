#include "bingenerator.h"
#include "asmparser.h"
#include <assert.h>
#include <ctype.h>
#include "utils.h"

static Op* init_op_table()
{
    static Op op_table[] = { 
         { .name = "SYS", .name_len = 3, .argc = 1},
         { .name = "CLR", .name_len = 3, .argc = 0},
         { .name = "RET", .name_len = 3, .argc = 0},
         { .name = "JMP", .name_len = 3, .argc = 1},
         { .name = "CALL", .name_len = 3, .argc = 1},
         { .name = "SKE", .name_len = 3, .argc = 2},
         { .name = "SKNE", .name_len = 4, .argc = 2},
         { .name = "SKRE", .name_len = 4, .argc = 2},
         { .name = "LOAD", .name_len = 4, .argc = 2},
         { .name = "ADD", .name_len = 3, .argc = 2},
         { .name = "ASN", .name_len = 3, .argc = 2},
         { .name = "OR", .name_len = 2, .argc = 2},
         { .name = "AND", .name_len = 3, .argc = 2},
         { .name = "XOR", .name_len = 3, .argc = 2},
         { .name = "ADDR", .name_len = 4, .argc = 2},
         { .name = "SUB", .name_len = 3, .argc = 2},
         { .name = "SHR", .name_len = 3, .argc = 1},
         { .name = "SUBR", .name_len = 4, .argc = 2},
         { .name = "SHL", .name_len = 3, .argc = 1},
         { .name = "SKRNE", .name_len = 5, .argc = 2},
         { .name = "LOADI", .name_len = 5, .argc = 1},
         { .name = "JMP0", .name_len = 4, .argc = 1},
         { .name = "RAND", .name_len = 4, .argc = 2},
         { .name = "DRAW", .name_len = 4, .argc = 3},
         { .name = "SKK", .name_len = 3, .argc = 1},
         { .name = "SKNK", .name_len = 4, .argc = 1},
         { .name = "DELA", .name_len = 4, .argc = 1},
         { .name = "KEYW", .name_len = 4, .argc = 1},
         { .name = "DELR", .name_len = 4, .argc = 1},
         { .name = "SNDR", .name_len = 4, .argc = 1},
         { .name = "ADDI", .name_len = 4, .argc = 1},
         { .name = "SILS", .name_len = 4, .argc = 1},
         { .name = "BCD", .name_len = 3, .argc = 1},
         { .name = "DUMP", .name_len = 4, .argc = 1},
         { .name = "DUMPI", .name_len = 5, .argc = 1},
         { .name = "LB", .name_len = 2, .argc = 1}
    };
    return op_table;
}

void init_label(Label *label, const char *name, size_t len, uint16_t addr)
{
    assert(label); assert(name); assert(len < CHIP8_MAX_LABEL_NAME_LENGTH);

    label->name[len - 1] = '\0';
    strncpy(label->name, name, len);
    label->addr = addr;
}

void init_generator(BinGenerator *gen, AsmParser *parser)
{
    assert(gen); assert(parser);

    gen->parser = parser;
    gen->op_table = init_op_table();
    gen->current_addr = 0;
    gen->label_count = 0;
}

bool generate_bin(BinGenerator *gen, AsmOpts *opts)
{
    assert(gen); assert(opts);

    const char *token = NULL;
    size_t token_size = 0;
    bool success = 0;
    do {
        get_next_token(gen->parser, &token, &token_size);
        success = process_token(gen, token, token_size);
    } while (success && token_size > 0);
    return success;
}

bool process_token(BinGenerator *gen, const char *token, size_t size)
{
    assert(gen); assert(token);
    return true;

    if (size == 0) {
        return true;
    }

    typedef enum { START = 0, OPERATOR } State;
    static State state = START;
    static const char *last_op = NULL;
    static size_t last_op_size = 0;

    switch (state) {
    /* In the start state, we expect either a label or operator */
    case START:
        if (!is_operator(token, size, gen->op_table)) {
            if (!is_label(token, size)) {
                error_msg(token, size, " is not a valid operator or label!");   
                return false;
            } else {
                stdout_msg(token, size, " recognized as a label!");
                Label label;
                init_label(&label, token, size - 1, gen->current_addr);
                gen->labels[gen->label_count++] = label;
            }
        } else {
            stdout_msg(token, size, " recognized as an operator!");
            last_op = token;
            last_op_size = size;
            state = OPERATOR;
        }
        break;
    case OPERATOR:
        if (!is_operand(token, size, last_op, last_op_size)) {
            print_token(token, size);
            fprintf(stderr, " is not a valid operand for operator ");
            print_token(last_op, last_op_size);
            putchar('\n');
            return false;
        } else {
            print_token(token, size);
            fprintf(stdout, " is recognized as an operand for operator ");
            print_token(last_op, last_op_size);
            putchar('\n');
            state = START;
        }
        break;
    }
    return true;
}

/*
 * TODO: linear search will be done on each operator. This is possibly
 * very inefficient. Profile this and if it is a performance bottleneck in
 * parsing, look into making a look up table using hashing. For
 * now though, this will suffice.
 */
bool is_operator(const char *token, size_t size, const Op *op_table)
{
    for (int i = 0; i < CHIP8_OPCODE_COUNT; ++i) {
        if (streq(token, size, op_table[i].name, op_table[i].name_len)) {
            return true;
        }
    }
    return false;
}

bool is_label(const char *token, size_t size)
{
    return token[size - 1] == ':';
}

bool is_operand(const char *token, size_t size, const char *last_op, size_t op_size)
{
    const char *end = token + size;
    if (streq(last_op, op_size, "LOAD", 4)) {
        /* In the LOAD command, we need operands of the form
         * [rR][0-15],$[0-F]{4}
         */
       if (tolower(*token) != 'r') {
           return false;
       }
       ++token;

       /* Horner's rule to get first register number */
       uint8_t reg1 = 0;
       while (token != end && *token != ',') {
           reg1 *= 10;
           reg1 += *token++ - '0';
       } 

       /* Registers should only be 0x0 to 0xF and occupy two spaces */
       if (reg1 > 0xF) {
           return false;
       }

       /* After the first register should be a comma */
       if (*token++ != ',') {
           return false;
       }
       
       /* Then a dollar sign */
       if (*token++ != '$') {
           return false;
       }

       uint8_t value = 0;
       while (token != end) {
           value *= 10;
           if (*token >= 'A' && *token <= 'F') {
                value += *token - 'A' + 10;
           } else if (isdigit(*token)) {
                value += *token - '0';
           } else {
               return false;
           }
           ++token;
       }
       return true;
    } else if (streq(last_op, op_size, "LOADI", 5)) {
        /* The LOADI command expects a label */
     //   for (size_t i = 0; i < num_label; ++i) {
      //      if (streq(token, size, labels[i].name, 5)) {
       //         return true;
        //    }
        //}
        return true;
    }
    return false;
}
