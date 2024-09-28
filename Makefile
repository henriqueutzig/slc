# Compiler and flags
CC = gcc
CFLAGS = -std=c11 -Wall -Iinclude

# Directories
SRCDIR = src
INCDIR = include
OBJDIR = obj

# Find all .c files in the src directory and its subdirectories
SRCS = $(shell find $(SRCDIR) -name '*.c')

# Convert the .c source files to corresponding .o object files in obj directory
OBJS = $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

# Target executable name
TARGET = program

# Default target
all: $(TARGET)

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
	rm includes/flex/lex.yy.h
