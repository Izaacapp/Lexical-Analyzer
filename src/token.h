#ifndef TOKEN_H
#define TOKEN_H

#define MAX_IDENTIFIER_LENGTH 11
#define MAX_NUMBER_LENGTH 5

typedef enum {
    skipsym = 1, identsym, numbersym, plussym, minussym,
    multsym, slashsym, eqlsym, neqsym, lessym, leqsym,
    gtrsym, geqsym, lparentsym, rparentsym, commasym, semicolonsym,
    periodsym, becomessym, beginsym, endsym, ifsym, thensym,
    whilesym, dosym, callsym, constsym, varsym, procsym, writesym,
    readsym, elsesym
} token_type;

// Structure to hold token information
typedef struct {
    token_type type;
    char lexeme[MAX_IDENTIFIER_LENGTH + 1]; // PL/0 identifiers have a max length of 11 characters
} Token;

#endif