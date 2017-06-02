#pragma once

#include <stdio.h>
#include <stdbool.h>

typedef struct {
   const char *buf; /* The memory buffer containing the string to parse */
   size_t buf_size; /* The size of the above memory buffer */
   size_t cursor;   /* Where in the memory buffer we are */
   const char *last_token; /* The last token we processed */
   size_t last_token_size; /* The size of the last token */ 
} AsmParser;

/*
 * Initializes the parser to be ready to parse the 
 * given string
 */
bool init_parser(AsmParser *parser, const char *str);

/*
 * Frees up the resources obtained by the parser. Once this
 * function is called, the parser passed in cannot be used 
 * until another call to init_parser() is made
 */
bool free_parser(AsmParser *parser);

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
void get_next_token(AsmParser *parser, const char **token, size_t *size);
