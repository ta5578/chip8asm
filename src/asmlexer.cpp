#include "asmlexer.h"
#include <cctype>
#include "utils.h"

AsmLexer::AsmLexer(FileReader& reader) : buf(reader.read_all()), cursor(0) {}

Token AsmLexer::get_next_token()
{
    skip_white_space();

    /* We don't want comment tokens */
    if (buf[cursor] == ';') {
        while (cursor < buf.size() && buf[cursor] != '\n') {
            ++cursor;
        }
        return get_next_token();
    }

    std::string tok;
    tok.reserve(10);
    while (cursor < buf.size() && !isspace(buf[cursor])) {
        tok += buf[cursor++];
        if (tok == ",") {
            return {TokenType::COMMA, tok};
        } else if (is_operator(tok)) {
            return {TokenType::OPERATOR, tok};
        } else if (is_hex(tok)) {
            return {TokenType::HEX, tok};
        } else if (is_register(tok)) {
            return {TokenType::REGISTER, tok};
        }
    }
    return {TokenType::LABEL, tok};
}

void AsmLexer::skip_white_space()
{
    while (cursor < buf.size() && isspace(buf[cursor])) {
        ++cursor;
    }
}
