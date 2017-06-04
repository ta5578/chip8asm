#pragma once

#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
#include "vm_def.h"

static void print_token(const char *token, size_t size)
{
    for (size_t i = 0; i < size; ++i) {
        putchar(token[i]);
    }
}

static void error_msg(const char *token, size_t size, const char *msg)
{
    for (size_t i = 0; i < size; ++i) {
        fprintf(stderr, "%c", token[i]);
    }
    fprintf(stderr, " %s\n", msg);
}

static void stdout_msg(const char *token, size_t size, const char *msg)
{
    print_token(token, size);
    printf("%s\n", msg);
}

/*
 * Perform a case insensitive comparison of two strings
 */
static bool streq(const char *s1, size_t size1, const char *s2, size_t size2)
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
static bool alt_strcmp(const char *base, const char *s1, const char *s2)
{
    return strcmp(base, s1) == 0 || strcmp(base, s2) == 0;
}


/*
 * Allocates a __null-terminated__ string buffer on the heap
 * capable of holding 'size' characters. If allocation fails,
 * the function returns NULL.
 */
static char* smalloc(size_t size)
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
static size_t fsize(FILE *file)
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
static size_t read_file_into_mem(FILE *file, const char **buf)
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

static void show_help()
{
    puts("chip8asm is an assembler for the chip 8 VM.");
    puts("The only required argument is the input .asm file.");
    puts("The first argument should be one of the input file or help.");
    puts("Here are the supported options:");
    puts("   --dump-asm | -dasm -- dumps the assembled statements with memory locations");
    puts("   --output | -o -- the name of the output ROM file. By default, it is 'a.rom'");
    puts("   --help | -h -- displays this help screen");
}

/* 
 * Parse the command line arguments and populate the opt
 * structure. Returns true if successful.
 */
static bool parse_args(int argc, char **argv, AsmOpts *opts)
{
    assert(opts);

    if (argc < 2) {
        return false;
    }

    /* The first option should only be one of these */
    if (alt_strcmp(argv[1], "--help", "-h")) {
       opts->show_help = true;
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
            return false;
        }
    }
    return true;
}
