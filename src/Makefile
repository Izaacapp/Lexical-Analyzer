# Compiler
CC = gcc

# Compiler flags
CFLAGS = -g -Wall -Wextra

# Executable name
TARGET = lexer

# Source files
SRCS = lex.c main.c

# Object files
OBJS = $(SRCS:.c=.o)

# Default target
all: $(TARGET)

# Rule to create the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Rule to create object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule to remove compiled files
clean:
	rm -f $(TARGET) $(OBJS)

# Phony targets
.PHONY: all clean
