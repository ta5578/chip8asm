#include <cstdlib>
#include <string>
#include "utils.h"
#include "Lexer.h"
#include "Parser.h"
#include <exception>
#include "ParseException.h"
#include "Generator.h"

// the options used by the program
struct AsmOpts {
    const char* in_file; // the file we are reading from.
    const char* out_file; // the file we are writing to.
    bool dump_asm; // flag to determine if we're dumping the assembly to stdout.
    bool show_help; // flag to determine if we're showing help message.
};

static void write_rom(const std::string& filePath, const std::vector<c8::Instruction>& instructions)
{
    std::FILE *fp = std::fopen(filePath.c_str(), "wb");
    if (!fp) {
        throw std::runtime_error("Unable to open file for writing.");
    }

    for (const auto& i : instructions) {
        auto op = i.op;
        /* LB is the only operation to take single byte values. */
        if (i.stmt.op == "LB") {
            auto value = to8Bit(op);
            std::fwrite(&value, sizeof(value), 1, fp);
        } else {
            std::fwrite(&op, sizeof(op), 1, fp);
        }
    }
    std::fclose(fp);
}

static void dump_asm(const std::vector<c8::Instruction>& instructions)
{
    std::puts("-------- ASM Dump --------");
    for (const auto& i : instructions) {
        const std::string line = i.toString();
        std::puts(line.c_str());
    }
    std::puts("-------- End Dump --------");
}

static std::string read_file(const char *path)
{
    std::FILE *fp = std::fopen(path, "r");
    if (!fp) { return ""; }

    fseek(fp, 0, SEEK_END);
    auto fsize = std::ftell(fp);
    std::rewind(fp);

    std::string buf(fsize, '\0');
    std::fread(&buf[0], sizeof(char), fsize, fp);
    std::fclose(fp);

    return buf;
}

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
                std::fprintf(stderr, "Output flag specified without an output file!\n");
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
    std::puts("chip8asm is an assembler for the chip 8 VM.");
    std::puts("The only required argument is the input .asm file.");
    std::puts("The first argument should be one of the input file or help.");
    std::puts("Here are the supported options:");
    std::puts("   --dump-asm | -dasm -- dumps the assembled statements with memory locations");
    std::puts("   --output | -o -- the name of the output ROM file. By default, it is 'a.rom'");
    std::puts("   --help | -h -- displays this help screen");
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

        const std::string text = read_file(argv[1]);
        if (text.empty()) {
            std::fprintf(stderr, "Error reading from '%s'\n", text.c_str());
            return EXIT_FAILURE;
        }

        c8::Lexer lexer(text);
        c8::Parser parser(text);
        auto statements = parser.parse();
        auto instructions = c8::generateInstructions(statements);

        write_rom(opts.out_file, instructions);
        if (opts.dump_asm) {
            dump_asm(instructions);
        }
        
        std::puts("Done.");

    } catch (const ParseException& e) {
        std::fprintf(stderr, "Caught Parse Exception: %s\n", e.what());
    } catch (const std::exception& e) {
        std::fprintf(stderr, "Caught generic exception: %s\n", e.what());
    } catch (...) {
        std::fprintf(stderr, "Unknown error! Please retry!\n");
    }
}
