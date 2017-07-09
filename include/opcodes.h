#pragma once

#include <vector>
#include <string>
#include <functional>
#include "vm_def.h"
#include "utils.h"
#include <map>

/*
 * Each opcode will have a function returning a 16 bit value
 * given a vector of arguments
 */
typedef std::function<uint16_t(const std::vector<std::string>&)> OpFxn;

/* Unsupported */
inline uint16_t fxnSYS(const std::vector<std::string>& args)
{
    V_UNUSED(args);
    return 0x0;
}

inline uint16_t fxnCLR(const std::vector<std::string>& args)
{
    V_UNUSED(args);
    return 0x00E0;
}

inline uint16_t fxnRET(const std::vector<std::string>& args)
{
    V_UNUSED(args);
    return 0x00EE;
}

inline uint16_t fxnJMP(const std::vector<std::string>& args)
{
    return 0x1000 | to_hex(args[0]);
}

inline uint16_t fxnCALL(const std::vector<std::string>& args)
{
    return 0x2000 | to_hex(args[0]);
}

inline uint16_t fxnSKE(const std::vector<std::string>& args)
{
    return 0x3000 | to_hex(args[0]) << 8 | to_hex(args[1]);
}

inline uint16_t fxnSKNE(const std::vector<std::string>& args)
{
    return 0x4000 | to_hex(args[0]) << 8 | to_hex(args[1]);
}

inline uint16_t fxnSKRE(const std::vector<std::string>& args)
{
    return 0x5000 | to_hex(args[0]) << 8 | to_hex(args[1]) << 4;
}

inline uint16_t fxnLOAD(const std::vector<std::string>& args)
{
    return 0x6000 | to_hex(args[0]) << 8 | to_hex(args[1]);
}

inline uint16_t fxnADD(const std::vector<std::string>& args)
{
    return 0x7000 | to_hex(args[0]) << 8 | to_hex(args[1]);
}

inline uint16_t fxnASN(const std::vector<std::string>& args)
{
    return 0x8000 | to_hex(args[0]) << 8 | to_hex(args[1]) << 4;
}

inline uint16_t fxnOR(const std::vector<std::string>& args)
{
    return 0x8000 | to_hex(args[0]) << 8 | to_hex(args[1]) << 4 | 0x1;
}

inline uint16_t fxnAND(const std::vector<std::string>& args)
{
    return 0x8000 | to_hex(args[0]) << 8 | to_hex(args[1]) << 4 | 0x2;
}

inline uint16_t fxnXOR(const std::vector<std::string>& args)
{
    return 0x8000 | to_hex(args[0]) << 8 | to_hex(args[1]) << 4 | 0x3;
}

inline uint16_t fxnRADD(const std::vector<std::string>& args)
{
    return 0x8000 | to_hex(args[0]) << 8 | to_hex(args[1]) << 4 | 0x4;
}

inline uint16_t fxnSUB(const std::vector<std::string>& args)
{
    return 0x8000 | to_hex(args[0]) << 8 | to_hex(args[1]) << 4 | 0x5;
}

inline uint16_t fxnSHR(const std::vector<std::string>& args)
{
    return 0x8000 | to_hex(args[0]) << 8 | 0x6;
}

inline uint16_t fxnRSUB(const std::vector<std::string>& args)
{
    return 0x8000 | to_hex(args[0]) << 8 | to_hex(args[1]) << 4 | 0x7;
}

inline uint16_t fxnSHL(const std::vector<std::string>& args)
{
    return 0x8000 | to_hex(args[0]) << 8 | 0xE;
}

inline uint16_t fxnSKRNE(const std::vector<std::string>& args)
{
    return 0x9000 | to_hex(args[0]) << 8 | to_hex(args[1]) << 4;
}

inline uint16_t fxnILOAD(const std::vector<std::string>& args)
{
    return 0xA000 | to_hex(args[0]);
}

