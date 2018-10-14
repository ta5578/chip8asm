#pragma once

#include <string>
#include "token.h"

class Lexer {

private:
    std::string buf;
    size_t cursor;
    void skip_white_space();

public:
    Lexer(const char *buf);
    Token get_next_token();
};