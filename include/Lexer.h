#pragma once

#include <string>

namespace c8 {
    enum class TokenType {
        OPERATOR, /* One of the recognized operators                 */
        LABEL,    /* A label                                         */
        HEX,      /* Hexadecimal value starting with '$'             */
        REGISTER, /* Register starting with 'r' (case - insensitive) */
        COMMA,    /* A comma ','                                     */
        UNKNOWN
    };

    struct Token {
        TokenType _type;
        std::string _str;
        Token(TokenType type, const std::string& str)
            : _type(type), _str(str) {}
        Token()
            : Token(TokenType::UNKNOWN, "") {}
    };

    class Lexer {
    public:
        Lexer(const std::string& buf);
        Token get_next_token();

    private:
        std::string _buf;
        size_t _cursor;
        void skip_white_space();
    };
}