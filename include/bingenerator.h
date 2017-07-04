#pragma once

#include "asmlexer.h"
#include "vm_def.h"
#include <map>
#include <cstdint>
#include <vector>
#include <string>
#include "utils.h"
#include "FileWriter.h"

class BinGenerator {
    AsmLexer lexer;
    AsmOpts opts;
    FileWriter fwriter;

    std::map<std::string, uint16_t> addrs;
    std::map<std::string, std::vector<uint16_t>> parse_tree;
    std::string curr_label;
    uint16_t curr_addr;

    bool label_exists(const std::string& lbl) const;
    uint16_t get_opcode(const std::string& op, const std::vector<std::string>& args) const;
    void process_label(const std::string& label);
    void process_operator(const std::string& op);

public:
    BinGenerator(const AsmLexer& lexer, const AsmOpts& opts);
    void parse();
    std::string generate_bin();
};
