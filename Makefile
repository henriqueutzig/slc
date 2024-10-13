# Define sources and targets
SRC_DIR = ./src

MAIN_SRC = $(SRC_DIR)/main.c
MAIN_OBJ = main.o

LEX_SRC = $(SRC_DIR)/flex/scanner.l
LEX_OUT = $(SRC_DIR)/flex/lex.yy.c
LEX_OBJ = lex.yy.o

BISON_SRC = $(SRC_DIR)/bison/parser.y
BISON_H_OUT = $(SRC_DIR)/bison/parser.tab.h
BISON_C_OUT = $(SRC_DIR)/bison/parser.tab.c
BISON_OBJ = parser.tab.o 

# Output files
TAR_FILE = $(BINARY).tgz
BINARY = etapa2

TEST = tests/test.sh
TEST_OUT = output/

# Compilation commands
CC = gcc
LEX = flex
BISON = bison

# Default target: compile and run
all: $(BINARY)

# Rule to create the final binary
$(BINARY): $(MAIN_SRC) $(LEX_OUT) $(TOKENS_H) $(BISON_C_OUT) $(BISON_H_OUT)
	$(CC) -I $(SRC_DIR) -c $(MAIN_SRC) $(LEX_OUT) $(BISON_C_OUT)
	$(CC) $(MAIN_OBJ) $(LEX_OBJ) $(BISON_OBJ) -o $(BINARY)

# Rule to generate parser.tab.h using bison 
$(BISON_H_OUT): $(BISON_SRC)
	$(BISON) -o $(BISON_H_OUT) -d $(BISON_SRC)
	
# Rule to generate parser.tab.c using bison 
$(BISON_C_OUT): $(BISON_SRC)
	$(BISON) -o $(BISON_C_OUT) -d $(BISON_SRC)	

# Rule to generate lex.yy.c using flex
$(LEX_OUT): $(LEX_SRC)
	$(LEX) -o $(LEX_OUT) $(LEX_SRC)

# Rule to run the program
run: $(BINARY)
	./$(BINARY)

# Rule to create a .tgz file with the correct structure
tar: $(BINARY)
	mkdir -p temp_dir
	cp $(LEX_SRC) $(MAIN_SRC) Makefile temp_dir/
	tar cvzf $(TAR_FILE) -C temp_dir .
	# rm -rf temp_dir

# Clean up the generated files
clean:
	rm -f $(LEX_OUT) $(BINARY) $(TAR_FILE) $(BISON_C_OUT) $(BISON_H_OUT) *.o

# Run automated tests
test: $(BINARY)
	rm -rf ${TEST_OUT} 
	bash ${TEST}

# Phony targets
.PHONY: all run clean tar test
