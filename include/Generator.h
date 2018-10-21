#pragma once

#include "Parser.h"

namespace c8 {
    struct Instruction {
        Statement stmt;
        uint16_t op;

        Instruction(Statement stmt, uint16_t op);

        std::string toString() const;
    };

    std::vector<Instruction> generateInstructions(const std::vector<Statement>& statements);

}