#####################################
#          Authors - Grupo J:       #
#     Henrique Utzig - 00319043     #
#     João Pedro Cosme - 00314792   #
#####################################

# Compilation commands
CC = gcc
CFLAGS = -I./src -I./src/errors -I./src/bison -I./src/asd -I./src/lexema -I./src/stack -I./src/symbol_table -I./src/iloc -I./src/code_generator
LEX = flex
BISON = bison

# Directories and files
SRC_DIR = ./src
BISON_SRC = $(SRC_DIR)/bison/parser.y
LEX_SRC = $(SRC_DIR)/flex/scanner.l
SRC_FILES = $(SRC_DIR)/main.c \
            $(SRC_DIR)/asd/asd.c \
            $(SRC_DIR)/iloc/iloc.c \
            $(SRC_DIR)/code_generator/code_generator.c \
            $(SRC_DIR)/lexema/lexema.c \
            $(SRC_DIR)/stack/stack.c \
            $(SRC_DIR)/symbol_table/symbol_table.c \
            $(SRC_DIR)/symbol_table/content.c \
            $(SRC_DIR)/flex/lex.yy.c \
            $(SRC_DIR)/bison/parser.tab.c
OBJECTS = $(SRC_FILES:.c=.o)

# Output files
BINARY = etapa5
TAR_FILE = $(BINARY).tgz
TEST_OUT = output/
TEST = tests/testreport.sh

# Default target: build binary
all: $(BINARY)

# Rule to create the final binary
$(BINARY): $(OBJECTS)
	$(CC) -o $@ $(OBJECTS)

# Rule to generate parser.tab.h and parser.tab.c using bison
$(SRC_DIR)/bison/parser.tab.c $(SRC_DIR)/bison/parser.tab.h: $(BISON_SRC)
	$(BISON) -Wall -Werror -d -o $(SRC_DIR)/bison/parser.tab.c $<

# Rule to generate lex.yy.c using flex
$(SRC_DIR)/flex/lex.yy.c: $(LEX_SRC) $(SRC_DIR)/bison/parser.tab.h
	$(LEX) -o $@ $<

# Compile .c files into .o object files
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Clean up generated files
clean:
	rm -f $(BINARY) $(TAR_FILE) $(OBJECTS) $(SRC_DIR)/bison/parser.tab.c $(SRC_DIR)/bison/parser.tab.h $(SRC_DIR)/flex/lex.yy.c

# Run the program
run: $(BINARY)
	./$(BINARY)

# Create a .tgz archive with the project
tar: $(BINARY)
	mkdir -p temp_dir/src
	cp -r $(SRC_DIR)/* temp_dir/src
	cp Makefile temp_dir/
	cp -r ./tests temp_dir/
	find ./temp_dir -name "*.o" -type f -delete
	find temp_dir -name ".*" -exec rm -rf {} +
	tar cvzf $(TAR_FILE) --exclude="\..*" -C temp_dir .
	rm -rf temp_dir

# Run automated tests
test: $(BINARY)
	rm -rf $(TEST_OUT)
	bash $(TEST)

# Phony targets
.PHONY: all run clean tar test
