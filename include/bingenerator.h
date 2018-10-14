#pragma once

#include "Lexer.h"
#include <cstdint>
#include <cstdio>
#include <vector>
#include <string>
#include "vm_def.h"
#include <map>

class BinGenerator {
    emu::Lexer lexer;
    AsmOpts opts;
    std::FILE *fp;

    std::map<std::string, uint16_t> label_addrs;
    std::vector<Instruction> instructions;
    std::string curr_label;
    uint16_t curr_addr;

    bool label_exists(const std::string& lbl) const;
    uint16_t get_opcode(const std::string& op, const std::vector<std::string>& args) const;
    void process_label(const std::string& label);
    void process_operator(const std::string& op);
    void dump_asm() const;
public:
    BinGenerator(const emu::Lexer& lexer, const AsmOpts& opts);
    ~BinGenerator();
    void parse();
    void generate_bin();
};
