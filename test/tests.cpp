#define CATCH_CONFIG_MAIN
#include "test/catch.hpp"
#include "Lexer.h"
#include "opcodes.h"

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

TEST_CASE("TestSYS")
{
    auto val = fxnSYS({});
    REQUIRE(0x0 == val);
}

TEST_CASE("TestCLR")
{
    auto val = fxnCLR({});
    REQUIRE(0x00E0 == val);
}

TEST_CASE("TestRET")
{
    auto val = fxnRET({});
    REQUIRE(0x00EE == val);
}

TEST_CASE("TestJMP")
{
    auto val = fxnJMP({ "ABC" });
    REQUIRE(0x1ABC == val);
}

TEST_CASE("TestCALL")
{
    auto val = fxnCALL({ "ABC" });
    REQUIRE(0x2ABC == val);
}

TEST_CASE("TestSKE")
{
    auto val = fxnSKE({ "RF", "AB" });
    REQUIRE(0x3FAB == val);
}

TEST_CASE("TestSKNE")
{
    auto val = fxnSKNE({ "RF", "AB" });
    REQUIRE(0x4FAB == val);
}

TEST_CASE("TestSKRE")
{
    auto val = fxnSKRE({ "RA", "RB" });
    REQUIRE(0x5AB0 == val);
}

TEST_CASE("TestLOAD")
{
    auto val = fxnLOAD({ "R1", "$AB" });
    REQUIRE(0x61AB == val);
}

TEST_CASE("TestADD")
{
    auto val = fxnADD({ "R1", "$AB" });
    REQUIRE(0x71AB == val);
}

TEST_CASE("TestASN")
{
    auto val = fxnASN({ "RA", "RB" });
    REQUIRE(0x8AB0 == val);
}

TEST_CASE("TestOR")
{
    auto val = fxnOR({ "RA", "RB" });
    REQUIRE(0x8AB1 == val);
}

TEST_CASE("TestAND")
{
    auto val = fxnAND({ "RA", "RB" });
    REQUIRE(0x8AB2 == val);
}

TEST_CASE("TestXOR")
{
    auto val = fxnXOR({ "RA", "RB" });
    REQUIRE(0x8AB3 == val);
}

TEST_CASE("TestRADD")
{
    auto val = fxnRADD({ "RA", "RB" });
    REQUIRE(0x8AB4 == val);
}

TEST_CASE("TestSUB")
{
    auto val = fxnSUB({ "RA", "RB" });
    REQUIRE(0x8AB5 == val);
}

TEST_CASE("TestSHR")
{
    auto val = fxnSHR({ "R2" });
    REQUIRE(0x8206 == val);
}

TEST_CASE("TestRSUB")
{
    auto val = fxnRSUB({ "RA", "RB" });
    REQUIRE(0x8AB7 == val);
}

TEST_CASE("TestSHL")
{
    auto val = fxnSHL({ "R2" });
    REQUIRE(0x820E == val);
}

TEST_CASE("TestSKRNE")
{
    auto val = fxnSKRNE({ "RA", "RB" });
    REQUIRE(0x9AB0 == val);
}

TEST_CASE("TestILOAD")
{
    auto val = fxnILOAD({ "$ABC" });
    REQUIRE(0xAABC == val);
}

TEST_CASE("TestZJMP")
{
    auto val = fxnZJMP({ "$ABC" });
    REQUIRE(0xBABC == val);
}

TEST_CASE("TestRAND")
{
    auto val = fxnRAND({ "R8" });
    REQUIRE(0xC808 == val);
}

TEST_CASE("TestDRAW")
{
    auto val = fxnDRAW({ "r0", "r1", "$6" });
    REQUIRE(0xD016 == val);
}

TEST_CASE("TestSKK")
{
    auto val = fxnSKK({ "9" });
    REQUIRE(0xE99E == val);
}

TEST_CASE("TestSKNK")
{
    auto val = fxnSKNK({ "9" });
    REQUIRE(0xE9A1 == val);
}

TEST_CASE("TestDELA")
{
    auto val = fxnDELA({ "r4" });
    REQUIRE(0xF407 == val);
}

TEST_CASE("TestKEYW")
{
    auto val = fxnKEYW({ "rF" });
    REQUIRE(0xFF0A == val);
}

TEST_CASE("TestDELR")
{
    auto val = fxnDELR({ "rF" });
    REQUIRE(0xFF15 == val);
}

TEST_CASE("TestSNDR")
{
    auto val = fxnSNDR({ "rF" });
    REQUIRE(0xFF18 == val);
}

TEST_CASE("TestIADD")
{
    auto val = fxnIADD({ "rF" });
    REQUIRE(0xFF1E == val);
}

TEST_CASE("TestSILS")
{
    auto val = fxnSILS({ "rF" });
    REQUIRE(0xFF29 == val);
}

TEST_CASE("TestBCD")
{
    auto val = fxnBCD({ "rF" });
    REQUIRE(0xFF33 == val);
}

TEST_CASE("TestDUMP")
{
    auto val = fxnDUMP({ "rF" });
    REQUIRE(0xFF55 == val);
}

TEST_CASE("TestIDUMP")
{
    auto val = fxnIDUMP({ "rF" });
    REQUIRE(0xFF65 == val);
}

TEST_CASE("TestLB")
{
    auto val = fxnLB({ "$ABC" });
    REQUIRE(0xABC == val);
}