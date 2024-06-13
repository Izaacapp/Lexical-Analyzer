#ifndef TOKEN_H
#define TOKEN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

#define MAX_IDENTIFIER_LENGTH 11
#define MAX_NUMBER_LENGTH 5
#define MAX_BUFFER_LENGTH 1000

typedef enum {
    skipsym = 1, identsym, numbersym, plussym, minussym,
    multsym, slashsym, fisym, eqsym, neqsym, lessym, leqsym,
    gtrsym, geqsym, lparentsym, rparentsym, commasym, semicolonsym,
    periodsym, becomessym, beginsym, endsym, ifsym, thensym,
    whilesym, dosym, callsym, constsym, varsym, procsym, writesym,
    readsym, elsesym
} token_type;

typedef struct {
    char lexeme[MAX_BUFFER_LENGTH + 1];
    char value[MAX_BUFFER_LENGTH + 1];
} token;

typedef struct {
    token *tokens;
    int size;
    int capacity;
} token_list;

char peek_char();
void clear_buffer(char *buffer, int length);
int is_reserved_word(char *buffer);
int get_special_symbol_token(char *buffer);
int is_special_character(char c);
token_list *create_token_list();
token_list *destroy_token_list(token_list *list);
token_list *append_token(token_list *list, token tok);
void print_lexeme_table(token_list *list);
void print_token_list(token_list *list);
void tokenize();

#endif // TOKEN_H
