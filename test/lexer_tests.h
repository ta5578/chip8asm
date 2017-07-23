#pragma once

#include "asmlexer.h"
#include "utils.h"
#include <cstdlib>

TEST(TestLexerEx)
{
    const char *buf = read_file("../examples/ex.asm");
    AsmLexer lex(buf);
    std::free(const_cast<char*>(buf));

    auto tok = lex.get_next_token();
    CHECK_EQUAL("start", tok.second);

    tok = lex.get_next_token();
    CHECK_EQUAL("ILOAD", tok.second);

    tok = lex.get_next_token();
    CHECK_EQUAL("sprite", tok.second);

    tok = lex.get_next_token();
    CHECK_EQUAL("LOAD", tok.second);

    tok = lex.get_next_token();
    CHECK_EQUAL("r0", tok.second);

    tok = lex.get_next_token();
    CHECK_EQUAL(",", tok.second);

    tok = lex.get_next_token();
    CHECK_EQUAL("$A", tok.second);

    tok = lex.get_next_token();
    CHECK_EQUAL("LOAD", tok.second);

    tok = lex.get_next_token();
    CHECK_EQUAL("r1", tok.second);

    tok = lex.get_next_token();
    CHECK_EQUAL(",", tok.second);

    tok = lex.get_next_token();
    CHECK_EQUAL("$5", tok.second);

    tok = lex.get_next_token();
    CHECK_EQUAL("DRAW", tok.second);

    tok = lex.get_next_token();
    CHECK_EQUAL("r0", tok.second);

    tok = lex.get_next_token();
    CHECK_EQUAL(",", tok.second);

    tok = lex.get_next_token();
    CHECK_EQUAL("r1", tok.second);

    tok = lex.get_next_token();
    CHECK_EQUAL(",", tok.second);

    tok = lex.get_next_token();
    CHECK_EQUAL("$5", tok.second);

    tok = lex.get_next_token();
    CHECK_EQUAL("end", tok.second);

    tok = lex.get_next_token();
    CHECK_EQUAL("JMP", tok.second);

    tok = lex.get_next_token();
    CHECK_EQUAL("end", tok.second);

    tok = lex.get_next_token();
    CHECK_EQUAL("sprite", tok.second);

    tok = lex.get_next_token();
    CHECK_EQUAL("LB", tok.second);

    tok = lex.get_next_token();
    CHECK_EQUAL("$F0", tok.second);

    tok = lex.get_next_token();
    CHECK_EQUAL("LB", tok.second);

    tok = lex.get_next_token();
    CHECK_EQUAL("$90", tok.second);

    tok = lex.get_next_token();
    CHECK_EQUAL("LB", tok.second);

    tok = lex.get_next_token();
    CHECK_EQUAL("$F0", tok.second);

    tok = lex.get_next_token();
    CHECK_EQUAL("LB", tok.second);

    tok = lex.get_next_token();
    CHECK_EQUAL("$90", tok.second);

    tok = lex.get_next_token();
    CHECK_EQUAL("LB", tok.second);

    tok = lex.get_next_token();
    CHECK_EQUAL("$90", tok.second);
}