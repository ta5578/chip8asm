#pragma once

#ifndef NDEBUG
    #include <cstdio>
    #define LOG_IMPL(func, line, m, ...) \
                do {\
                    std::fprintf(stderr, "[%s:%d] "##m##"\n", reinterpret_cast<const char*>(func), static_cast<int>(line), ##__VA_ARGS__);\
                } while (0)
    #define LOG(m, ...) LOG_IMPL(__FUNCTION__, __LINE__, m, ##__VA_ARGS__)
#else
    #define LOG(m, ...)
#endif

#define V_UNUSED(x) ((void)(x))

struct AsmOpts {
    const char *in_file, *out_file;
    bool dump_asm, show_help;

    AsmOpts(const char *in = "", const char *out = "a.rom", bool dump_asm = false,
        bool show_help = false) :
        in_file(in), out_file(out), dump_asm(dump_asm), show_help(show_help) {}
};