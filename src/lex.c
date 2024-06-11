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

#define MAX_BUFFER_LENGTH 1000
#define INITIAL_CAPACITY 10

FILE *input_file;
FILE *output_file;

typedef struct {
    Token *tokens;
    int size;
    int capacity;
} TokenList;

TokenList *create_token_list() {
    TokenList *list = (TokenList *)malloc(sizeof(TokenList));
    list->size = 0;
    list->capacity = INITIAL_CAPACITY;
    list->tokens = (Token *)malloc(sizeof(Token) * list->capacity);
    return list;
}

void add_token(TokenList *list, Token token) {
    if (list->size >= list->capacity) {
        list->capacity *= 2;
        list->tokens = (Token *)realloc(list->tokens, sizeof(Token) * list->capacity);
    }
    list->tokens[list->size++] = token;
}

void free_token_list(TokenList *list) {
    free(list->tokens);
    free(list);
}

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

    TokenList *token_list = create_token_list();
    Token token;
    char c;

    // Sample tokenization logic (this will be expanded in the next steps)
    while ((c = fgetc(input_file)) != EOF) {
        if (isspace(c)) {
            continue;
        } else if (isalpha(c)) {
            // Handle identifiers and reserved words
            int i = 0;
            while (isalnum(c) && i < MAX_IDENTIFIER_LENGTH) {
                token.lexeme[i++] = c;
                c = fgetc(input_file);
            }
            ungetc(c, input_file);
            token.lexeme[i] = '\0';
            token.type = identsym; // This is just a placeholder
            add_token(token_list, token);
        } else if (isdigit(c)) {
            // Handle numbers
            int i = 0;
            while (isdigit(c) && i < MAX_NUMBER_LENGTH) {
                token.lexeme[i++] = c;
                c = fgetc(input_file);
            }
            ungetc(c, input_file);
            token.lexeme[i] = '\0';
            token.type = numbersym; // This is just a placeholder
            add_token(token_list, token);
        } else {
            // Handle special symbols
            token.lexeme[0] = c;
            token.lexeme[1] = '\0';
            token.type = skipsym; // This is just a placeholder
            add_token(token_list, token);
        }
    }

    // Print tokens for debugging
    for (int i = 0; i < token_list->size; i++) {
        print_both("Token: %s, Type: %d\n", token_list->tokens[i].lexeme, token_list->tokens[i].type);
    }

    free_token_list(token_list);

    fclose(input_file);
    fclose(output_file);
}