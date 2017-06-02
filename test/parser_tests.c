#include "CuTest.h"
#include "asmparser.h"

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
