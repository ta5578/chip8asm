#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "utils.h"
#include "vm_def.h"
#include "asmparser.h"
#include "bingenerator.h"

int main(int argc, char **argv)
{
    AsmOpts opts;
    init_opts(&opts);

    if (!parse_args(argc, argv, &opts)) {
        show_help();
        return EXIT_FAILURE;
    }

    if (opts.show_help) {
        show_help();
        return EXIT_SUCCESS;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        fprintf(stderr, "ERROR! Could not open file '%s'. Ensure read permissions are set.\n", argv[1]);
        goto error;
    }

    const char *buf = NULL;
    size_t buf_size = read_file_into_mem(file, &buf);
    if (buf_size == 0) {
        fprintf(stderr, "ERROR! Unable to read file '%s' into memory. \n", argv[1]);
        goto error;
    }
    fclose(file); /* The file is no longer necessary after being in memory. */
    
    AsmParser parser;
    if (!init_parser(&parser, buf)) {
        fprintf(stderr, "ERROR! Couldn't initialize the parser properly!\n");
        goto error;
    }

    BinGenerator gen;
    init_generator(&gen, &parser);
    if (!generate_bin(&gen, &opts)) {
        fprintf(stderr, "ERROR! Unable to generate final binary '%s'.\n", opts.out_file);
        goto error;
    }

    free((void*)buf);
    return EXIT_SUCCESS;

error:
    if (file) {
        fclose(file);
    }
    if (buf) {
        free((void*)buf);
    }
    return EXIT_FAILURE;
}
