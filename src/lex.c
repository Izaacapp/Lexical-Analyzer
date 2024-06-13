#include "token.h"

extern FILE *input_file;
extern FILE *output_file;

token_list *tokens;

char peek_char() {
    int c = getc(input_file);
    ungetc(c, input_file);
    return (char)c;
}

void clear_buffer(char *buffer, int length) {
    for (int i = 0; i < length; i++)
        buffer[i] = '\0';
}

int is_reserved_word(char *buffer) {
    if (strcmp(buffer, "const") == 0) return constsym;
    if (strcmp(buffer, "var") == 0) return varsym;
    if (strcmp(buffer, "procedure") == 0) return procsym;
    if (strcmp(buffer, "call") == 0) return callsym;
    if (strcmp(buffer, "begin") == 0) return beginsym;
    if (strcmp(buffer, "end") == 0) return endsym;
    if (strcmp(buffer, "if") == 0) return ifsym;
    if (strcmp(buffer, "fi") == 0) return fisym;
    if (strcmp(buffer, "then") == 0) return thensym;
    if (strcmp(buffer, "else") == 0) return elsesym;
    if (strcmp(buffer, "while") == 0) return whilesym;
    if (strcmp(buffer, "do") == 0) return dosym;
    if (strcmp(buffer, "read") == 0) return readsym;
    if (strcmp(buffer, "write") == 0) return writesym;
    return 0;
}

int get_special_symbol_token(char *buffer) {
    if (strcmp(buffer, "+") == 0) return plussym;
    if (strcmp(buffer, "-") == 0) return minussym;
    if (strcmp(buffer, "*") == 0) return multsym;
    if (strcmp(buffer, "/") == 0) return slashsym;
    if (strcmp(buffer, "(") == 0) return lparentsym;
    if (strcmp(buffer, ")") == 0) return rparentsym;
    if (strcmp(buffer, ",") == 0) return commasym;
    if (strcmp(buffer, ";") == 0) return semicolonsym;
    if (strcmp(buffer, ".") == 0) return periodsym;
    if (strcmp(buffer, "=") == 0) return eqsym;
    if (strcmp(buffer, "<") == 0) return lessym;
    if (strcmp(buffer, ">") == 0) return gtrsym;
    if (strcmp(buffer, ":=") == 0) return becomessym;
    if (strcmp(buffer, "<=") == 0) return leqsym;
    if (strcmp(buffer, ">=") == 0) return geqsym;
    if (strcmp(buffer, "<>") == 0) return neqsym;
    return 0;
}

int is_special_character(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')' || 
            c == '=' || c == ',' || c == '.' || c == '<' || c == '>' || c == ':' || 
            c == ';');
}

token_list *create_token_list() {
    token_list *list = malloc(sizeof(token_list));
    list->size = 0;
    list->capacity = 10;
    list->tokens = malloc(sizeof(token) * list->capacity);
    return list;
}

token_list *destroy_token_list(token_list *list) {
    free(list->tokens);
    free(list);
    return NULL;
}

token_list *append_token(token_list *list, token tok) {
    if (list->size == list->capacity) {
        list->capacity *= 2;
        list->tokens = realloc(list->tokens, sizeof(token) * list->capacity);
    }
    list->tokens[list->size++] = tok;
    return list;
}

void print_lexeme_table(token_list *list) {
    for (int i = 0; i < list->size; i++)
        printf("%10s %20s\n", list->tokens[i].lexeme, list->tokens[i].value);
}

void print_token_list(token_list *list) {
    for (int i = 0; i < list->size; i++) {
        char id_val[3] = {0}, num_val[3] = {0};
        sprintf(id_val, "%d", identsym);
        sprintf(num_val, "%d", numbersym);

        if (strcmp(list->tokens[i].value, id_val) == 0 || strcmp(list->tokens[i].value, num_val) == 0)
            printf("%s %s ", list->tokens[i].value, list->tokens[i].lexeme);
        else
            printf("%s ", list->tokens[i].value);
    }
    printf("\n");  // Ensure newline after the token list
}

