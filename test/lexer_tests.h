#pragma once

#include "asmlexer.h"
#include "FileReader.h"

TEST(TestLexerExCall)
{
    FileReader reader("../examples/call.asm");
    AsmLexer lex(reader);
    auto tok = lex.get_next_token();
    CHECK_EQUAL("start", tok.second);

    tok = lex.get_next_token();
    CHECK_EQUAL("CALL", tok.second);

    tok = lex.get_next_token();
    CHECK_EQUAL("start", tok.second);

    tok = lex.get_next_token();
    CHECK_EQUAL("CALL", tok.second);

    tok = lex.get_next_token();
    CHECK_EQUAL("$EEE", tok.second);
}