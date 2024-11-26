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

# Directories and files
SRC_DIR = ./src
BISON_SRC = $(SRC_DIR)/bison/parser.y
LEX_SRC = $(SRC_DIR)/flex/scanner.l
MAIN_SRC = $(SRC_DIR)/main.c
SRC_FILES = $(MAIN_SRC) $(wildcard $(SRC_DIR)/*/*.c)
OBJECTS = $(SRC_FILES:.c=.o)

# Outputs
BINARY = etapa4
TAR_FILE = $(BINARY).tgz

# Test variables
TEST=tests/testreport.sh
TEST_OUT = output/

# Default target
all: $(BINARY)

# Compile final binary
$(BINARY): $(OBJECTS)
	$(CC) -o $@ $^

# Generate parser.tab.c and parser.tab.h
$(SRC_DIR)/bison/parser.tab.c $(SRC_DIR)/bison/parser.tab.h: $(BISON_SRC)
	$(BISON) -Wall -Werror -o $(@:.h=.c) -d $<

# Generate lex.yy.c
$(SRC_DIR)/flex/lex.yy.c: $(LEX_SRC) $(SRC_DIR)/bison/parser.tab.h
	$(LEX) -o $@ $<

# Compile .c files into .o
%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

# Clean up generated files
clean:
	rm -f $(BINARY) $(TAR_FILE) $(OBJECTS) \
		$(SRC_DIR)/bison/parser.tab.* $(SRC_DIR)/flex/lex.yy.c

# Run the binary
run: $(BINARY)
	./$<

# Create .tgz package
tar: $(BINARY)
	mkdir -p temp_dir/src
	cp -r $(SRC_DIR)/* temp_dir/src
	cp Makefile temp_dir/
	cp -r ./tests temp_dir/
	find temp_dir -name ".*" -exec rm -rf {} +
	tar cvzf $(TAR_FILE) -C temp_dir .
	rm -rf temp_dir

# Run tests
test: $(BINARY)
	rm -rf $(TEST_OUT)
	bash $(TEST)

# Phony targets
.PHONY: all run clean tar test
