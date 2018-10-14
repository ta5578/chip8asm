#include "bingenerator.h"
#include "ParseException.h"
#include "opcodes.h"
#include <iostream>

BinGenerator::BinGenerator(const emu::Lexer& lexer, const AsmOpts& opts) :
    lexer(lexer), opts(opts), fp(std::fopen(opts.out_file, "wb")),
    curr_label(), curr_addr(0x0200) {}

BinGenerator::~BinGenerator()
{
    std::fclose(fp);
}

void BinGenerator::parse()
{
    emu::Token tok;
    do {
        tok = lexer.get_next_token();
        std::string str = tok._str;
        LOG("Token '%s' retrieved.", str.c_str());

        auto type = tok._type;
        if (type == emu::TokenType::LABEL) {
            process_label(str);
        } else if (type == emu::TokenType::OPERATOR) {
            process_operator(str);
        } else {
            throw ParseException(str + " is not a valid starting token! (OPERATOR|LABEL) expected!");
        }
    } while (!tok._str.empty());
}

void BinGenerator::dump_asm() const
{
    std::cout << "-------- Asm Dump --------\n";
    for (const auto& i : instructions) {
        uint16_t op = 0;
        /* Replace labels with their addresses */
        if (one_of<std::string>(i.op, {"JMP", "CALL", "ZJMP", "ILOAD"})) {
            auto label = i.args[0];
            auto arg = from_hex(label_addrs.at(label));
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
    /* First create the instruction set and assign addresses to labels and operations */
    parse();

    /* Now perform the binary conversion and write it to the file */
    for (const auto& i : instructions) {
        uint16_t op = 0;
        /* Replace labels with their addresses */
        if (one_of<std::string>(i.op, {"JMP", "CALL", "ZJMP", "ILOAD"})) {
            auto label = i.args[0];
            auto arg = from_hex(label_addrs.at(label));
            op = get_opcode(i.op, {arg});
        } else {
            op = get_opcode(i.op, i.args);
        }
        /* Correct for the host machine endianness to chip 8 big endian */
        op = endi(op);
        std::fwrite(&op, sizeof(op), 1, fp);
    }

    LOG("-------- Label Addresses --------");
    for (const auto& it : label_addrs) {
        LOG("%s -> %0x%04X", it.first.c_str(), it.second);
    }
    LOG("-------- End Addresses --------");

    /* Now dump assembly to the screen if requested */
    if (opts.dump_asm) {
        dump_asm();
    }
}

bool BinGenerator::label_exists(const std::string& lbl) const
{
    return label_addrs.find(lbl) != label_addrs.end();
}

uint16_t BinGenerator::get_opcode(const std::string& op, const std::vector<std::string>& args) const
{
    auto itr = OPERATORS.find(op);
    return itr->second(args);
}

void BinGenerator::process_label(const std::string& label)
{
    if (label_exists(label)) {
        throw ParseException(label + " label is redefined!");
    }

    curr_label = label;
    if (!curr_label.empty()) {
        label_addrs.insert({curr_label, curr_addr});
    }
}

void BinGenerator::process_operator(const std::string& str)
{
    /* Not implemented */
    if (str == "SYS") {
        return;
    }

    std::vector<std::string> args;

    /* Expects no arguments */
    if (one_of<std::string>(str, {"CLR", "RET"})) {

        /* Expects label or hex */
    } else if (one_of<std::string>(str, {"JMP", "CALL", "ZJMP", "ILOAD"})) {
        auto t1 = lexer.get_next_token();
        if (t1._type != emu::TokenType::LABEL && t1._type != emu::TokenType::HEX) {
            throw ParseException(str + " expects a label or hex address as an operand!");
        }
        args.push_back(t1._str);

        /* Expects register, comma, and hex */
    } else if (one_of<std::string>(str, {"SKE", "SKNE", "SKRE", "LOAD", "ADD", "RAND"})) {
        auto t1 = lexer.get_next_token();
        if (t1._type != emu::TokenType::REGISTER) {
            throw ParseException("REGISTER expected after " + str + "!\n");
        }
        auto t2 = lexer.get_next_token();
        if (t2._type != emu::TokenType::COMMA) {
            throw ParseException("COMMA expected after " + t1._str + "!\n");
        }
        auto t3 = lexer.get_next_token();
        if (t3._type != emu::TokenType::HEX) {
            throw ParseException("HEX expected after " + t2._str + "!\n");
        }
        args.push_back(t1._str);
        args.push_back(t3._str);

        /* Expects 2 registers */
    } else if (one_of<std::string>(str, {"ASN", "OR", "AND", "XOR", "RADD", "SUB", "RSUB", "SKRNE"})) {
        auto t1 = lexer.get_next_token();
        if (t1._type != emu::TokenType::REGISTER) {
            throw ParseException("REGISTER expected after " + str + "!\n");
        }
        auto t2 = lexer.get_next_token();
        if (t2._type != emu::TokenType::COMMA) {
            throw ParseException("COMMA expected after " + t1._str + "!\n");
        }
        auto t3 = lexer.get_next_token();
        if (t3._type != emu::TokenType::REGISTER) {
            throw ParseException("REGISTER expected after " + t2._str + "!\n");
        }
        args.push_back(t1._str);
        args.push_back(t3._str);

        /* Expects one register */
    } else if (one_of<std::string>(str, {"SHR", "SHL", "SKK", "SKNK", "DELA", "KEYW", "DELR", "SNDR", "IADD", "SILS", "BCD", "DUMP", "IDUMP"})) {
        auto t1 = lexer.get_next_token();
        if (t1._type != emu::TokenType::REGISTER) {
            throw ParseException("REGISTER expected after " + str + "!\n");
        }
        args.push_back(t1._str);

    } else if (str == "DRAW") { /* DRAW is the only operator to take three operands */
        auto t1 = lexer.get_next_token();
        if (t1._type != emu::TokenType::REGISTER) {
            throw ParseException("REGISTER expected after " + str + "!\n");
        }
        auto t2 = lexer.get_next_token();
        if (t2._type != emu::TokenType::COMMA) {
            throw ParseException("COMMA expected after " + t1._str + "!\n");
        }
        auto t3 = lexer.get_next_token();
        if (t3._type != emu::TokenType::REGISTER) {
            throw ParseException("REGISTER expected after " + t2._str + "!\n");
        }
        auto t4 = lexer.get_next_token();
        if (t4._type != emu::TokenType::COMMA) {
            throw ParseException("COMMA expected after " + t3._str + "!\n");
        }
        auto t5 = lexer.get_next_token();
        if (t5._type != emu::TokenType::HEX) {
            throw ParseException("HEX expected after " + t4._str + "!\n");
        }
        args.push_back(t1._str);
        args.push_back(t3._str);
        args.push_back(t5._str);

    } else { /* Must be special instruction LB */
        auto t1 = lexer.get_next_token();
        if (t1._type != emu::TokenType::HEX) {
            throw ParseException("HEX expected after " + str + "!\n");
        }
        args.push_back(t1._str);
    }

    /* Now put it into the symbol table */
    instructions.push_back({curr_label, str, args, curr_addr});
    curr_addr += 2;
}
