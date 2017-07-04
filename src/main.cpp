#include <cassert>
#include <cstdlib>
#include <string>
#include <iostream>
#include "utils.h"
#include "FileReader.h"
#include "vm_def.h"
#include "asmlexer.h"
#include "bingenerator.h"
#include <exception>
#include "ParseException.h"

static bool parse_args(int argc, char **argv, AsmOpts *opts)
{
    if (argc < 2) {
        return false;
    }

    std::string first(argv[1]);
    if (first == "-h") {
        opts->show_help = true;
        return true;
    }
    opts->in_file = argv[1];

    for (int i = 2; i < argc; ++i) {
        std::string arg(argv[i]);
        if (arg == "--dump-asm") {
            opts->dump_asm = true;
        } else if (arg == "--output" || arg == "-o") {
            opts->out_file = argv[i + 1];
            if (opts->out_file == nullptr) {
                std::cerr << "Output flag specified without an output file!\n";
                return false;
            }
            ++i;
        } else {
            return false;
        }
    }
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

        std::cout << "Reading from '" << opts.in_file << "'' and writing to '" << opts.out_file << "'.\n";
        std::cout << "Dump ASM: " << (opts.dump_asm ? "true" : "false") << "\n";

        FileReader reader(argv[1]);
        AsmLexer lexer(reader);
        BinGenerator gen(lexer, opts);
        gen.generate_bin();
        std::cout << "End of " << std::string("$EEE").substr(1) << "\n";
        std::cout << "Hex: " << to_hex("$EEE") << "\n";
        std::cout << "Binary ROM successfully generated!\n";

    } catch (const std::invalid_argument& e) {
        std::cerr << "Caught invalid argument: " << e.what() << "\n";
    } catch (const ParseException& e) {
        std::cerr << "Caught Parse Exception: " << e.what() << "\n";
    } catch (const std::exception& e) {
        std::cerr << "Caught generic exception: " << e.what() << "\n";
    } catch (...) {
        std::cerr << "Unknown error! Please retry!\n";
    }
}
