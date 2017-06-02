#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include "vm_def.h"
#include "asmparser.h"

static AsmOpts opts = {.in_file = NULL, .out_file = "a.rom", .dump_ops = false, .dump_asm = false };

/* TODO: this is global and anonymous. FIXME */
typedef struct {
    char name[32];
    uint16_t addr;
} Label;

static Label labels[100];
static uint8_t num_label = 0;
static uint16_t global_addr = 0x200;

Op* init_op_table()
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

void print_op_table(const Op *table)
{
    for (int i = 0; i < CHIP8_OPCODE_COUNT; ++i) {
        printf("Name: '%s', Expected Arguments: '%d'\n", table[i].name, table[i].argc);
    }
}

void print_token(const char *token, size_t size)
{
    for (size_t i = 0; i < size; ++i) {
        putchar(token[i]);
    }
}

/*
 * Perform a case insensitive comparison of two strings
 */
bool streq(const char *s1, size_t size1, const char *s2, size_t size2)
{
    if (size1 != size2) {
        return false;
    }
    for (size_t i = 0; i < size1; ++i) {
        if (tolower(s1[i]) != tolower(s2[i])) {
            return false;
        }
    }
    return true;
}

/*
 * Returns true of one of 's1' or 's2' matches the 'base' string
 */
bool alt_strcmp(const char *base, const char *s1, const char *s2)
{
    return strcmp(base, s1) == 0 || strcmp(base, s2) == 0;
}

/*
 * Set the 'slices' array to contain pointers to the character in question until
 * the length of slices desired is succeeded or we go over the entire token
 * string.
 *
 * For example, if the token is "r1,r2" and we wish to split by ',' then we the
 * pointers will point to:
 *
 * r1_,_r2
 *
 * Since the caller has a pointer to the beginning of the token and to the split
 * characters, an "in-place" string split can be achieved by iterating until you
 * hit a pointer in the 'slices' array.
 */
