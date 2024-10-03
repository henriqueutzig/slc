# Define paths
SRC_DIR = ./src
INCLUDE_DIR = ./src/includes
FLEX_DIR = $(INCLUDE_DIR)/flex
TOKENS_DIR = $(INCLUDE_DIR)/tokens

# Define sources and targets
MAIN_SRC = $(SRC_DIR)/main.c
LEX_SRC = $(FLEX_DIR)/scanner.l
TOKENS_H = $(TOKENS_DIR)/tokens.h
LEX_OUT = lex.yy.c
BINARY = etapa1

# Compilation commands
CC = gcc
LEX = flex
BISON = bison

# Output files
TAR_FILE = etapa1.tgz

# Default target: compile and run
all: $(BINARY)

# Rule to create the final binary
$(BINARY): $(MAIN_SRC) $(LEX_OUT) $(TOKENS_H)
	$(CC) -I$(INCLUDE_DIR) $(MAIN_SRC) $(LEX_OUT) -o $(BINARY)

# Rule to generate lex.yy.c using flex
$(LEX_OUT): $(LEX_SRC)
	$(LEX) $(LEX_SRC)

# Rule to run the program
run: $(BINARY)
	./$(BINARY)

# Rule to create a .tgz file for submission
tar: $(BINARY)
	tar cvzf $(TAR_FILE) .

# Clean up the generated files
clean:
	rm -f $(LEX_OUT) $(BINARY) $(TAR_FILE)

# Phony targets
.PHONY: all run clean tar
