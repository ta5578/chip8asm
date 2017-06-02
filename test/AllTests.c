

/* This is auto-generated code. Edit at your own peril. */
#include <stdio.h>
#include <stdlib.h>

#include "CuTest.h"


extern void TestInitParser(CuTest*);
extern void TestFreeParser(CuTest*);
extern void TestGetTokenStartWithLabel(CuTest*);
extern void TestGetTokenStartWithOperator(CuTest*);
extern void TestGetTokenEmptyBuf(CuTest*);
extern void TestGetTokenSkipCommentsSemiColonAttached(CuTest*);
extern void TestGetTokenSkipCommentsSemiColonSpaced(CuTest*);


void RunAllTests(void) 
{
    CuString *output = CuStringNew();
    CuSuite* suite = CuSuiteNew();


    SUITE_ADD_TEST(suite, TestInitParser);
    SUITE_ADD_TEST(suite, TestFreeParser);
    SUITE_ADD_TEST(suite, TestGetTokenStartWithLabel);
    SUITE_ADD_TEST(suite, TestGetTokenStartWithOperator);
    SUITE_ADD_TEST(suite, TestGetTokenEmptyBuf);
    SUITE_ADD_TEST(suite, TestGetTokenSkipCommentsSemiColonAttached);
    SUITE_ADD_TEST(suite, TestGetTokenSkipCommentsSemiColonSpaced);

    CuSuiteRun(suite);
    CuSuiteSummary(suite, output);
    CuSuiteDetails(suite, output);
    printf("%s\n", output->buffer);
    CuStringDelete(output);
    CuSuiteDelete(suite);
}

int main(void)
{
    RunAllTests();
}

