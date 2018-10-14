#pragma once

#include <iostream>

#define VLOG(x, y) do {\
        if ((x).verbose) {\
            std::cout << y << '\n';\
        }\
    } while(0)

#define V_UNUSED(x) ((void)(x))

struct AsmOpts {
    const char *in_file, *out_file;
    bool dump_asm, show_help, verbose;

    AsmOpts(const char *in = "", const char *out = "a.rom", bool dump_asm = false,
        bool show_help = false, bool verbose = false) :
        in_file(in), out_file(out), dump_asm(dump_asm), show_help(show_help), verbose(verbose) {}
};

struct Instruction {
    std::string label, op;
    std::vector<std::string> args;
    uint16_t addr;

    Instruction(const std::string& label, const std::string& op,
        const std::vector<std::string>& args, uint16_t addr = 0x0200) :
            label(label), op(op), args(args), addr(addr) {}
};
