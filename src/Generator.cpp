#include "Generator.h"
#include "utils.h"
#include "opcodes.h"

static uint16_t toBinary(const c8::Statement& stmt)
{
    uint16_t op = OPERATORS.find(stmt.op)->second(stmt.args);
    /* Correct for the host machine endianness to chip 8 big endian */
    op = endi(op);
    return op;
}

std::vector<c8::Instruction> c8::generateInstructions(const std::vector<c8::Statement>& statements)
{
    std::vector<c8::Instruction> insts;
    insts.reserve(statements.size());
    for (const auto& stmt : statements) {
        const auto op = toBinary(stmt);
        insts.emplace_back(stmt, op);
    }
    return insts;
}