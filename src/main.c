#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lex.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <source file>\n", argv[0]);
        return 1;
    }

    char* source = read_input(argv[1]);
    if (source == NULL) {
        fprintf(stderr, "Error: Unable to read source file\n");
        return 1;
    }
    
    Lexeme lexemes[MAX_LEXEMES];
    Token tokens[MAX_TOKENS];
    int lexeme_count = 0, token_count = 0;

    memset(lexemes, 0, sizeof(lexemes));
    memset(tokens, 0, sizeof(tokens));

    printf("Source Program:\n%s\n", source);

    lexical_analysis(source, lexemes, tokens, &lexeme_count, &token_count);

    printf("\nLexeme Table:\n\n");
    printf("lexeme  token type\n");
    for (int i = 0; i < lexeme_count; i++) {
        if (lexemes[i].error) {
            printf("%-12s Error: %s\n", lexemes[i].lexeme, lexemes[i].type == identsym ? "name too long" : lexemes[i].type == numbersym ? "number too long" : "invalid character");
        } else {
            printf("%-12s %d\n", lexemes[i].lexeme, lexemes[i].type);
        }
    }

    printf("\nToken List:\n");
    for (int i = 0; i < token_count; i++) {
        if (tokens[i].type == identsym || tokens[i].type == numbersym) {
            printf("%d %s ", tokens[i].type, tokens[i].value);
        } else {
            printf("%d ", tokens[i].type);
        }
    }
    printf("\n");

    free(source);
    return 0;
}
