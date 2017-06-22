#include <cassert>
#include <cstdlib>
#include <string>
#include <iostream>
#include "utils.h"
#include "FileReader.h"
#include "vm_def.h"
#include "asmlexer.h"
#include "asmparser.h"
#include "bingenerator.h"
#include <exception>

static bool parse_args(int argc, char **argv, AsmOpts *opts)
{
    return true;
}

static void show_help()
{
    std::cout << "chip8asm is an assembler for the chip 8 VM.\n";
    std::cout << "The only required argument is the input .asm file.\n";
    std::cout << "The first argument should be one of the input file or help.\n";
    std::cout << "Here are the supported options:\n";
    std::cout << "   --dump-asm | -dasm -- dumps the assembled statements with memory locations\n";
    std::cout << "   --output | -o -- the name of the output ROM file. By default, it is 'a.rom'\n";
    std::cout << "   --help | -h -- displays this help screen\n";
}

int main(int argc, char **argv)
{
    try {
        AsmOpts opts;
        if (!parse_args(argc, argv, &opts)) {
            show_help();
            return EXIT_FAILURE;
        }

        if (opts.show_help) {
            show_help();
            return EXIT_SUCCESS;
        }

        std::cout << "Reading from " << opts.in_file << " and writing to " << opts.out_file << "\n";

        FileReader reader(argv[1]);
        AsmLexer lexer(reader);
        AsmParser parser(lexer);
        BinGenerator gen(parser);
        std::cout << "Generated bin: " << gen.generate_bin() << "\n";

    } catch (const std::invalid_argument& e) {
        std::cerr << "Caught invalid argument: " << e.what() << "\n";
    } catch (const std::exception& e) {
        std::cerr << "Caught generic exception: " << e.what() << "\n";
    } catch (...) {
        std::cerr << "Unknown error! Please retry!\n";
    }
}
