#pragma once

struct AsmOpts {
    const char *in_file, *out_file;
    bool dump_asm, show_help;

    AsmOpts(const char *in = "", const char *out = "a.rom", bool dump_asm = false, bool show_help = false) :
        in_file(in), out_file(out), dump_asm(dump_asm), show_help(show_help) {}
};
