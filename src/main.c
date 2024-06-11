#include <stdio.h>
#include "lex.c"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <source-file>\n", argv[0]);
        return 1;
    }

    // Call the lexical analysis function
    lexicalAnalysis(argv[1]);

    return 0;
}