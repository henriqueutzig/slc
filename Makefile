#####################################
# 		Authors - Grupo J:			#
# 	Henrique Utzig - 00319043		#
# 	João Pedro Cosme - 00314792		#
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

# Output files
BINARY = etapa3
TAR_FILE = $(BINARY).tgz

# Automated test paths
TEST = tests/testreport.sh
TEST_OUT = output/

# Compilation commands
CC = gcc
LEX = flex
BISON = bison

# Default target: clean and compile
all: clean $(BINARY)

# Rule to create the final binary
$(BINARY): $(MAIN_SRC) $(LEX_OUT) $(BISON_C_OUT) $(ASD_C) $(LEXEMA_C)
	$(CC) -I$(SRC_DIR) -I$(SRC_DIR)/bison -I$(SRC_DIR)/asd -I$(SRC_DIR)/lexema -o $(BINARY) $(MAIN_SRC) $(LEX_OUT) $(BISON_C_OUT) $(ASD_C) $(LEXEMA_C)

# Rule to generate parser.tab.h and parser.tab.c using bison
$(BISON_H_OUT) $(BISON_C_OUT): $(BISON_SRC)
	$(BISON) -Wall -Werror -Wcounterexamples -Wother -Wconflicts-sr -Wconflicts-rr -o $(BISON_C_OUT) -d $(BISON_SRC)

# Rule to generate lex.yy.c using flex
$(LEX_OUT): $(LEX_SRC)
	$(LEX) -o $(LEX_OUT) $(LEX_SRC)

# Rule to clean up the generated files
clean:
	rm -f $(BINARY) $(TAR_FILE) $(BISON_C_OUT) $(BISON_H_OUT) $(LEX_OUT)

# Rule to run the program
run: $(BINARY)
	./$(BINARY)

# Rule to create a .tgz file with the correct structure
tar: $(BINARY)
	mkdir -p temp_dir/src
	cp -r $(SRC_DIR)/* temp_dir/src
	cp Makefile temp_dir/
	cp -r ./tests temp_dir/
	tar cvzf $(TAR_FILE) -C temp_dir .
	rm -rf temp_dir

# Run automated tests
test: $(BINARY)
	rm -rf ${TEST_OUT} 
	bash ${TEST}

# Live reload: Tests will be run each time a file is saved
serve: $(BINARY) 
	find ${SRC_DIR} ./tests -type f | entr -c make test

# Phony targets
.PHONY: all run clean tar test serve
