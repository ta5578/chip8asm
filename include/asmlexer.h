#pragma once

#include <string>
#include "FileReader.h"
#include "token.h"

class AsmLexer {

private:
    std::string buf;
    size_t cursor;
    void skip_white_space();

public:
    AsmLexer(FileReader& reader);
    Token get_next_token();
};