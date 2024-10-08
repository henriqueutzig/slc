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
LEX_SRC = $(SRC_DIR)/flex/scanner.l
BISON_SRC = $(SRC_DIR)/bison/parser.y
TOKENS_H = $(SRC_DIR)/includes/tokens.h
LEX_OUT = $(SRC_DIR)/flex/lex.yy.c

BISON_H_OUT = $(SRC_DIR)/bison/parser.tab.h
BISON_C_OUT = $(SRC_DIR)/bison/parser.tab.c


BINARY = etapa2

# Compilation commands
CC = gcc
LEX = flex
BISON = bison

# Output files
TAR_FILE = $(BINARY).tgz

# Default target: compile and run
all: $(BINARY)

# Rule to create the final binary
$(BINARY): $(MAIN_SRC) $(LEX_OUT) $(TOKENS_H) $(BISON_H_OUT) $(BISON_C_OUT)
	$(CC) -I$(SRC_DIR) $(MAIN_SRC) $(LEX_OUT) $(BISON_C_OUT) $(BISON_C_OUT) -o $(BINARY)

# Rule to generate lex.yy.c using flex
$(LEX_OUT): $(LEX_SRC)
	$(LEX) -o $(LEX_OUT) $(LEX_SRC)

# Rule to generate  using bison 
$(BISON_H_OUT): $(BISON_SRC)
	$(BISON) -o  -o $(BISON_H_OUT) -d $(BISON_SRC)

# # Rule to generate  using bison 
$(BISON_C_OUT): $(BISON_SRC)
	$(BISON) -o $(BISON_C_OUT) -d $(BISON_SRC)

# Rule to run the program
run: $(BINARY)
	./$(BINARY)

# Rule to create a .tgz file with the correct structure
tar: $(BINARY)
	mkdir -p temp_dir
	cp $(LEX_SRC) $(MAIN_SRC) $(TOKENS_H) Makefile temp_dir/
	tar cvzf $(TAR_FILE) -C temp_dir .
	rm -f $(LEX_OUT) $(BISON_C_OUT $(BISON_H_OUT)
	rm -rf temp_dir

# Clean up the generated files
clean:
	rm -f $(LEX_OUT) $(BINARY) $(TAR_FILE)

# Phony targets
.PHONY: all run clean tar
