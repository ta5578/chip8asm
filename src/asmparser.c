#include "asmparser.h"
#include <ctype.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

bool init_parser(AsmParser *parser, const char *str)
{
    assert(parser);
    assert(str);

    parser->buf = str;
    parser->buf_size = strlen(str);
    parser->cursor = 0;
    parser->last_token = NULL;
    parser->last_token_size = 0;
}

bool free_parser(AsmParser *parser)
{
    assert(parser);

    parser->buf = NULL;
    parser->buf_size = 0;
    parser->cursor = 0;
    parser->last_token = NULL;
    parser->last_token_size = 0;
}

void get_next_token(AsmParser *parser, const char **token, size_t *size)
{
    assert(parser);
    assert(token);
    assert(size);

    while (parser->cursor < parser->buf_size && isspace(parser->buf[parser->cursor])) {
        ++parser->cursor;
    }

    size_t token_size = 0;
    while (parser->cursor < parser->buf_size && !isspace(parser->buf[parser->cursor])) {
        ++parser->cursor;
        ++token_size;
    }

    *token = parser->buf + parser->cursor - token_size;
    *size = token_size;
    /* We don't want comment tokens */
    if (**token == ';') {
        while (parser->cursor < parser->buf_size && parser->buf[parser->cursor] != '\n') {
            ++parser->cursor;
        }
        get_next_token(parser, token, size);
    }
}
