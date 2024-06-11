#include "token.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to read the source program
void readSourceProgram(const char *filename) {
    // TODO: Implement reading from file
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    // Read file content here...
    fclose(file);
}

// Function to perform lexical analysis
void lexicalAnalysis() {
    // TODO: Implement lexical analysis logic
    // 1. Read source program character by character
    // 2. Identify tokens based on PL/0 grammar
    // 3. Handle errors: number too long, name too long, invalid symbols
    // 4. Generate lexeme table and token list
}

// Main function
int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <source-file>\n", argv[0]);
        return 1;
    }
    readSourceProgram(argv[1]);
    lexicalAnalysis();
    return 0;
}