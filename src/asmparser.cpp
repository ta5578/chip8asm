#include "asmparser.h"
#include "utils.h"
#include "ParseException.h"
#include "token.h"
#include <iostream>

AsmParser::AsmParser(const AsmLexer& lexer) : lexer(lexer) {}

void AsmParser::parse()
{
    Token tok;
    std::string last_label = "SOF"; /* Start of file */
    do {
        tok = lexer.get_next_token();
        std::string str = tok.second;
        std::cout << tok;
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

bool AsmParser::label_exists(const std::string& lbl) const
{
    return parse_tree.find(lbl) != parse_tree.end();
}

uint16_t AsmParser::get_opcode(const std::string& op, const std::vector<std::string>& args) const
{
    auto itr = OPERATORS.find(op);
    return itr->second(args);
}

void AsmParser::process_label(const std::string& label)
{
            // if (label_exists(str)) {
            //     throw ParseException(str + " is already a label!");
            // } else {
            //     parse_tree.insert({str, std::vector<uint16_t>()});
            //     last_label = str;
            // }
}

void AsmParser::process_operator(const std::string& str)
{
    /* Not implemented */
    if (str == "SYS") {
        return;
    }

    if (one_of<std::string>(str, {"CLR", "RET"})) {
        // auto ops = parse_tree[last_label];
        // auto op = get_opcode(str, {});
        // ops.push_back(op);
        /* Expects label or hex */
    } else if (one_of<std::string>(str, {"JMP", "CALL", "ZJMP", "ILOAD"})) {
        auto t1 = lexer.get_next_token();
        if (t1.first != TokenType::LABEL && t1.first != TokenType::HEX) {
            throw ParseException(str + " expects a label or hex address as an operand!");
        }
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
        std::cout << t1 << t2 << t3;
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
        std::cout << t1 << t2 << t3;
        /* Expects one register */
    } else if (one_of<std::string>(str, {"SHR", "SHL", "SKK", "SKNK", "DELA", "KEYW", "DELR", "SNDR", "IADD", "SILS", "BCD", "DUMP", "IDUMP"})) {
        auto t1 = lexer.get_next_token();
        if (t1.first != TokenType::REGISTER) {
            throw ParseException("REGISTER expected after " + str + "!\n");
        }
        std::cout << t1;
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
        std::cout << t1 << t2 << t3 << t4 << t5;
    } else { /* Must be special instruction LB */
        auto t1 = lexer.get_next_token();
        if (t1.first != TokenType::HEX) {
            throw ParseException("HEX expected after " + str + "!\n");
        }
        std::cout << t1;
    }
}