void tokenize() {
    char c;
    char buffer[MAX_BUFFER_LENGTH + 1] = {0};
    int buffer_index = 0;

    while ((c = fgetc(input_file)) != EOF) {
        if (iscntrl(c) || isspace(c)) {
            continue;
        }
        if (isdigit(c)) {
            buffer[buffer_index++] = c;
            while (1) {
                char nextc = peek_char();
                if (isspace(nextc) || is_special_character(nextc)) {
                    token tok;
                    if (buffer_index > MAX_NUMBER_LENGTH) {
                        printf("%10s %20s\n", buffer, "Error: number too long");
                    } else {
                        printf("%10s %20d\n", buffer, numbersym);
                        sprintf(tok.value, "%d", numbersym);
                        strcpy(tok.lexeme, buffer);
                        append_token(tokens, tok);
                    }
                    clear_buffer(buffer, buffer_index);
                    buffer_index = 0;
                    break;
                } else if (isdigit(nextc)) {
                    c = getc(input_file);
                    buffer[buffer_index++] = c;
                } else if (nextc == EOF) {
                    break;
                } else if (isalpha(nextc)) {
                    token tok;
                    printf("%10s %20d\n", buffer, numbersym);
                    sprintf(tok.value, "%d", numbersym);
                    strcpy(tok.lexeme, buffer);
                    append_token(tokens, tok);
                    clear_buffer(buffer, buffer_index);
                    buffer_index = 0;
                    break;
                }
            }
        } else if (isalpha(c)) {
            buffer[buffer_index++] = c;
            while (1) {
                char nextc = peek_char();
                if (isspace(nextc) || is_special_character(nextc) || nextc == EOF) {
                    int token_value = is_reserved_word(buffer);
                    if (token_value) {
                        token tok;
                        printf("%10s %20d\n", buffer, token_value);
                        sprintf(tok.value, "%d", token_value);
                        strcpy(tok.lexeme, buffer);
                        append_token(tokens, tok);
                        clear_buffer(buffer, buffer_index);
                        buffer_index = 0;
                        break;
                    } else {
                        token tok;
                        if (buffer_index > MAX_IDENTIFIER_LENGTH) {
                            printf("%10s %20s\n", buffer, "Error: name too long");
                        } else {
                            printf("%10s %20d\n", buffer, identsym);
                            sprintf(tok.value, "%d", identsym);
                            strcpy(tok.lexeme, buffer);
                            append_token(tokens, tok);
                        }
                        clear_buffer(buffer, buffer_index);
                        buffer_index = 0;
                        break;
                    }
                } else if (isalnum(nextc)) {
                    c = getc(input_file);
                    buffer[buffer_index++] = c;
                }
            }
        } else if (is_special_character(c)) {
            buffer[buffer_index++] = c;
            char nextc = peek_char();

            if (is_special_character(nextc)) {
                if (c == '/' && nextc == '*') {
                    clear_buffer(buffer, buffer_index);
                    buffer_index = 0;
                    while (1) {
                        c = getc(input_file);
                        nextc = peek_char();
                        if (c == '*' && nextc == '/') {
                            c = getc(input_file);
                            break;
                        }
                    }
                    continue;
                }

                if (c == '/' && nextc == '/') {
                    clear_buffer(buffer, buffer_index);
                    buffer_index = 0;
                    while (1) {
                        c = getc(input_file);
                        nextc = peek_char();
                        if (c == '\n') {
                            break;
                        }
                    }
                    continue;
                }

                c = getc(input_file);
                buffer[buffer_index++] = c;
                token tok;
                int token_value = get_special_symbol_token(buffer);
                if (!token_value) {
                    for (int i = 0; i < buffer_index; i++)
                        printf("%10c %20s\n", buffer[i], "Error: invalid character");
                } else {
                    printf("%10s %20d\n", buffer, token_value);
                    sprintf(tok.value, "%d", token_value);
                    strcpy(tok.lexeme, buffer);
                    append_token(tokens, tok);
                }
                clear_buffer(buffer, buffer_index);
                buffer_index = 0;
            } else {
                token tok;
                int token_value = get_special_symbol_token(buffer);
                if (!token_value) {
                    printf("%10c %20s\n", c, "Error: invalid character");
                } else {
                    printf("%10s %20d\n", buffer, token_value);
                    sprintf(tok.value, "%d", token_value);
                    strcpy(tok.lexeme, buffer);
                    append_token(tokens, tok);
                }
                clear_buffer(buffer, buffer_index);
                buffer_index = 0;
            }
        }
    }
}
