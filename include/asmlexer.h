#pragma once

#include <string>
#include "token.h"

class AsmLexer {

private:
    std::string buf;
    size_t cursor;
    void skip_white_space();

public:
    AsmLexer(const char *buf);
    Token get_next_token();
};