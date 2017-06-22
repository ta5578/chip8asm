#pragma once

#include "vm_def.h"
#include "asmparser.h"
#include <set>
#include <string>

struct Label {
    std::string name;
    uint16_t addr;
    Label(std::string name, uint16_t addr) : name(name), addr(addr) {}
};

class BinGenerator {

private:
    AsmParser parser;
    std::set<Label> labels;
    uint16_t current_addr;
    std::string prev_tok;

    enum class State {
        START = 0,
        OPERATOR
    } state;

public:
    BinGenerator(const AsmParser& parser);
    std::string generate_bin();
};
