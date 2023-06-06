#include "bmp.h"
#include "io.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int ch = 0;
    char infile[100];
    char outfile[100];
    bool out = false;
    bool in = false;

    while ((ch = getopt(argc, argv, "o:i:h")) != -1) {
        switch (ch) {
        case 'i': {
            strcpy(infile, optarg);
            in = true;
            break;
        }
        case 'o': {
            strcpy(outfile, optarg);
            out = true;
            break;
        }
        case 'h': {
            fprintf(stdout,
                "-i: Following the '-i' the user chooses which input file to be analyzed (requires "
                "a filename for the following argument)\n-o: Following the '-o' the user creates "
                "or chooses a file for the output to be stored in.\n-d: All graphs are initialized "
                "as directed -> Enables directed graph.\n-h: help statement\n");
            return 0;
        }
        default: {
            fprintf(stderr,
                "-i: Following the '-i' the user chooses which input file to be analyzed (requires "
                "a filename for the following argument)\n-o: Following the '-o' the user creates "
                "or chooses a file for the output to be stored in.\n-d: All graphs are initialized "
                "as directed -> Enables directed graph.\n-h: help statement\n");
            return 0;
        }
        }
    }

    const char *inputFilename = infile;
    const char *outputFilename = outfile;

    Buffer *buffer = read_open(inputFilename);
    if (buffer == NULL || (strstr(outfile, ".bmp") != NULL)) {
        fprintf(stderr,
            "-i: Following the '-i' the user chooses which input file to be analyzed (requires "
            "a filename for the following argument)\n-o: Following the '-o' the user creates "
            "or chooses a file for the output to be stored in.\n-d: All graphs are initialized "
            "as directed -> Enables directed graph.\n-h: help statement\n");
        return 1;
    }

    if (!in) {
        fprintf(stderr, "Input file was not given!\n");
        return 1;
    }
    if (!out) {
        fprintf(stderr, "Output file was not given!\n");
        return 1;
    }

    BMP *bmp = bmp_create(buffer);
    read_close(&buffer);

    if (bmp == NULL) {
        fprintf(stderr, "Failed to create BMP\n");
        return 1;
    }

    bmp_reduce_palette(bmp);

    buffer = write_open(outputFilename);
    if (buffer == NULL) {
        fprintf(stderr, "Failed to open output file\n");
        bmp_free(&bmp);
        return 1;
    }

    bmp_write(bmp, buffer);
    write_close(&buffer);
    bmp_free(&bmp);

    return 0;
}
