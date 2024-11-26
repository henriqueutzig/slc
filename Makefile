#####################################
#          Authors - Grupo J:       #
#     Henrique Utzig - 00319043     #
#     João Pedro Cosme - 00314792   #
#####################################

# Compilation commands
CC = gcc
CFLAGS = -I$(SRC_DIR) -I$(SRC_DIR)/errors -I$(SRC_DIR)/bison -I$(SRC_DIR)/asd -I$(SRC_DIR)/lexema -I$(SRC_DIR)/stack -I$(SRC_DIR)/symbol_table
LEX = flex
BISON = bison

# Source and object files
SRC_DIR = ./src
BISON_SRC = $(SRC_DIR)/bison/parser.y
LEX_SRC = $(SRC_DIR)/flex/scanner.l

MAIN_SRC = $(SRC_DIR)/main.c
SRC_FILES = $(MAIN_SRC) $(SRC_DIR)/asd/asd.c $(SRC_DIR)/lexema/lexema.c \
            $(SRC_DIR)/stack/stack.c $(SRC_DIR)/symbol_table/symbol_table.c \
            $(SRC_DIR)/symbol_table/content.c $(SRC_DIR)/flex/lex.yy.c \
            $(SRC_DIR)/bison/parser.tab.c

OBJECTS = $(SRC_FILES:.c=.o)

# Output files
BINARY = etapa4
TAR_FILE = $(BINARY).tgz

# Default target: clean and compile
all: $(BINARY)

# Rule to create the final binary
$(BINARY): $(OBJECTS)
	$(CC) -o $(BINARY) $(OBJECTS)

# Rule to generate parser.tab.h and parser.tab.c using bison
$(SRC_DIR)/bison/parser.tab.c $(SRC_DIR)/bison/parser.tab.h: $(BISON_SRC)
	$(BISON) -Wall -Werror -o $(SRC_DIR)/bison/parser.tab.c -d $(BISON_SRC)

# Rule to generate lex.yy.c using flex
$(SRC_DIR)/flex/lex.yy.c: $(LEX_SRC) $(SRC_DIR)/bison/parser.tab.h
	$(LEX) -o $(SRC_DIR)/flex/lex.yy.c $(LEX_SRC)

# Compile .c files into .o object files
%.o: %.c
	$(CC) -c -I$(SRC_DIR) -I$(SRC_DIR)/errors -I$(SRC_DIR)/bison -I$(SRC_DIR)/asd -I$(SRC_DIR)/lexema -I$(SRC_DIR)/stack -I$(SRC_DIR)/symbol_table -o $@ $<

# Rule to clean up the generated files
clean:
	rm -f $(BINARY) $(TAR_FILE) $(OBJECTS) $(SRC_DIR)/bison/parser.tab.c $(SRC_DIR)/bison/parser.tab.h $(SRC_DIR)/flex/lex.yy.c

# Rule to run the program
run: $(BINARY)
	./$(BINARY)

# Rule to create a .tgz file with the correct structure
tar: $(BINARY)
	mkdir -p temp_dir/src
	cp -r $(SRC_DIR)/* temp_dir/src
	cp Makefile temp_dir/
	cp -r ./tests temp_dir/
	find temp_dir -name ".*" -exec rm -rf {} +
	tar cvzf $(TAR_FILE) -C temp_dir .
	rm -rf temp_dir

# Run automated tests
test: $(BINARY)
	rm -rf ${TEST_OUT} 
	bash ${TEST}

# Phony targets
.PHONY: all run clean tar test
