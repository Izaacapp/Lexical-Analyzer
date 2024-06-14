#ifndef LEX_H
#define LEX_H

#define MAX_INPUT_SIZE 1024
#define MAX_LEXEME_LENGTH 11
#define MAX_NUMBER_LENGTH 5
#define MAX_LEXEMES 1000
#define MAX_TOKENS 1000

typedef enum {
    skipsym = 1, identsym, numbersym, plussym, minussym, multsym, slashsym,
    fisym, eqlsym, neqsym, lessym, leqsym, gtrsym, geqsym, lparentsym,
    rparentsym, commasym, semicolonsym, periodsym, becomessym, beginsym,
    endsym, ifsym, thensym, whilesym, dosym, callsym, constsym, varsym,
    procsym, writesym, readsym, elsesym
} token_type;

typedef struct {
    char lexeme[MAX_LEXEME_LENGTH + 1];
    token_type type;
    int error;
} Lexeme;

typedef struct {
    token_type type;
    char value[MAX_LEXEME_LENGTH + 1]; // Holds either the identifier name or the number as a string
} Token;

char* read_input(const char* filename);
void lexical_analysis(const char* source, Lexeme lexemes[], Token tokens[], int* lexeme_count, int* token_count);

#endif
