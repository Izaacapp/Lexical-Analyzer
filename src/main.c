#include <stdio.h>
#include "token.h"

void lexicalAnalysis(const char *input_file, const char *output_file);

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <input file> <output file>\n", argv[0]);
        return 1;
    }

    // Call the lexical analysis function
    lexicalAnalysis(argv[1], argv[2]);

    return 0;
}