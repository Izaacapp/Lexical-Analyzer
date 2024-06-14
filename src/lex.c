#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "lex.h"

const char* reserved_words[] = {
    "const", "var", "procedure", "call", "begin", "end", "if", "fi", "then", 
    "else", "while", "do", "read", "write"
};

const token_type reserved_word_tokens[] = {
    constsym, varsym, procsym, callsym, beginsym, endsym, ifsym, fisym, thensym, 
    elsesym, whilesym, dosym, readsym, writesym
};

char* read_input(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error opening file %s\n", filename);
        exit(1);
    }

    char* buffer = malloc(MAX_INPUT_SIZE);
    if (!buffer) {
        fprintf(stderr, "Memory allocation error\n");
        exit(1);
    }

    size_t bytes_read = fread(buffer, 1, MAX_INPUT_SIZE, file);
    buffer[bytes_read] = '\0';

    fclose(file);
    return buffer;
}

int is_letter(char c) {
    return isalpha(c);
}

int is_digit(char c) {
    return isdigit(c);
}

token_type get_reserved_word_token(const char* lexeme) {
    for (size_t i = 0; i < sizeof(reserved_words) / sizeof(reserved_words[0]); i++) {
        if (strcmp(lexeme, reserved_words[i]) == 0) {
            return reserved_word_tokens[i];
        }
    }
    return identsym;
}

void add_token(Lexeme lexemes[], Token tokens[], int* lexeme_count, int* token_count, const char* lexeme, token_type type, int error) {
    strcpy(lexemes[*lexeme_count].lexeme, lexeme);
    lexemes[*lexeme_count].type = type;
    lexemes[*lexeme_count].error = error;
    (*lexeme_count)++;
    
    if (!error) {
        tokens[*token_count].type = type;
        strcpy(tokens[*token_count].value, lexeme);
        (*token_count)++;
    }
}

void handle_improper_comment(const char* source, int start, int end, Lexeme lexemes[], Token tokens[], int* lexeme_count, int* token_count) {
    add_token(lexemes, tokens, lexeme_count, token_count, "/", slashsym, 0);
    add_token(lexemes, tokens, lexeme_count, token_count, "*", multsym, 0);
    char buffer[256];
    int idx = 0;
    for (int j = start + 2; j < end; j++) {
        if (isspace(source[j])) {
            if (idx > 0) {
                buffer[idx] = '\0';
                add_token(lexemes, tokens, lexeme_count, token_count, buffer, identsym, 0);
                idx = 0;
            }
        } else {
            buffer[idx++] = source[j];
        }
    }
    if (idx > 0) {
        buffer[idx] = '\0';
        add_token(lexemes, tokens, lexeme_count, token_count, buffer, identsym, 0);
    }
}

void lexical_analysis(const char* source, Lexeme lexemes[], Token tokens[], int* lexeme_count, int* token_count) {
    int i = 0;
    int length = strlen(source);

    while (i < length && *lexeme_count < MAX_LEXEMES && *token_count < MAX_TOKENS) {
        if (isspace(source[i])) {
            i++;
        } else if (source[i] == '/' && source[i + 1] == '*') {
            // Handle /* comments
            int start = i;
            i += 2;
            int comment_closed = 0;
            while (i < length) {
                if (source[i] == '*' && source[i + 1] == '/') {
                    comment_closed = 1;
                    i += 2;
                    break;
                }
                i++;
            }
            if (!comment_closed) {
                // Handle improperly enclosed comment
                handle_improper_comment(source, start, i, lexemes, tokens, lexeme_count, token_count);
                continue;
            }
        } else if (source[i] == '/' && source[i + 1] == ' ') {
            // Handle / * not a comment */
            add_token(lexemes, tokens, lexeme_count, token_count, "/", slashsym, 0);
            i++;
        } else if (is_letter(source[i])) {
            int start = i;
            while (is_letter(source[i]) || is_digit(source[i])) i++;
            int len = i - start;
            char buffer[len + 1];
            strncpy(buffer, &source[start], len);
            buffer[len] = '\0';
            token_type type = get_reserved_word_token(buffer);
            add_token(lexemes, tokens, lexeme_count, token_count, buffer, type, len > MAX_LEXEME_LENGTH);
        } else if (is_digit(source[i])) {
            int start = i;
            while (is_digit(source[i])) i++;
            int len = i - start;
            char buffer[len + 1];
            strncpy(buffer, &source[start], len);
            buffer[len] = '\0';
            add_token(lexemes, tokens, lexeme_count, token_count, buffer, numbersym, len > MAX_NUMBER_LENGTH);
        } else {
            char buffer[3] = {0};
            switch (source[i]) {
                case '+': buffer[0] = '+'; add_token(lexemes, tokens, lexeme_count, token_count, buffer, plussym, 0); break;
                case '-': buffer[0] = '-'; add_token(lexemes, tokens, lexeme_count, token_count, buffer, minussym, 0); break;
                case '*': buffer[0] = '*'; add_token(lexemes, tokens, lexeme_count, token_count, buffer, multsym, 0); break;
                case '/': buffer[0] = '/'; add_token(lexemes, tokens, lexeme_count, token_count, buffer, slashsym, 0); break;
                case '(': buffer[0] = '('; add_token(lexemes, tokens, lexeme_count, token_count, buffer, lparentsym, 0); break;
                case ')': buffer[0] = ')'; add_token(lexemes, tokens, lexeme_count, token_count, buffer, rparentsym, 0); break;
                case '=': buffer[0] = '='; add_token(lexemes, tokens, lexeme_count, token_count, buffer, eqlsym, 0); break;
                case '<':
                    if (source[i + 1] == '>') {
                        buffer[0] = '<'; buffer[1] = '>'; buffer[2] = '\0'; add_token(lexemes, tokens, lexeme_count, token_count, buffer, neqsym, 0); i++;
                    } else if (source[i + 1] == '=') {
                        buffer[0] = '<'; buffer[1] = '='; buffer[2] = '\0'; add_token(lexemes, tokens, lexeme_count, token_count, buffer, leqsym, 0); i++;
                    } else {
                        buffer[0] = '<'; add_token(lexemes, tokens, lexeme_count, token_count, buffer, lessym, 0);
                    }
                    break;
                case '>':
                    if (source[i + 1] == '=') {
                        buffer[0] = '>'; buffer[1] = '='; buffer[2] = '\0'; add_token(lexemes, tokens, lexeme_count, token_count, buffer, geqsym, 0); i++;
                    } else {
                        buffer[0] = '>'; add_token(lexemes, tokens, lexeme_count, token_count, buffer, gtrsym, 0);
                    }
                    break;
                case ';': buffer[0] = ';'; add_token(lexemes, tokens, lexeme_count, token_count, buffer, semicolonsym, 0); break;
                case '.': buffer[0] = '.'; add_token(lexemes, tokens, lexeme_count, token_count, buffer, periodsym, 0); break;
                case ',': buffer[0] = ','; add_token(lexemes, tokens, lexeme_count, token_count, buffer, commasym, 0); break;
                case ':':
                    if (source[i + 1] == '=') {
                        buffer[0] = ':'; buffer[1] = '='; buffer[2] = '\0'; add_token(lexemes, tokens, lexeme_count, token_count, buffer, becomessym, 0); i++;
                    } else {
                        buffer[0] = ':'; add_token(lexemes, tokens, lexeme_count, token_count, buffer, skipsym, 1);
                    }
                    break;
                default:
                    buffer[0] = source[i]; add_token(lexemes, tokens, lexeme_count, token_count, buffer, skipsym, 1);
            }
            i++;
        }
    }
}
