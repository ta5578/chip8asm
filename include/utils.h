#pragma once

#include <string>
#include <set>

static const std::set<std::string> OPERATORS = {
    "SYS",
    "CLR",
    "RET",
    "JMP",
    "CALL",
    "SKE",
    "SKNE",
    "SKRE",
    "LOAD",
    "ADD",
    "ASN",
    "OR",
    "AND",
    "XOR",
    "ADDR",
    "SUB",
    "SHR",
    "SUBR",
    "SHL",
    "SKRNE",
    "LOADI",
    "JMP0",
    "RAND",
    "DRAW",
    "SKK",
    "SKNK",
    "DELA",
    "KEYW",
    "DELR",
    "SNDR",
    "ADDI",
    "SILS",
    "BCD",
    "DUMP",
    "DUMPI"
};

static bool is_operator(const std::string& s)
{
    return OPERATORS.find(s) != OPERATORS.end();
}
