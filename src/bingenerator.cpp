#include "bingenerator.h"
#include "ParseException.h"
#include "opcodes.h"
#include <iostream>

BinGenerator::BinGenerator(const std::vector<Instruction>& instructions, const std::map<std::string, uint16_t>& labels, const AsmOpts& opts)
    : _instructions(instructions), _labels(labels), opts(opts), fp(std::fopen(opts.out_file, "wb")) {}
    

BinGenerator::~BinGenerator()
{
    std::fclose(fp);
}


void BinGenerator::dump_asm() const
{
    std::cout << "-------- Asm Dump --------\n";
    for (const auto& i : _instructions) {
        uint16_t op = 0;
        /* Replace labels with their addresses */
        if (one_of<std::string>(i.op, {"JMP", "CALL", "ZJMP", "ILOAD"})) {
            auto label = i.args[0];
            auto arg = from_hex(_labels.at(label));
            op = get_opcode(i.op, {arg});
        } else {
            op = get_opcode(i.op, i.args);
        }
        std::cout << from_hex(i.addr) << " -- " << from_hex(op) << " ; ";
        std::string s = i.op + " ";
        for (const auto& a : i.args) {
            s += a;
            s += ", ";
        }
        /* Remove the last comma */
        s = s.substr(0, s.find_last_of(","));
        std::cout << s << "\n";
    }
    std::cout << "-------- End Dump --------\n";
}

void BinGenerator::generate_bin()
{
    /* Now perform the binary conversion and write it to the file */
    for (const auto& i : _instructions) {
        uint16_t op = 0;
        /* Replace labels with their addresses */
        if (one_of<std::string>(i.op, {"JMP", "CALL", "ZJMP", "ILOAD"})) {
            auto label = i.args[0];
            auto arg = from_hex(_labels.at(label));
            op = get_opcode(i.op, {arg});
        } else {
            op = get_opcode(i.op, i.args);
        }
        /* Correct for the host machine endianness to chip 8 big endian */
        op = endi(op);
        std::fwrite(&op, sizeof(op), 1, fp);
    }

#ifndef NDEBUG
    LOG("-------- Label Addresses --------");
    for (const auto& it : _labels) {
        LOG("%s -> 0x%04X", it.first.c_str(), it.second);
    }
    LOG("-------- End Addresses --------");
#endif

    /* Now dump assembly to the screen if requested */
    if (opts.dump_asm) {
        dump_asm();
    }
}

uint16_t BinGenerator::get_opcode(const std::string& op, const std::vector<std::string>& args) const
{
    auto itr = OPERATORS.find(op);
    return itr->second(args);
}