#pragma once

#include <utility>
#include <string>
#include <ostream>

enum class TokenType {
    OPERATOR, /* One of the recognized operators                 */
    LABEL,    /* A label                                         */
    HEX,      /* Hexadecimal value starting with '$'             */
    REGISTER, /* Register starting with 'r' (case - insensitive) */
    COMMA    /* A comma ','                                     */
};

typedef std::pair<TokenType, std::string> Token;

static std::ostream& operator<<(std::ostream& os, const Token type)
{
    std::string s;
    switch (type.first) {
        case TokenType::OPERATOR:
            s = "OPERATOR";
            break;
        case TokenType::LABEL:
            s = "LABEL";
            break;
        case TokenType::HEX:
            s = "HEX";
            break;
        case TokenType::REGISTER:
            s = "REGISTER";
            break;
        case TokenType::COMMA:
            s = "COMMA";
            break;
    }
    os << "Token: '" << type.second << "' Type: " << s << "\n";
    return os;
}
