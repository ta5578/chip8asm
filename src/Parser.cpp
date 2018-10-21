#include "Parser.h"
#include "ParseException.h"
#include "utils.h"

c8::Parser::Parser(c8::Lexer lexer)
    : _lexer(lexer), _currAddress(0x0200) {}

std::vector<c8::Statement> c8::Parser::parse()
{
    c8::Token tok;
    std::map<std::string, uint16_t> labelToAddress;
    std::vector<Statement> statements;
    do {
        tok = _lexer.get_next_token();
        LOG("Token '%s' retrieved.", tok._str.c_str());

        if (tok._type == c8::TokenType::LABEL) {
            parse_label(tok._str, labelToAddress);
        } else if (tok._type == c8::TokenType::OPERATOR) {
            parse_operator(tok._str, statements);
        } else {
            throw ParseException(tok._str + " is not a valid starting token! (OPERATOR|LABEL) expected!");
        }
    } while (!tok._str.empty());

    replaceLabelsWithAddress(statements, labelToAddress);

    for (auto& p : labelToAddress) {
        LOG("%s -> 0x%04X", p.first.c_str(), p.second);
    }
    return statements;
}

void c8::Parser::parse_label(const std::string& label, std::map<std::string, uint16_t>& labels)
{
    if (labels.count(label) > 0) {
        throw ParseException(label + " label is redefined!");
    }

    _currLabel = label;
    if (!_currLabel.empty()) {
        labels.insert({ _currLabel, _currAddress });
    }
}

void c8::Parser::parse_operator(const std::string& op, std::vector<Statement>& statements)
{
    /* Not implemented */
    if (op == "SYS") {
        return;
    }

    std::vector<std::string> args;
    uint16_t offset = 0;
    /* Expects no arguments */
    if (one_of<std::string>(op, { "CLR", "RET" })) {

        /* Expects label or hex */
    } else if (one_of<std::string>(op, { "JMP", "CALL", "ZJMP", "ILOAD" })) {
        auto t1 = _lexer.get_next_token();
        if (t1._type != c8::TokenType::LABEL && t1._type != c8::TokenType::HEX) {
            throw ParseException(op + " expects a label or hex address as an operand!");
        }
        args.push_back(t1._str);
        offset = 2;
        /* Expects register, comma, and hex */
    } else if (one_of<std::string>(op, { "SKE", "SKNE", "SKRE", "LOAD", "ADD", "RAND" })) {
        auto t1 = _lexer.get_next_token();
        if (t1._type != c8::TokenType::REGISTER) {
            throw ParseException("REGISTER expected after " + op + "!\n");
        }
        auto t2 = _lexer.get_next_token();
        if (t2._type != c8::TokenType::COMMA) {
            throw ParseException("COMMA expected after " + t1._str + "!\n");
        }
        auto t3 = _lexer.get_next_token();
        if (t3._type != c8::TokenType::HEX) {
            throw ParseException("HEX expected after " + t2._str + "!\n");
        }
        args.push_back(t1._str);
        args.push_back(t3._str);
        offset = 2;
        /* Expects 2 registers */
    } else if (one_of<std::string>(op, { "ASN", "OR", "AND", "XOR", "RADD", "SUB", "RSUB", "SKRNE" })) {
        auto t1 = _lexer.get_next_token();
        if (t1._type != c8::TokenType::REGISTER) {
            throw ParseException("REGISTER expected after " + op + "!\n");
        }
        auto t2 = _lexer.get_next_token();
        if (t2._type != c8::TokenType::COMMA) {
            throw ParseException("COMMA expected after " + t1._str + "!\n");
        }
        auto t3 = _lexer.get_next_token();
        if (t3._type != c8::TokenType::REGISTER) {
            throw ParseException("REGISTER expected after " + t2._str + "!\n");
        }
        args.push_back(t1._str);
        args.push_back(t3._str);
        offset = 2;
        /* Expects one register */
    } else if (one_of<std::string>(op, { "SHR", "SHL", "SKK", "SKNK", "DELA", "KEYW", "DELR", "SNDR", "IADD", "SILS", "BCD", "DUMP", "IDUMP" })) {
        auto t1 = _lexer.get_next_token();
        if (t1._type != c8::TokenType::REGISTER) {
            throw ParseException("REGISTER expected after " + op + "!\n");
        }
        args.push_back(t1._str);
        offset = 2;
    } else if (op == "DRAW") { /* DRAW is the only operator to take three operands */
        auto t1 = _lexer.get_next_token();
        if (t1._type != c8::TokenType::REGISTER) {
            throw ParseException("REGISTER expected after " + op + "!\n");
        }
        auto t2 = _lexer.get_next_token();
        if (t2._type != c8::TokenType::COMMA) {
            throw ParseException("COMMA expected after " + t1._str + "!\n");
        }
        auto t3 = _lexer.get_next_token();
        if (t3._type != c8::TokenType::REGISTER) {
            throw ParseException("REGISTER expected after " + t2._str + "!\n");
        }
        auto t4 = _lexer.get_next_token();
        if (t4._type != c8::TokenType::COMMA) {
            throw ParseException("COMMA expected after " + t3._str + "!\n");
        }
        auto t5 = _lexer.get_next_token();
        if (t5._type != c8::TokenType::HEX) {
            throw ParseException("HEX expected after " + t4._str + "!\n");
        }
        args.push_back(t1._str);
        args.push_back(t3._str);
        args.push_back(t5._str);
        offset = 2;
    } else { /* Must be special instruction LB */
        auto t1 = _lexer.get_next_token();
        if (t1._type != c8::TokenType::HEX) {
            throw ParseException("HEX expected after " + op + "!\n");
        }
        args.push_back(t1._str);
        offset = 1;
    }

    /* Now put it into the symbol table */
    statements.push_back({ _currLabel, op, args, _currAddress });
    _currAddress += offset;
}

void c8::Parser::replaceLabelsWithAddress(std::vector<Statement>& statements, const std::map<std::string, uint16_t>& labels)
{
    for (auto& stmt : statements) {
        /* Only these instructions accept labels. */
        if (one_of<std::string>(stmt.op, { "JMP", "CALL", "ZJMP", "ILOAD" })) {
            auto label = stmt.args[0];
            if (labels.count(label) == 0) {
                throw ParseException(label + " is a label that hasn't been defined.");
            }
            stmt.args[0] = from_hex(labels.at(label));
        }
    }
}