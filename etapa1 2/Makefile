# Check for the existence of the 'src' directory
ifeq ($(wildcard src),)
    # If 'src' directory does not exist, use flat structure
    SRC_DIR = .
else
    # If 'src' directory exists, use structured organization
    SRC_DIR = ./src
endif

# Define sources and targets
MAIN_SRC = $(SRC_DIR)/main.c
LEX_SRC = $(SRC_DIR)/scanner.l
TOKENS_H = $(SRC_DIR)/tokens.h
LEX_OUT = lex.yy.c
BINARY = etapa1

# Compilation commands
CC = gcc
LEX = flex

# Output files
TAR_FILE = etapa1.tgz

# Default target: compile and run
all: $(BINARY)

# Rule to create the final binary
$(BINARY): $(MAIN_SRC) $(LEX_OUT) $(TOKENS_H)
	$(CC) -I$(SRC_DIR) $(MAIN_SRC) $(LEX_OUT) -o $(BINARY)

# Rule to generate lex.yy.c using flex
$(LEX_OUT): $(LEX_SRC)
	$(LEX) $(LEX_SRC)

# Rule to run the program
run: $(BINARY)
	./$(BINARY)

# Rule to create a .tgz file with the correct structure
tar: $(BINARY)
	mkdir -p temp_dir
	cp $(LEX_SRC) $(MAIN_SRC) $(TOKENS_H) Makefile temp_dir/
	tar cvzf $(TAR_FILE) -C temp_dir .
	rm -rf temp_dir

# Clean up the generated files
clean:
	rm -f $(LEX_OUT) $(BINARY) $(TAR_FILE)

# Phony targets
.PHONY: all run clean tar
