#pragma once

#include <vector>
#include <map>
#include "Lexer.h"

namespace c8 {

    struct Statement {
        std::string label, op;
        std::vector<std::string> args;
        uint16_t addr;

        Statement(const std::string& label, const std::string& op,
            const std::vector<std::string>& args, uint16_t addr) :
            label(label), op(op), args(args), addr(addr)
        {}
    };

    class Parser {
    public:
        Parser(c8::Lexer lexer);
        std::vector<Statement> parse();

    private:
        c8::Lexer _lexer;
        std::string _currLabel;
        uint16_t _currAddress;

        void parse_label(const std::string& label, std::map<std::string, uint16_t>& labels);
        void parse_operator(const std::string& op, std::vector<Statement>& statements);
        void replaceLabelsWithAddress(std::vector<Statement>& statements, const std::map<std::string, uint16_t>& labels);
    };
}