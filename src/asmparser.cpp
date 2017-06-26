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
        std::cout << "Retrieved token " << str << "\n";
        auto type = tok.first;
        if (type == TokenType::LABEL) {
            std::cout << str << " is a label!\n";
            // if (label_exists(str)) {
            //     throw ParseException(str + " is already a label!");
            // } else {
            //     parse_tree.insert({str, std::vector<uint16_t>()});
            //     last_label = str;
            // }
        } else if (type == TokenType::OPERATOR) {
            std::cout << str << " is an operator!\n";
            if (str == "SYS") {
                ; /* Not implemented */
            } else if (str == "CLR" || str == "RET") {
                auto ops = parse_tree[last_label];
                auto op = get_opcode(str, {});
                ops.push_back(op);
            } else if (str == "JMP" || str == "CALL") {
                auto n_label = lexer.get_next_token();
                if (n_label.first != TokenType::LABEL) {
                    throw ParseException(str + " expects a label as an operand!");
                }
            }
        } else if (type == TokenType::HEX) {
            std::cout << str << " is a hex value!\n";
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