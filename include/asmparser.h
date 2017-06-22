#pragma once

#include "asmlexer.h"

class AsmParser {
    AsmLexer lexer;

public:
    AsmParser(const AsmLexer& lexer);

};