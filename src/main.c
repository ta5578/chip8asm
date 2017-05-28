#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <ctype.h>

void print_token(const char *token, size_t size)
{
    for (size_t i = 0; i < size; ++i) {
        putchar(token[i]);
    }
}

bool streq(const char *s1, size_t size1, const char *s2, size_t size2)
{
    if (size1 != size2) {
        return false;
    }
    for (size_t i = 0; i < size1; ++i) {
        if (s1[i] != s2[i]) {
            return false;
        }
    }
    return true;
}


bool is_operator(const char *token, size_t size)
{
    return streq(token, size, "LOAD", 4) ||
        streq(token, size, "LOADI", 5) ||
        streq(token, size, "DRAW", 4) ||
        streq(token, size, "JUMP", 4) ||
        streq(token, size, "LB", 2);
}

bool is_label(const char *token, size_t size)
{
    return token[size - 1] == ':';
}

bool is_operand(const char *token, size_t size, const char *op, size_t op_size)
{
    return true; 
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

/*
 * Skips ahead into the memory buffer until the next space-delimited token is
 * found. Comment tokens are automatically ignored and the next valid token is
 * returned.
 *
 * Note that no memory allocation is done for 'new' token strings. Instead, a token 
 * is defined as as just a window into the original buffer. The length of 
 * the token is also returned. This way, we avoid repeated memory allocations for strings.
 *
 * None of the arguments are expected to be null.
 */
void get_next_token(const char *buf, size_t buf_size, const char **token, size_t *size)
{
    assert(buf);
    assert(token);
    assert(size);

    static size_t cursor = 0;

    while (cursor < buf_size && isspace(buf[cursor])) {
        ++cursor;
    }

    size_t token_size = 0;
    while (cursor < buf_size && !isspace(buf[cursor])) {
        ++cursor;
        ++token_size;
    }

    *token = buf + cursor - token_size;
    *size = token_size;
    /* We don't want comment tokens */
    if (**token == ';') {
        while (cursor < buf_size && buf[cursor] != '\n') {
            ++cursor;
        }
        get_next_token(buf, buf_size, token, size);
    }
}

bool process_token(const char *token, size_t size)
{
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
        if (!is_operator(token, size)) {
            if (!is_label(token, size)) {
                error_msg(token, size, " is not a valid operator or label!");   
                return false;
            } else {
                stdout_msg(token, size, " recognized as a label!");
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

bool generate_bin(const char *buf, size_t buf_size)
{
    assert(buf);

    const char *token = NULL;
    size_t token_size = 0;
    bool success = 0;
    do {
        get_next_token(buf, buf_size, &token, &token_size);
        /* There's a bit of a kink here in that the very laste
         * token will be empty and cause the below to report failure
         * if we don't explicitly check for an empty token
         */
        success = process_token(token, token_size);
    } while (success && token_size > 0);
    return success;
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr, "The assembly file is required as an argument.\n");
        return EXIT_FAILURE;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        fprintf(stderr, "Could not open file '%s'. Ensure read permissions are set.\n", argv[1]);
        goto error;
    }

    const char *buf = NULL;
    size_t buf_size = read_file_into_mem(file, &buf);
    if (buf_size == 0) {
        fprintf(stderr, "Unable to read file '%s' into memory. \n", argv[1]);
        goto error;
    }
    fclose(file); /* The file is no longer necessary after being in memory. */
    
    if (!generate_bin(buf, buf_size)) {
        fprintf(stderr, "Error.\n");
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
