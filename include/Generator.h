#pragma once

#include "Parser.h"

namespace c8 {
    struct Instruction {
        Statement stmt;
        uint16_t op;

        Instruction(Statement stmt, uint16_t op)
            : stmt(std::move(stmt)), op(op) {}
    };

    std::vector<Instruction> generateInstructions(const std::vector<Statement>& statements);

}