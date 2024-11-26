#####################################
#          Authors - Grupo J:       #
#     Henrique Utzig - 00319043     #
#     João Pedro Cosme - 00314792   #
#####################################

# Define sources and targets
SRC_DIR = ./src

MAIN_SRC = $(SRC_DIR)/main.c
LEX_SRC = $(SRC_DIR)/flex/scanner.l
LEX_OUT = $(SRC_DIR)/flex/lex.yy.c
BISON_SRC = $(SRC_DIR)/bison/parser.y
BISON_H_OUT = $(SRC_DIR)/bison/parser.tab.h
BISON_C_OUT = $(SRC_DIR)/bison/parser.tab.c

ASD_C = $(SRC_DIR)/asd/asd.c
LEXEMA_C = $(SRC_DIR)/lexema/lexema.c
STACK_C = $(SRC_DIR)/stack/stack.c
SYMBOL_TABLE_C = $(SRC_DIR)/symbol_table/symbol_table.c
CONTENT_C = $(SRC_DIR)/symbol_table/content.c

OBJS = main.o lex.yy.o parser.tab.o asd.o lexema.o stack.o symbol_table.o content.o

# Output files
BINARY = etapa4
TAR_FILE = $(BINARY).tgz

# Automated test paths
TEST = tests/testreport.sh
TEST_OUT = output/

# Compilation commands
CC = gcc
CFLAGS = -I$(SRC_DIR) -I$(SRC_DIR)/errors -I$(SRC_DIR)/bison -I$(SRC_DIR)/asd -I$(SRC_DIR)/lexema -I$(SRC_DIR)/stack -I$(SRC_DIR)/symbol_table
LEX = flex
BISON = bison

# Default target: clean and compile
all: $(BINARY)

# Rule to create the final binary
$(BINARY): $(OBJS)
	$(CC) -o $(BINARY) $(OBJS)

# Rule to generate .o files
main.o: $(MAIN_SRC)
	$(CC) $(CFLAGS) -c $(MAIN_SRC) -o main.o

lex.yy.o: $(LEX_OUT) $(BISON_H_OUT)
	$(CC) $(CFLAGS) -c $(LEX_OUT) -o lex.yy.o

parser.tab.o: $(BISON_C_OUT)
	$(CC) $(CFLAGS) -c $(BISON_C_OUT) -o parser.tab.o

asd.o: $(ASD_C)
	$(CC) $(CFLAGS) -c $(ASD_C) -o asd.o

lexema.o: $(LEXEMA_C)
	$(CC) $(CFLAGS) -c $(LEXEMA_C) -o lexema.o

stack.o: $(STACK_C)
	$(CC) $(CFLAGS) -c $(STACK_C) -o stack.o

symbol_table.o: $(SYMBOL_TABLE_C)
	$(CC) $(CFLAGS) -c $(SYMBOL_TABLE_C) -o symbol_table.o

content.o: $(CONTENT_C)
	$(CC) $(CFLAGS) -c $(CONTENT_C) -o content.o

# Rule to generate parser.tab.h and parser.tab.c using bison
$(BISON_H_OUT) $(BISON_C_OUT): $(BISON_SRC)
	$(BISON) -Wall -Werror -Wcounterexamples -Wother -Wconflicts-sr -Wconflicts-rr -o $(BISON_C_OUT) -d $(BISON_SRC)

# Rule to generate lex.yy.c using flex
$(LEX_OUT): $(LEX_SRC)
	$(LEX) -o $(LEX_OUT) $(LEX_SRC)

# Rule to clean up the generated files
clean:
	rm -f $(BINARY) $(TAR_FILE) $(OBJS) $(BISON_C_OUT) $(BISON_H_OUT) $(LEX_OUT)

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
