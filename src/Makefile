CC = gcc
CFLAGS = -Wall -Wextra

all: lexical_analyzer

lexical_analyzer: lex.o main.o
	$(CC) $(CFLAGS) -o lexical_analyzer lex.o main.o

lex.o: lex.c token.h
	$(CC) $(CFLAGS) -c lex.c

main.o: main.c token.h
	$(CC) $(CFLAGS) -c main.c

clean:
	rm -f *.o lexical_analyzer

.PHONY: all clean