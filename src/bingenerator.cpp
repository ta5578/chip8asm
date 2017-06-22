#include "bingenerator.h"
#include "asmparser.h"
#include <assert.h>
#include <ctype.h>
#include "utils.h"

BinGenerator::BinGenerator(const AsmParser& parser)
    :parser(parser), current_addr(0) {}

std::string BinGenerator::generate_bin()
{
    return "";
}

/* in the start state, we expect either a label or operator */
static bool process_start_state(BinGenerator *gen, const char *token, size_t len)
{
    return true;
}

static bool process_operator_state(BinGenerator *gen, const char *token, size_t len)
{
    return true;
}