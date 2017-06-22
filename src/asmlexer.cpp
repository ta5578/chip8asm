#include "asmlexer.h"
#include <cctype>
#include "utils.h"

AsmLexer::AsmLexer(FileReader& reader) : buf(reader.read_all()), cursor(0) {}

Token AsmLexer::get_next_token()
{
    while (cursor < buf.size() && isspace(buf[cursor])) {
        ++cursor;
    }

    std::string tok;
    tok.reserve(10);
    while (cursor < buf.size() && !isspace(buf[cursor])) {
        tok += buf[cursor++];
        if (tok == ",") {
            return {TokenType::COMMA, tok};
        } else if (is_operator(tok)) {
            return {TokenType::OPERATOR, tok};
        } else if (tok[0] == '$') {
            while (!isspace(buf[cursor]) && isdigit(buf[cursor])) {
                tok += buf[cursor++];
            }
            return {TokenType::HEX, tok};
        } else if (std::tolower(tok[0]) == 'r') {
            int i = 0; /* Registers are [rR][0-15]{1,2} */
            while (cursor < buf.size() && std::isdigit(buf[cursor]) && i < 2) {
                tok += buf[cursor++];
                ++i;
            }
            return {TokenType::REGISTER, tok};
        }
    }

    /* We don't want comment tokens */
    if (buf[cursor - tok.size()] == ';') {
        while (cursor < buf.size() && buf[cursor] != '\n') {
            ++cursor;
        }
        return get_next_token();
    }
    return {TokenType::LABEL, tok};
}
