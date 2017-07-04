#pragma once

#include <utility>
#include <string>

enum class TokenType {
    OPERATOR, /* One of the recognized operators                 */
    LABEL,    /* A label                                         */
    HEX,      /* Hexadecimal value starting with '$'             */
    REGISTER, /* Register starting with 'r' (case - insensitive) */
    COMMA    /* A comma ','                                     */
};

typedef std::pair<TokenType, std::string> Token;
