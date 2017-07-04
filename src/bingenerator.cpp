#include "bingenerator.h"
#include "ParseException.h"
#include "token.h"
#include <iostream>

BinGenerator::BinGenerator(const AsmLexer& lexer, const AsmOpts& opts) : lexer(lexer), opts(opts),
    fwriter(opts.out_file, FileBit::BINARY), curr_label("SOF"), curr_addr(0x2000) {}

void BinGenerator::parse()
{
    Token tok;
    do {
        tok = lexer.get_next_token();
        std::string str = tok.second;
        auto type = tok.first;
        if (type == TokenType::LABEL) {
            process_label(str);
        } else if (type == TokenType::OPERATOR) {
            process_operator(str);
        } else {
            throw ParseException(str + " is not a valid starting token! (OPERATOR|LABEL) expected!");
        }
    } while (!tok.second.empty());
}

void BinGenerator::generate_bin()
{
    for (const auto& it : addrs) {
        std::cout << "Label: " << it.first << " Addr: " << it.second << ".\n";
    }
}

bool BinGenerator::label_exists(const std::string& lbl) const
{
    return parse_tree.find(lbl) != parse_tree.end();
}

uint16_t BinGenerator::get_opcode(const std::string& op, const std::vector<std::string>& args) const
{
    auto itr = OPERATORS.find(op);
    return itr->second(args);
}

void BinGenerator::process_label(const std::string& label)
{
    if (label_exists(label)) {
        throw ParseException(label + " is already a label!");
    } else {
        parse_tree.insert({label, {}});
        addrs.insert({label, curr_addr});
        curr_label = label;
    }
}

void BinGenerator::process_operator(const std::string& str)
{
    /* Not implemented */
    if (str == "SYS") {
        return;
    }

    /* Expects no arguments */
    if (one_of<std::string>(str, {"CLR", "RET"})) {
        auto op = get_opcode(str, {});
        fwriter.write(op);

        /* Expects label or hex */
    } else if (one_of<std::string>(str, {"JMP", "CALL", "ZJMP", "ILOAD"})) {
        auto t1 = lexer.get_next_token();
        if (t1.first != TokenType::LABEL && t1.first != TokenType::HEX) {
            throw ParseException(str + " expects a label or hex address as an operand!");
        }
        std::vector<std::string> args;
        if (t1.first == TokenType::LABEL) {
            args.push_back(std::to_string(addrs[t1.second]));
        } else {
            args.push_back(t1.second);
        }
        auto op = get_opcode(str, args);
        fwriter.write(op);

        /* Expects register, comma, and hex */
    } else if (one_of<std::string>(str, {"SKE", "SKNE", "SKRE", "LOAD", "ADD", "RAND"})) {
        auto t1 = lexer.get_next_token();
        if (t1.first != TokenType::REGISTER) {
            throw ParseException("REGISTER expected after " + str + "!\n");
        }
        auto t2 = lexer.get_next_token();
        if (t2.first != TokenType::COMMA) {
            throw ParseException("COMMA expected after " + t1.second + "!\n");
        }
        auto t3 = lexer.get_next_token();
        if (t3.first != TokenType::HEX) {
            throw ParseException("HEX expected after " + t2.second + "!\n");
        }
        auto op = get_opcode(str, {t1.second, t3.second});
        fwriter.write(op);

        /* Expects 2 registers */
    } else if (one_of<std::string>(str, {"ASN", "OR", "AND", "XOR", "RADD", "SUB", "RSUB", "SKRNE"})) {
        auto t1 = lexer.get_next_token();
        if (t1.first != TokenType::REGISTER) {
            throw ParseException("REGISTER expected after " + str + "!\n");
        }
        auto t2 = lexer.get_next_token();
        if (t2.first != TokenType::COMMA) {
            throw ParseException("COMMA expected after " + t1.second + "!\n");
        }
        auto t3 = lexer.get_next_token();
        if (t3.first != TokenType::REGISTER) {
            throw ParseException("REGISTER expected after " + t2.second + "!\n");
        }
        auto op = get_opcode(str, {t1.second, t3.second});
        fwriter.write(op);

        /* Expects one register */
    } else if (one_of<std::string>(str, {"SHR", "SHL", "SKK", "SKNK", "DELA", "KEYW", "DELR", "SNDR", "IADD", "SILS", "BCD", "DUMP", "IDUMP"})) {
        auto t1 = lexer.get_next_token();
        if (t1.first != TokenType::REGISTER) {
            throw ParseException("REGISTER expected after " + str + "!\n");
        }
        auto op = get_opcode(str, {t1.second});
        fwriter.write(op);

    } else if (str == "DRAW") { /* DRAW is the only operator to take three operands */
        auto t1 = lexer.get_next_token();
        if (t1.first != TokenType::REGISTER) {
            throw ParseException("REGISTER expected after " + str + "!\n");
        }
        auto t2 = lexer.get_next_token();
        if (t2.first != TokenType::COMMA) {
            throw ParseException("COMMA expected after " + t1.second + "!\n");
        }
        auto t3 = lexer.get_next_token();
        if (t3.first != TokenType::REGISTER) {
            throw ParseException("REGISTER expected after " + t2.second + "!\n");
        }
        auto t4 = lexer.get_next_token();
        if (t4.first != TokenType::COMMA) {
            throw ParseException("COMMA expected after " + t3.second + "!\n");
        }
        auto t5 = lexer.get_next_token();
        if (t5.first != TokenType::HEX) {
            throw ParseException("HEX expected after " + t4.second + "!\n");
        }
        auto op = get_opcode(str, {t1.second, t3.second, t5.second});
        fwriter.write(op);

    } else { /* Must be special instruction LB */
        auto t1 = lexer.get_next_token();
        if (t1.first != TokenType::HEX) {
            throw ParseException("HEX expected after " + str + "!\n");
        }
        auto op = get_opcode(str, {t1.second});
        fwriter.write(op);
    }
}
