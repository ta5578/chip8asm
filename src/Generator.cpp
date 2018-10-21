#include "Generator.h"
#include "utils.h"
#include "opcodes.h"

c8::Instruction::Instruction(c8::Statement stmt, uint16_t op)
    : stmt(std::move(stmt)), op(op) {}

std::string c8::Instruction::toString() const
{
    const auto& args = stmt.args;
    const std::string line = stmt.op + " " + asCsv(args.begin(), args.end());
    /* LB is the only operation to take single byte values. */
    if (stmt.op == "LB") {
        uint8_t value = to8Bit(op);
        return fmt("0x%04X | 0x%02X ; %s", stmt.addr, value, line.c_str());
    } else {
        return fmt("0x%04X | 0x%04X ; %s", stmt.addr, op, line.c_str());
    }
}

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