#pragma once

#include <cstdint>
#include <cstdio>
#include <string>
#include <vector>
#include <map>
#include "vm_def.h"

class BinGenerator {
public:
    BinGenerator(const std::vector<Instruction>& instructions, const std::map<std::string, uint16_t>& labels, const AsmOpts& opts);
    ~BinGenerator();
    void generate_bin();
private:
    AsmOpts opts;
    std::FILE *fp;

    std::vector<Instruction> _instructions;
    std::map<std::string, uint16_t> _labels;

    uint16_t get_opcode(const std::string& op, const std::vector<std::string>& args) const;
    void dump_asm() const;
};
