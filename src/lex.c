#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX_IDENT_LEN 11
#define MAX_NUM_LEN 5
#define MAX_LEXEMES 1000

typedef enum {
    skipsym = 1, identsym, numbersym, plussym, minussym,
    multsym, slashsym, fisym, eqlsym, neqsym, lessym, leqsym,
    gtrsym, geqsym, lparentsym, rparentsym, commasym, semicolonsym,
    periodsym, becomessym, beginsym, endsym, ifsym, thensym,
    whilesym, dosym, callsym, constsym, varsym, procsym, writesym,
    readsym, elsesym, errorsym
} token_type;

typedef struct {
    char lexeme[MAX_IDENT_LEN + 1];
    token_type token;
    char error[50];
} Lexeme;

Lexeme lexemes[MAX_LEXEMES];
int lexeme_count = 0;

char *reservedWords[] = {
    "const", "var", "procedure", "call", "begin", "end", "if", "fi",
    "then", "else", "while", "do", "read", "write"
};

token_type reservedTokens[] = {
    constsym, varsym, procsym, callsym, beginsym, endsym, ifsym, fisym,
    thensym, elsesym, whilesym, dosym, readsym, writesym
};

void addLexeme(char *lex, token_type token, char *error) {
    if (lexeme_count < MAX_LEXEMES) {
        strcpy(lexemes[lexeme_count].lexeme, lex);
        lexemes[lexeme_count].token = token;
        if (error) {
            strcpy(lexemes[lexeme_count].error, error);
        } else {
            lexemes[lexeme_count].error[0] = '\0';
        }
        lexeme_count++;
    }
}

token_type identifyReservedWord(char *word) {
    for (int i = 0; i < sizeof(reservedWords) / sizeof(reservedWords[0]); i++) {
        if (strcmp(word, reservedWords[i]) == 0) {
            return reservedTokens[i];
        }
    }
    return identsym;
}

void printLexemeTable() {
    printf("Lexeme Table:\n\n");
    printf("%-20s%-10s\n", "lexeme", "token type");
    for (int i = 0; i < lexeme_count; i++) {
        printf("%-20s%-10d", lexemes[i].lexeme, lexemes[i].token);
        if (lexemes[i].error[0] != '\0') {
            printf(" Error: %s", lexemes[i].error);
        }
        printf("\n");
    }
}

void printTokenList() {
    printf("Token List:\n");
    for (int i = 0; i < lexeme_count; i++) {
        printf("%d", lexemes[i].token);
        if (lexemes[i].token == identsym || lexemes[i].token == numbersym) {
            printf(" %s", lexemes[i].lexeme);
        }
        if (i < lexeme_count - 1) {
            printf(" ");
        }
    }
    printf("\n");
}

void processInput(const char *input) {
    int length = strlen(input);
    char buffer[MAX_IDENT_LEN + 1];
    int buffer_index = 0;

    for (int i = 0; i < length; i++) {
        char c = input[i];

        if (isspace(c)) continue;  // Ignore whitespace

        if (isalpha(c)) {  // Identifiers or reserved words
            buffer_index = 0;
            while (isalnum(c) && buffer_index < MAX_IDENT_LEN) {
                buffer[buffer_index++] = c;
                c = input[++i];
            }
            buffer[buffer_index] = '\0';
            if (isalnum(c)) {  // Name too long
                while (isalnum(c)) c = input[++i];
                addLexeme(buffer, errorsym, "Name too long");
            } else {
                token_type token = identifyReservedWord(buffer);
                addLexeme(buffer, token, NULL);
            }
            i--;  // Adjust for extra increment
        } else if (isdigit(c)) {  // Numbers
            buffer_index = 0;
            while (isdigit(c) && buffer_index < MAX_NUM_LEN) {
                buffer[buffer_index++] = c;
                c = input[++i];
            }
            buffer[buffer_index] = '\0';
            if (isdigit(c)) {  // Number too long
                while (isdigit(c)) c = input[++i];
                addLexeme(buffer, errorsym, "Number too long");
            } else {
                addLexeme(buffer, numbersym, NULL);
            }
            i--;  // Adjust for extra increment
        } else {  // Special symbols and errors
            buffer[0] = c;
            buffer[1] = '\0';
            switch (c) {
                case '+': addLexeme(buffer, plussym, NULL); break;
                case '-': addLexeme(buffer, minussym, NULL); break;
                case '*': addLexeme(buffer, multsym, NULL); break;
                case '/':
                    if (input[i + 1] == '*') {  // Start of comment
                        i += 2;  // Skip '/*'
                        while (i < length - 1 && !(input[i] == '*' && input[i + 1] == '/')) {
                            i++;
                        }
                        if (i < length - 1) {
                            i++;  // Skip '*/'
                        } else {
                            addLexeme("/*", errorsym, "Unterminated comment");
                        }
                    } else {
                        addLexeme(buffer, slashsym, NULL);
                    }
                    break;
                case '=': addLexeme(buffer, eqlsym, NULL); break;
                case '<':
                    if (input[i + 1] == '>') {
                        buffer[1] = '>';
                        buffer[2] = '\0';
                        addLexeme(buffer, neqsym, NULL);
                        i++;
                    } else if (input[i + 1] == '=') {
                        buffer[1] = '=';
                        buffer[2] = '\0';
                        addLexeme(buffer, leqsym, NULL);
                        i++;
                    } else {
                        addLexeme(buffer, lessym, NULL);
                    }
                    break;
                case '>':
                    if (input[i + 1] == '=') {
                        buffer[1] = '=';
                        buffer[2] = '\0';
                        addLexeme(buffer, geqsym, NULL);
                        i++;
                    } else {
                        addLexeme(buffer, gtrsym, NULL);
                    }
                    break;
                case ':':
                    if (input[i + 1] == '=') {
                        buffer[1] = '=';
                        buffer[2] = '\0';
                        addLexeme(buffer, becomessym, NULL);
                        i++;
                    } else {
                        addLexeme(buffer, errorsym, "Invalid character");
                    }
                    break;
                case ';': addLexeme(buffer, semicolonsym, NULL); break;
                case ',': addLexeme(buffer, commasym, NULL); break;
                case '.': addLexeme(buffer, periodsym, NULL); break;
                case '(': addLexeme(buffer, lparentsym, NULL); break;
                case ')': addLexeme(buffer, rparentsym, NULL); break;
                default:
                    addLexeme(buffer, errorsym, "Invalid character");
                    break;
            }
        }
    }
}

void readInputFile(const char *filename, char *buffer, int size) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    int index = 0;
    char c;
    while ((c = fgetc(file)) != EOF && index < size - 1) {
        buffer[index++] = c;
    }
    buffer[index] = '\0';
    fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const int bufferSize = 10000;
    char input[bufferSize];

    readInputFile(argv[1], input, bufferSize);

    printf("Source Program:\n%s\n", input);

    processInput(input);
    printLexemeTable();
    printTokenList();

    return 0;
}