inline uint16_t fxnZJMP(const std::vector<std::string>& args)
{
    return 0xB000 | to_hex(args[0]);
}

inline uint16_t fxnRAND(const std::vector<std::string>& args)
{
    return 0xC000 | to_hex(args[0]) << 8 | to_hex(args[0]);
}

inline uint16_t fxnDRAW(const std::vector<std::string>& args)
{
    return 0xD000 | to_hex(args[0]) << 8 | to_hex(args[1]) << 4 | to_hex(args[2]);
}

inline uint16_t fxnSKK(const std::vector<std::string>& args)
{
    return 0xE000 | to_hex(args[0]) << 8 | 0x009E;
}

inline uint16_t fxnSKNK(const std::vector<std::string>& args)
{
    return 0xE000 | to_hex(args[0]) << 8 | 0x00A1;
}

inline uint16_t fxnDELA(const std::vector<std::string>& args)
{
    return 0xF000 | to_hex(args[0]) << 8 | 0x0007;
}

inline uint16_t fxnKEYW(const std::vector<std::string>& args)
{
    return 0xF000 | to_hex(args[0]) << 8 | 0x000A;
}

inline uint16_t fxnDELR(const std::vector<std::string>& args)
{
    return 0xF000 | to_hex(args[0]) << 8 | 0x0015;
}

inline uint16_t fxnSNDR(const std::vector<std::string>& args)
{
    return 0xF000 | to_hex(args[0]) << 8 | 0x0018;
}

inline uint16_t fxnIADD(const std::vector<std::string>& args)
{
    return 0xF000 | to_hex(args[0]) << 8 | 0x001E;
}

inline uint16_t fxnSILS(const std::vector<std::string>& args)
{
    return 0xF000 | to_hex(args[0]) << 8 | 0x0029;
}

inline uint16_t fxnBCD(const std::vector<std::string>& args)
{
    return 0xF000 | to_hex(args[0]) << 8 | 0x0033;
}

inline uint16_t fxnDUMP(const std::vector<std::string>& args)
{
    return 0xF000 | to_hex(args[0]) << 8 | 0x0055;
}

inline uint16_t fxnIDUMP(const std::vector<std::string>& args)
{
    return 0xF000 | to_hex(args[0]) << 8 | 0x0065;
}

inline uint16_t fxnLB(const std::vector<std::string>& args)
{
    return to_hex(args[0]);
}

static const std::map<std::string, OpFxn> OPERATORS = {
    {"SYS", fxnSYS },
    {"CLR", fxnCLR },
    {"RET", fxnRET },
    {"JMP", fxnJMP },
    {"CALL", fxnCALL },
    {"SKE", fxnSKE },
    {"SKNE", fxnSKNE },
    {"SKRE", fxnSKRE },
    {"LOAD", fxnLOAD },
    {"ADD", fxnADD },
    {"ASN", fxnASN },
    {"OR", fxnOR },
    {"AND", fxnAND },
    {"XOR", fxnXOR },
    {"RADD", fxnRADD },
    {"SUB", fxnSUB },
    {"SHR", fxnSHR },
    {"RSUB", fxnRSUB },
    {"SHL", fxnSHL },
    {"SKRNE", fxnSKRNE },
    {"ILOAD", fxnILOAD },
    {"ZJMP", fxnZJMP },
    {"RAND", fxnRAND },
    {"DRAW", fxnDRAW },
    {"SKK", fxnSKK },
    {"SKNK", fxnSKNK },
    {"DELA", fxnDELA },
    {"KEYW", fxnKEYW },
    {"DELR", fxnDELR },
    {"SNDR", fxnSNDR },
    {"IADD", fxnIADD },
    {"SILS", fxnSILS },
    {"BCD", fxnBCD },
    {"DUMP", fxnDUMP },
    {"IDUMP", fxnIDUMP },
    {"LB", fxnLB}
};

inline bool is_operator(const std::string& s)
{
    return OPERATORS.find(s) != OPERATORS.end();
}