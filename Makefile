# Compiler and flags
CC = gcc
FLEX_LIB_PATH = $(shell find /opt/homebrew -name "libfl.a" | awk -F'/' '{sub("/" $$NF, ""); print}')
CFLAGS = -std=c11 -Wall -Iinclude -lfl -L$(FLEX_LIB_PATH) #-v

# Directories
SRCDIR = src
INCDIR = include
OBJDIR = obj

# Find all .c files in the src directory and its subdirectories
SRCS = $(shell find $(SRCDIR) -name '*.c')

# Convert the .c source files to corresponding .o object files in obj directory
OBJS = $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

# Target executable name
TARGET = etapa1

# Default target
all: $(TARGET)

# flex --header-file=includes/flex/lex.yy.h includes/flex/scanner.l

# Build the final executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Rule for building .o files from .c files
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)  # Ensure the directory exists
	$(CC) $(CFLAGS) -c $< -o $@

# Include dependency files, if they exist
-include $(OBJS:.o=.d)

# Generate dependency files for each .c file
$(OBJDIR)/%.d: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $< -MM -MT $(@:.d=.o) >$@

# Clean up the build
.PHONY: clean
clean:
	rm -rf $(OBJDIR) $(TARGET)
	if [ cat includes/flex/lex.yy.h ]; then 
		rm includes/flex/lex.yy.h
	fi