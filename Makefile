# Compiler settings
CC ?= gcc
OPT ?= -O2
SANITIZE_ADDR ?= -fsanitize=address
UNUSED_FLGS ?= -Wall -Wextra -Wpedantic -Wunused -Wunused-variable -Wunused-parameter -Wunused-label -Wunused-value -Wunused-macros -Wunused-local-typedefs
# UNUSED_FLGS += -Wunused-function
CFLAGS = -Wall -Wextra -g -std=c23 $(OPT) -I./include $(SANITIZE_ADDR) $(UNUSED_FLGS)

# Directories
OUT_DIR = out
SRC_DIR = src
OBJ_DIR = $(OUT_DIR)/obj

# Output executable
TARGET = ./$(OUT_DIR)/main

# Find all .c files in src/ and map them to .o files in obj/
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

# Default target
all: $(TARGET)

# Link object files to create the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create the object directory if it doesn't exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Clean up build artifacts
clean:
	rm -rf $(OUT_DIR)

hello:
	@echo "Hello"

.PHONY: all clean hello
