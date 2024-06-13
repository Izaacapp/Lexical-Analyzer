#include <stdio.h>
#include <stdlib.h>
#include "token.h"

FILE *input_file;
FILE *output_file;
token_list *tokens; // Declare tokens here

void print_to_both(const char *format, ...) {
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    va_start(args, format);
    vfprintf(output_file, format, args);
    va_end(args);
}

void print_source_code() {
    char c;
    char last_char = 0;
    while ((c = fgetc(input_file)) != EOF) {
        print_to_both("%c", c);
        last_char = c;
    }
    if (last_char != '\n')
        print_to_both("\n");
    rewind(input_file);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <input file> <output file>\n", argv[0]);
        return 1;
    }

    input_file = fopen(argv[1], "r");
    output_file = fopen(argv[2], "w");

    if (input_file == NULL) {
        printf("Error: Could not open input file %s\n", argv[1]);
        return 1;
    }

    if (output_file == NULL) {
        printf("Error: Could not open output file %s\n", argv[2]);
        return 1;
    }

    print_to_both("Source Program:\n");
    print_source_code();
    print_to_both("\nLexeme Table:\n\n%10s %20s\n", "lexeme", "token type");

    tokens = create_token_list();
    tokenize();

    print_to_both("\nToken List:\n");
    print_token_list(tokens);
    destroy_token_list(tokens);

    fclose(input_file);
    fclose(output_file);

    return 0;
}
