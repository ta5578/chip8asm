#pragma once

#include "asmlexer.h"
#include <unordered_map>
#include <cstdint>
#include <vector>
#include <string>
#include "utils.h"

class AsmParser {
    AsmLexer lexer;
    std::unordered_map<std::string, std::vector<uint16_t>> parse_tree;
    bool label_exists(const std::string& lbl) const;
    uint16_t get_opcode(const std::string& op, const std::vector<std::string>& args) const;

public:
    AsmParser(const AsmLexer& lexer);
    void parse();
};