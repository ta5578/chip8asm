#include "Lexer.h"
#include <cctype>
#include "opcodes.h"

emu::Lexer::Lexer(const std::string& buf) 
    : _buf(buf), _cursor(0) {}

emu::Token emu::Lexer::get_next_token()
{
    skip_white_space();

    std::string tok;
    tok.reserve(10);
    while (_cursor < _buf.size() && !isspace(_buf[_cursor])) {
        tok += _buf[_cursor++];
        if (tok == ",") {
            return {TokenType::COMMA, tok};
        } else if (is_operator(tok)) {
            return {TokenType::OPERATOR, tok};
        } else if (tok[0] == '$') {
            while (_cursor < _buf.size() && is_valid_hex_char(_buf[_cursor])) {
                tok += _buf[_cursor++];
            }
            return {TokenType::HEX, tok};
        } else if (is_register(tok)) {
            return {TokenType::REGISTER, tok};
        } else if (tok[0] == ';') {
            while (_cursor < _buf.size() && _buf[_cursor] != '\n') {
                ++_cursor;
            }
            return get_next_token();
        }
    }
    return {emu::TokenType::LABEL, tok};
}

void emu::Lexer::skip_white_space()
{
    while (_cursor < _buf.size() && isspace(_buf[_cursor])) {
        ++_cursor;
    }
}
