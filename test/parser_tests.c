#include "CuTest.h"
#include <assert.h>
#include "string.h"
#include "asmparser.h"
#include <stdbool.h>
#include <stdlib.h>

/*
 * Creates a duplicate of the string pointed to by token on the heap
 * and null terminates it. This is because tokens are normally represented
 * by a pointer into the file buffer with a size offset. We want to test for
 * string equality though so we need to make sure to have a null terminated
 * string. It's easiest to do this by just allocating a buffer on the heap.
 */
static char* get_token_str(const char *expected, const char *str, size_t size)
{
    size_t len = strlen(expected);
    char *d = malloc(len + 1);
    assert(d);
    d[len] = '\0';
    strncpy(d, str, len);
}

/*
 * A convenience method that sets up test cases for testing parser tokens.
 * We would have to do a similar setup for each parser test below.
 */
static void run_token_test(CuTest *tc, const char *buf, const char *expected)
{
    AsmParser parser;
    init_parser(&parser, buf);

    const char *token = NULL;
    size_t size = 0;
    get_next_token(&parser, &token, &size);

    char *d = get_token_str(expected, token, size);
    CuAssertTrue(tc, size == strlen(expected));
    CuAssertStrEquals(tc, expected, d);
    free(d);
}

void TestInitParser(CuTest *tc)
{
    AsmParser parser;
    const char *buf = "TEST";
    init_parser(&parser, buf);

    CuAssertStrEquals(tc, "TEST", parser.buf);
    CuAssertTrue(tc, parser.buf_size == 4);
    CuAssertTrue(tc, parser.cursor == 0);
    CuAssertTrue(tc, parser.last_token == NULL);
    CuAssertTrue(tc, parser.last_token_size == 0);
} 

void TestFreeParser(CuTest *tc)
{
    AsmParser parser;
    free_parser(&parser);

    CuAssertTrue(tc, parser.buf == NULL); 
    CuAssertTrue(tc, parser.buf_size == 0); 
    CuAssertTrue(tc, parser.cursor == 0); 
    CuAssertTrue(tc, parser.last_token == NULL); 
    CuAssertTrue(tc, parser.last_token_size == 0); 
}

void TestGetTokenStartWithLabel(CuTest *tc)
{
    const char *buf = "\nstart:\n    ";
    const char *expected = "start:";
    run_token_test(tc, buf, expected);
}

void TestGetTokenStartWithOperator(CuTest *tc)
{
    const char *buf = "\nLOAD";
    const char *expected = "LOAD";
    run_token_test(tc, buf, expected);
}

void TestGetTokenEmptyBuf(CuTest *tc)
{
    const char *buf = "";
    const char *expected = "";
    run_token_test(tc, buf, expected);
}

void TestGetTokenSkipCommentsSemiColonAttached(CuTest *tc)
{
    const char *buf = ";Look at me, I'm a comment!\nstart:";
    const char *expected = "start:";
    run_token_test(tc, buf, expected);
}

void TestGetTokenSkipCommentsSemiColonSpaced(CuTest *tc)
{
    /* Note the fact that there's a space between ';' and the next word. */
    const char *buf = "; I'm another comment with space.\nLOAD ";
    const char *expected = "LOAD";
    run_token_test(tc, buf, expected);
}
