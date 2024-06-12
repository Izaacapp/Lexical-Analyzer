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

int handle_reserved_word(char *buffer) {
    if (strcmp(buffer, "const") == 0)
        return constsym;
    else if (strcmp(buffer, "var") == 0)
        return varsym;
    else if (strcmp(buffer, "procedure") == 0)
        return procsym;
    else if (strcmp(buffer, "call") == 0)
        return callsym;
    else if (strcmp(buffer, "begin") == 0)
        return beginsym;
    else if (strcmp(buffer, "end") == 0)
        return endsym;
    else if (strcmp(buffer, "if") == 0)
        return ifsym;
    else if (strcmp(buffer, "then") == 0)
        return thensym;
    else if (strcmp(buffer, "else") == 0)
        return elsesym;
    else if (strcmp(buffer, "while") == 0)
        return whilesym;
    else if (strcmp(buffer, "do") == 0)
        return dosym;
    else if (strcmp(buffer, "read") == 0)
        return readsym;
    else if (strcmp(buffer, "write") == 0)
        return writesym;
    return 0; // invalid reserved word
}

int handle_special_symbol(char *buffer) {
    if (strcmp(buffer, "+") == 0)
        return plussym;
    else if (strcmp(buffer, "-") == 0)
        return minussym;
    else if (strcmp(buffer, "*") == 0)
        return multsym;
    else if (strcmp(buffer, "/") == 0)
        return slashsym;
    else if (strcmp(buffer, "(") == 0)
        return lparentsym;
    else if (strcmp(buffer, ")") == 0)
        return rparentsym;
    else if (strcmp(buffer, ",") == 0)
        return commasym;
    else if (strcmp(buffer, ";") == 0)
        return semicolonsym;
    else if (strcmp(buffer, ".") == 0)
        return periodsym;
    else if (strcmp(buffer, "=") == 0)
        return eqlsym;
    else if (strcmp(buffer, "<") == 0)
        return lessym;
    else if (strcmp(buffer, ">") == 0)
        return gtrsym;
    else if (strcmp(buffer, ":=") == 0)
        return becomessym;
    else if (strcmp(buffer, "<=") == 0)
        return leqsym;
    else if (strcmp(buffer, ">=") == 0)
        return geqsym;
    else if (strcmp(buffer, "<>") == 0)
        return neqsym;
    return 0; // invalid special symbol
}

// Check if a given character is a special symbol
int is_special_symbol(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')' ||
            c == '=' || c == ',' || c == '.' || c == '<' || c == '>' || c == ':' || c == ';');
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
    char buffer[MAX_BUFFER_LENGTH + 1] = {0};
    int buffer_index = 0;

    while ((c = fgetc(input_file)) != EOF) {
        if (isspace(c)) {
            continue;
        } else if (isalpha(c)) {
            buffer[buffer_index++] = c;
            while (1) {
                char nextc = peekc();
                if (isspace(nextc) || is_special_symbol(nextc) || nextc == EOF) {
                    int token_value = handle_reserved_word(buffer);
                    if (token_value) {
                        token.type = token_value;
                        strcpy(token.lexeme, buffer);
                    } else {
                        if (buffer_index > MAX_IDENTIFIER_LENGTH) {
                            print_both("%10s %20s\n", buffer, "ERROR: IDENTIFIER TOO LONG");
                            token.type = skipsym;
                        } else {
                            token.type = identsym;
                            strcpy(token.lexeme, buffer);
                        }
                    }
                    add_token(token_list, token);
                    buffer_index = 0;
                    memset(buffer, 0, sizeof(buffer));
                    break;
                } else if (isalnum(nextc)) {
                    c = getc(input_file);
                    buffer[buffer_index++] = c;
                }
            }
        } else if (isdigit(c)) {
            buffer[buffer_index++] = c;
            while (1) {
                char nextc = peekc();
                if (isspace(nextc) || is_special_symbol(nextc) || nextc == EOF) {
                    if (buffer_index > MAX_NUMBER_LENGTH) {
                        print_both("%10s %20s\n", buffer, "ERROR: NUMBER TOO LONG");
                        token.type = skipsym;
                    } else {
                        token.type = numbersym;
                        strcpy(token.lexeme, buffer);
                    }
                    add_token(token_list, token);
                    buffer_index = 0;
                    memset(buffer, 0, sizeof(buffer));
                    break;
                } else if (isdigit(nextc)) {
                    c = getc(input_file);
                    buffer[buffer_index++] = c;
                }
            }
        } else if (is_special_symbol(c)) {
            buffer[buffer_index++] = c;
            char nextc = peekc();

            if (is_special_symbol(nextc)) {
                c = getc(input_file);
                buffer[buffer_index++] = c;
                int token_value = handle_special_symbol(buffer);
                if (!token_value) {
                    for (int i = 0; i < buffer_index; i++) {
                        print_both("%10c %20s\n", buffer[i], "ERROR: INVALID SYMBOL");
                        token.type = skipsym;
                        add_token(token_list, token);
                    }
                } else {
                    token.type = token_value;
                    strcpy(token.lexeme, buffer);
                    add_token(token_list, token);
                }
                buffer_index = 0;
                memset(buffer, 0, sizeof(buffer));
            } else {
                int token_value = handle_special_symbol(buffer);
                if (!token_value) {
                    print_both("%10c %20s\n", c, "ERROR: INVALID SYMBOL");
                    token.type = skipsym;
                } else {
                    token.type = token_value;
                    strcpy(token.lexeme, buffer);
                }
                add_token(token_list, token);
                buffer_index = 0; 
                memset(buffer, 0, sizeof(buffer));
            }
        }
    }

   print_both("\nLexeme Table:\n");
for (int i = 0; i < token_list->size; i++) {
    print_both("%10s %20d\n", token_list->tokens[i].lexeme, token_list->tokens[i].type);
}

print_both("\nToken List:\n");
for (int i = 0; i < token_list->size; i++) {
    print_both("%d %s ", token_list->tokens[i].type, token_list->tokens[i].lexeme);
}

free_token_list(token_list);

fclose(input_file);
fclose(output_file);
} 