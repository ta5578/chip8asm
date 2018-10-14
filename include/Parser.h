#pragma once

#include <vector>
#include <map>
#include "Lexer.h"
#include "vm_def.h"

namespace c8 {

    class Parser {
    public:
        Parser(emu::Lexer lexer);
        void parse();

        const std::vector<Instruction>& getInstructions() const;
        const std::map<std::string, uint16_t>& getLabels() const;

    private:
        emu::Lexer _lexer;
        std::map<std::string, uint16_t> _labelToAddress;
        std::vector<Instruction> _instructions;
        std::string _currLabel;
        uint16_t _currAddress;

        bool label_exists(const std::string& lbl) const;
        void parse_label(const std::string& label);
        void parse_operator(const std::string& op);
    };
}