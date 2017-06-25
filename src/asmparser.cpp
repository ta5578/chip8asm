#include "asmparser.h"
#include "utils.h"
#include "ParseException.h"
#include "token.h"

AsmParser::AsmParser(const AsmLexer& lexer) : lexer(lexer) {}

void AsmParser::parse()
{
    Token tok;
    do {
        tok = lexer.get_next_token();
        const auto str = tok.second;
        auto type = tok.first;
        if (type == TokenType::LABEL) {
            if (label_exists(str)) {
                throw ParseException(str + " is already a label!");
            } else {
                parse_tree.insert({str, std::vector<uint16_t>()});
            }
        }
    } while (!tok.second.empty());
}

bool AsmParser::label_exists(const std::string& lbl) const
{
    return parse_tree.find(lbl) != parse_tree.end();
}