#include <cstdlib>
#include <string>
#include <iostream>
#include "utils.h"
#include "Lexer.h"
#include "Parser.h"
#include <exception>
#include "ParseException.h"
#include "opcodes.h"

static uint16_t generate_opcode(const std::string& op, const std::vector<std::string>& args)
{
    auto itr = OPERATORS.find(op);
    return itr->second(args);
}

static void write_rom(const std::string& filePath, const std::vector<Instruction>& instructions, const std::map<std::string, uint16_t>& labels)
{
    std::FILE *fp = std::fopen(filePath.c_str(), "wb");
    if (!fp) {
        throw std::runtime_error("Unable to open file for writing.");
    }

    /* Now perform the binary conversion and write it to the file */
    for (const auto& i : instructions) {
        uint16_t op = 0;
        /* Replace labels with their addresses */
        if (one_of<std::string>(i.op, { "JMP", "CALL", "ZJMP", "ILOAD" })) {
            auto label = i.args[0];
            auto arg = from_hex(labels.at(label));
            op = generate_opcode(i.op, { arg });
        } else {
            op = generate_opcode(i.op, i.args);
        }
        /* Correct for the host machine endianness to chip 8 big endian */
        op = endi(op);

        std::fwrite(&op, sizeof(op), 1, fp);
    }
    std::fclose(fp);

#ifndef NDEBUG
    LOG("-------- Label Addresses --------");
    for (const auto& it : labels) {
        LOG("%s -> 0x%04X", it.first.c_str(), it.second);
    }
    LOG("-------- End Addresses --------");
#endif
}

static void dump_asm(const std::vector<Instruction>& instructions, const std::map<std::string, uint16_t>& labels)
{
    std::cout << "-------- Asm Dump --------\n";
    for (const auto& i : instructions) {
        uint16_t op = 0;
        /* Replace labels with their addresses */
        if (one_of<std::string>(i.op, { "JMP", "CALL", "ZJMP", "ILOAD" })) {
            auto label = i.args[0];
            auto arg = from_hex(labels.at(label));
            op = generate_opcode(i.op, { arg });
        } else {
            op = generate_opcode(i.op, i.args);
        }
        std::cout << from_hex(i.addr) << " -- " << from_hex(op) << " ; ";
        std::string s = i.op + " ";
        for (const auto& a : i.args) {
            s += a;
            s += ", ";
        }
        /* Remove the last comma */
        s = s.substr(0, s.find_last_of(","));
        std::cout << s << "\n";
    }
    std::cout << "-------- End Dump --------\n";
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

        std::cout << "Reading from '" << opts.in_file << "' and writing to '" << opts.out_file << "'.\n";
        std::cout << "Dump ASM: " << (opts.dump_asm ? "true" : "false") << "\n";

        const std::string text = read_file(argv[1]);
        if (text.empty()) {
            std::cerr << "Error reading from " << argv[1] << "!\n";
            return EXIT_FAILURE;
        }

        emu::Lexer lexer(text);
        c8::Parser parser(lexer);
        parser.parse();

        auto instructions = parser.getInstructions();
        auto labels = parser.getLabels();
        write_rom(opts.out_file, instructions, labels);
        if (opts.dump_asm) {
            dump_asm(instructions, labels);
        }
        
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
