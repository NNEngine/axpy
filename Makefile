# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -std=c11 -Iincludes

# Libraries
LIBS = -lopenblas

# Directories
SRC_DIR = src
EX_DIR  = examples
OBJ_DIR = build

# Files
SRCS = $(SRC_DIR)/vector.c
EXE  = demo

# Default target
all: $(EXE)

# Build executable
$(EXE): $(EX_DIR)/demo.c $(SRCS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

# Clean build artifacts
clean:
	rm -f $(EXE)

# Phony targets
.PHONY: all clean