void strsplit(const char *token, size_t len, const char **slices, size_t max_slices, char c)
{
    assert(slices);

    size_t i = 0, slice_count = 0;
    while (i < len && slice_count < max_slices) {
        if (token[i++] == c) {
            slices[slice_count++] = token;
        }
    } 
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

void error_msg(const char *token, size_t size, const char *msg)
{
    for (size_t i = 0; i < size; ++i) {
        fprintf(stderr, "%c", token[i]);
    }
    fprintf(stderr, " %s\n", msg);
}

void stdout_msg(const char *token, size_t size, const char *msg)
{
    print_token(token, size);
    printf("%s\n", msg);
}

/*
 * Allocates a __null-terminated__ string buffer on the heap
 * capable of holding 'size' characters. If allocation fails,
 * the function returns NULL.
 */
char* smalloc(size_t size)
{
    char *s = malloc(size + 1);
    if (s) {
        s[size] = '\0';
    }
    return s;
}

/*
 * Returns the size of the file in bytes. The file stream
 * must not be null.
 */
size_t fsize(FILE *file)
{
    assert(file);

    /* Get where we are in the file first */
    size_t pos = ftell(file);

    /* Go to the beginning */
    fseek(file, 0, SEEK_SET);

    /* Now go to the end */
    fseek(file, 0, SEEK_END);

    /* Get the size of the file now (end - beginning) */
    size_t fsize = ftell(file);

    /* Rewind to where we were */
    fseek(file, pos, SEEK_SET);

    return fsize;
}

/*
 * Reads the entire contents of the file stream into a memory buffer.
 * The file stream and the buffer pointer must not be null. The function
 * will return the number of bytes read and set the 'buf' to point to a
 * heap-allocated string buffer whose size is returned by the function.
 *
 */
size_t read_file_into_mem(FILE *file, const char **buf)
{
    assert(file);
    assert(buf);

    size_t file_size = fsize(file);
    char *mem = smalloc(file_size);
    if (!mem) {
        return 0;
    } 

    size_t bytes_read = fread(mem, sizeof(*mem), file_size, file);
    *buf = mem;
   
    return bytes_read; 
}

bool process_token(const char *token, size_t size, const Op *op_table)
{
    assert(token);
    assert(op_table);

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
        if (!is_operator(token, size, op_table)) {
            if (!is_label(token, size)) {
                error_msg(token, size, " is not a valid operator or label!");   
                return false;
            } else {
                stdout_msg(token, size, " recognized as a label!");
                Label l;
                for (size_t i = 0; i < size - 1; ++i) {
                    l.name[i] = token[i];
                }
                l.name[size - 1] = '\0';
                l.addr = global_addr;
                labels[num_label++] = l;
                global_addr += 0x2;
                printf("Label '%s' mapped to address '%d'\n", l.name, l.addr);
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

bool generate_bin(const char *buf, size_t buf_size, const Op *op_table)
{
    assert(buf);
    assert(op_table);

    const char *token = NULL;
    size_t token_size = 0;
    bool success = 0;
    AsmParser parser;
    if (!init_parser(&parser, buf)) {
        fprintf(stderr, "Couldn't initialize the parser properly!\n");
        return false;
    }
    do {
        get_next_token(&parser, &token, &token_size);
        /* There's a bit of a kink here in that the very last
         * token will be empty and cause the below to report failure
         * if we don't explicitly check for an empty token
         */
        success = process_token(token, token_size, op_table);
    } while (success && token_size > 0);
    return success;
}

void show_help()
{
    puts("chip8asm is an assembler for the chip 8 VM.");
    puts("The only required argument is the input .asm file.");
    puts("The first argument should be one of: the input file, help, or -dops");
    puts("Here are the supported options:");
    puts("   --dump-asm | -dasm -- dumps the assembled statements with memory locations");
    puts("   --dump-ops | -dops -- dumps the supported op codes and exits successfully");
    puts("   --output | -o -- the name of the output ROM file. By default, it is 'a.rom'");
    puts("   --help | -h -- displays this help screen");
}

/* 
 * Parse the command line arguments and populate the opt
 * structure. Returns true if successful.
 */
bool parse_args(int argc, char **argv, AsmOpts *opts)
{
    assert(opts);

    if (argc < 2) {
        show_help();
        return false;
    }

    /* The first option should only be one of these */
    if (alt_strcmp(argv[1], "--dump-ops", "-dops")) {
       opts->dump_ops = true;
       return true;
    } else if (alt_strcmp(argv[1], "--help", "-h")) {
       show_help();
       opts->dump_ops = true;
       return true;
    } else {
       opts->in_file = argv[1];
    }

    for (int i = 2; i < argc; ++i) {
        if (alt_strcmp(argv[i], "--dump-asm", "-dasm")) {
            opts->dump_asm = true;
        } else if (alt_strcmp(argv[i], "--output", "-o")) {
            if (i == argc - 1) {
                fprintf(stderr, "ERROR! Output flag specified without specifying the output file!\n");
                return false;
            }
            opts->out_file = argv[i + 1];
        } else {
            fprintf(stderr, "ERROR! '%s' is an unrecognized option!\n", argv[i]);
            show_help();
            return false;
        }
    }
    return true;
}

int main(int argc, char **argv)
{
    if (!parse_args(argc, argv, &opts)) {
        return EXIT_FAILURE;
    }

    const Op *op_table = init_op_table();
    if (opts.dump_ops) {
       print_op_table(op_table);
       return EXIT_SUCCESS;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        fprintf(stderr, "ERROR! Could not open file '%s'. Ensure read permissions are set.\n", argv[1]);
        goto error;
    }

    const char *buf = NULL;
    size_t buf_size = read_file_into_mem(file, &buf);
    if (buf_size == 0) {
        fprintf(stderr, "ERROR! Unable to read file '%s' into memory. \n", argv[1]);
        goto error;
    }
    fclose(file); /* The file is no longer necessary after being in memory. */
    
    if (!generate_bin(buf, buf_size, op_table)) {
        fprintf(stderr, "ERROR! Unable to generate final binary '%s'.\n", opts.out_file);
        goto error;
    }

    free((void*)buf);
    return EXIT_SUCCESS;

error:
    if (file) {
        fclose(file);
    }
    if (buf) {
        free((void*)buf);
    }
    return EXIT_FAILURE;
}
