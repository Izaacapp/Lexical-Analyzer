/*
    COP 3402 Systems Software
    Lexical Analyzer
    Authored by Izaac Plambeck
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>
#include <regex.h>
#include "token.h"

#define MAX_IDENTIFIER_LENGTH 11
#define MAX_NUMBER_LENGTH 5
#define MAX_BUFFER_LENGTH 1000

FILE *input_file;
FILE *output_file;

// Peek at the next character from the input file without consuming it
char peekc() {
    int c = getc(input_file);
    ungetc(c, input_file);
    return (char)c;
}

// Print formatted output to both the console and the output file
void print_both(const char *format, ...) {
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    va_start(args, format);
    vfprintf(output_file, format, args);
    va_end(args);
}

// Print the entire source code from the input file to both the console and the output file
void print_source_code() {
    char c;
    char lastChar = 0; // To keep track of the last character printed
    while ((c = fgetc(input_file)) != EOF) {
        print_both("%c", c);
        lastChar = c;
    }
    if (lastChar != '\n') // If the last character wasn't a newline, print one
        print_both("\n");
    rewind(input_file); // Reset file pointer to the beginning of the file
}

// Test regex pattern matching
void test_regex() {
    regex_t regex;
    int reti;

    // Compile regular expression
    reti = regcomp(&regex, "^[a-zA-Z_][a-zA-Z0-9_]*$", REG_EXTENDED);
    if (reti) {
        fprintf(stderr, "Could not compile regex\n");
        exit(1);
    }

    // Execute regular expression
    reti = regexec(&regex, "test_identifier", 0, NULL, 0);
    if (!reti) {
        puts("Match");
    } else if (reti == REG_NOMATCH) {
        puts("No match");
    } else {
        char msgbuf[100];
        regerror(reti, &regex, msgbuf, sizeof(msgbuf));
        fprintf(stderr, "Regex match failed: %s\n", msgbuf);
        exit(1);
    }

    // Free compiled regular expression if you want to use the regex_t again
    regfree(&regex);
}

void lexicalAnalysis(const char *input_filename, const char *output_filename) {
    input_file = fopen(input_filename, "r");
    output_file = fopen(output_filename, "w");

    if (input_file == NULL) {
        printf("Error: Could not open input file %s\n", input_filename);
        return;
    }

    if (output_file == NULL) {
        printf("Error: Could not open output file %s\n", output_filename);
        return;
    }

    print_both("Source Program:\n");
    print_source_code();
    print_both("\n");

    // Test regex pattern matching
    test_regex();

    fclose(input_file);
    fclose(output_file);
}