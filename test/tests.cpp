#define CATCH_CONFIG_MAIN
#include "test/catch.hpp"
#include "Lexer.h"

TEST_CASE("LexerIntegrationTest")
{
    const std::string text = R"(
; This is a comment in the assembly file
; This will print the capital letter 'A' to the screen

start
    ILOAD sprite ; load the sprite location into index
    LOAD r0,$A ; load 10 into register 0
    LOAD r1,$5 ; load 5 into register 1
    DRAW r0,r1,$5 ; draw a 5 byte sprite at (x,y) specified in r0, r1

end
    JMP end ; loop indefinitely

; The capital letter 'A'
sprite
    LB $F0
    LB $90
    LB $F0
    LB $90
    LB $90
)";
    c8::Lexer lex(text);

    auto tok = lex.get_next_token();
    REQUIRE("start" == tok._str);

    tok = lex.get_next_token();
    REQUIRE("ILOAD" == tok._str);

    tok = lex.get_next_token();
    REQUIRE("sprite" == tok._str);

    tok = lex.get_next_token();
    REQUIRE("LOAD" == tok._str);

    tok = lex.get_next_token();
    REQUIRE("r0" == tok._str);

    tok = lex.get_next_token();
    REQUIRE("," == tok._str);

    tok = lex.get_next_token();
    REQUIRE("$A" == tok._str);

    tok = lex.get_next_token();
    REQUIRE("LOAD" == tok._str);

    tok = lex.get_next_token();
    REQUIRE("r1" == tok._str);

    tok = lex.get_next_token();
    REQUIRE("," == tok._str);

    tok = lex.get_next_token();
    REQUIRE("$5" == tok._str);

    tok = lex.get_next_token();
    REQUIRE("DRAW" == tok._str);

    tok = lex.get_next_token();
    REQUIRE("r0" == tok._str);

    tok = lex.get_next_token();
    REQUIRE("," == tok._str);

    tok = lex.get_next_token();
    REQUIRE("r1" == tok._str);

    tok = lex.get_next_token();
    REQUIRE("," == tok._str);

    tok = lex.get_next_token();
    REQUIRE("$5" == tok._str);

    tok = lex.get_next_token();
    REQUIRE("end" == tok._str);

    tok = lex.get_next_token();
    REQUIRE("JMP" == tok._str);

    tok = lex.get_next_token();
    REQUIRE("end" == tok._str);

    tok = lex.get_next_token();
    REQUIRE("sprite" == tok._str);

    tok = lex.get_next_token();
    REQUIRE("LB" == tok._str);

    tok = lex.get_next_token();
    REQUIRE("$F0" == tok._str);

    tok = lex.get_next_token();
    REQUIRE("LB" == tok._str);

    tok = lex.get_next_token();
    REQUIRE("$90" == tok._str);

    tok = lex.get_next_token();
    REQUIRE("LB" == tok._str);

    tok = lex.get_next_token();
    REQUIRE("$F0" == tok._str);

    tok = lex.get_next_token();
    REQUIRE("LB" == tok._str);

    tok = lex.get_next_token();
    REQUIRE("$90" == tok._str);

    tok = lex.get_next_token();
    REQUIRE("LB" == tok._str);

    tok = lex.get_next_token();
    REQUIRE("$90" == tok._str);
}