CC = gcc
CFLAGS = -Wall -Wextra -std=c99
OBJ = main.o lex.o

all: lex

lex: $(OBJ)
	$(CC) $(CFLAGS) -o lex $(OBJ)

main.o: main.c token.h
	$(CC) $(CFLAGS) -c main.c

lex.o: lex.c token.h
	$(CC) $(CFLAGS) -c lex.c

clean:
	rm -f *.o lex
