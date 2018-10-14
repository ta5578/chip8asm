#include "Parser.h"
#include "ParseException.h"
#include "utils.h"

c8::Parser::Parser(emu::Lexer lexer)
    : _lexer(lexer), _currAddress(0x0200) {}

void c8::Parser::parse()
{
    emu::Token tok;
    do {
        tok = _lexer.get_next_token();
        LOG("Token '%s' retrieved.", tok._str.c_str());

        if (tok._type == emu::TokenType::LABEL) {
            parse_label(tok._str);
        } else if (tok._type == emu::TokenType::OPERATOR) {
            parse_operator(tok._str);
        } else {
            throw ParseException(tok._str + " is not a valid starting token! (OPERATOR|LABEL) expected!");
        }
    } while (!tok._str.empty());
}

const std::vector<Instruction>& c8::Parser::getInstructions() const
{
    return _instructions;
}

const std::map<std::string, uint16_t>& c8::Parser::getLabels() const
{
    return _labelToAddress;
}

bool c8::Parser::label_exists(const std::string& label) const
{
    return _labelToAddress.find(label) != _labelToAddress.end();
}

void c8::Parser::parse_label(const std::string& label)
{
    if (label_exists(label)) {
        throw ParseException(label + " label is redefined!");
    }

    _currLabel = label;
    if (!_currLabel.empty()) {
        _labelToAddress.insert({ _currLabel, _currAddress });
    }
}

void c8::Parser::parse_operator(const std::string& op)
{
    /* Not implemented */
    if (op == "SYS") {
        return;
    }

    std::vector<std::string> args;

    /* Expects no arguments */
    if (one_of<std::string>(op, { "CLR", "RET" })) {

        /* Expects label or hex */
    } else if (one_of<std::string>(op, { "JMP", "CALL", "ZJMP", "ILOAD" })) {
        auto t1 = _lexer.get_next_token();
        if (t1._type != emu::TokenType::LABEL && t1._type != emu::TokenType::HEX) {
            throw ParseException(op + " expects a label or hex address as an operand!");
        }
        args.push_back(t1._str);

        /* Expects register, comma, and hex */
    } else if (one_of<std::string>(op, { "SKE", "SKNE", "SKRE", "LOAD", "ADD", "RAND" })) {
        auto t1 = _lexer.get_next_token();
        if (t1._type != emu::TokenType::REGISTER) {
            throw ParseException("REGISTER expected after " + op + "!\n");
        }
        auto t2 = _lexer.get_next_token();
        if (t2._type != emu::TokenType::COMMA) {
            throw ParseException("COMMA expected after " + t1._str + "!\n");
        }
        auto t3 = _lexer.get_next_token();
        if (t3._type != emu::TokenType::HEX) {
            throw ParseException("HEX expected after " + t2._str + "!\n");
        }
        args.push_back(t1._str);
        args.push_back(t3._str);

        /* Expects 2 registers */
    } else if (one_of<std::string>(op, { "ASN", "OR", "AND", "XOR", "RADD", "SUB", "RSUB", "SKRNE" })) {
        auto t1 = _lexer.get_next_token();
        if (t1._type != emu::TokenType::REGISTER) {
            throw ParseException("REGISTER expected after " + op + "!\n");
        }
        auto t2 = _lexer.get_next_token();
        if (t2._type != emu::TokenType::COMMA) {
            throw ParseException("COMMA expected after " + t1._str + "!\n");
        }
        auto t3 = _lexer.get_next_token();
        if (t3._type != emu::TokenType::REGISTER) {
            throw ParseException("REGISTER expected after " + t2._str + "!\n");
        }
        args.push_back(t1._str);
        args.push_back(t3._str);

        /* Expects one register */
    } else if (one_of<std::string>(op, { "SHR", "SHL", "SKK", "SKNK", "DELA", "KEYW", "DELR", "SNDR", "IADD", "SILS", "BCD", "DUMP", "IDUMP" })) {
        auto t1 = _lexer.get_next_token();
        if (t1._type != emu::TokenType::REGISTER) {
            throw ParseException("REGISTER expected after " + op + "!\n");
        }
        args.push_back(t1._str);

    } else if (op == "DRAW") { /* DRAW is the only operator to take three operands */
        auto t1 = _lexer.get_next_token();
        if (t1._type != emu::TokenType::REGISTER) {
            throw ParseException("REGISTER expected after " + op + "!\n");
        }
        auto t2 = _lexer.get_next_token();
        if (t2._type != emu::TokenType::COMMA) {
            throw ParseException("COMMA expected after " + t1._str + "!\n");
        }
        auto t3 = _lexer.get_next_token();
        if (t3._type != emu::TokenType::REGISTER) {
            throw ParseException("REGISTER expected after " + t2._str + "!\n");
        }
        auto t4 = _lexer.get_next_token();
        if (t4._type != emu::TokenType::COMMA) {
            throw ParseException("COMMA expected after " + t3._str + "!\n");
        }
        auto t5 = _lexer.get_next_token();
        if (t5._type != emu::TokenType::HEX) {
            throw ParseException("HEX expected after " + t4._str + "!\n");
        }
        args.push_back(t1._str);
        args.push_back(t3._str);
        args.push_back(t5._str);

    } else { /* Must be special instruction LB */
        auto t1 = _lexer.get_next_token();
        if (t1._type != emu::TokenType::HEX) {
            throw ParseException("HEX expected after " + op + "!\n");
        }
        args.push_back(t1._str);
    }

    /* Now put it into the symbol table */
    _instructions.push_back({ _currLabel, op, args, _currAddress });
    _currAddress += 2;
}