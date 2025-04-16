CC = gcc
CFLAGS = -ansi -pedantic-errors -Wall -Wextra -g

INCLUDE_DIR = include
SRC_DIR = src
TEST_DIR = test
BIN_DEBUG_DIR = bin/debug

PROJECT_NAME = stack
TARGET = ${PROJECT_NAME}

SRC_FILES = $(SRC_DIR)/$(PROJECT_NAME).c
TEST_FILES = $(TEST_DIR)/$(PROJECT_NAME)_test.c

SRC_OBJ_FILES = $(patsubst %.c, $(BIN_DEBUG_DIR)/%.o, $(notdir $(SRC_FILES)))
TEST_OBJ_FILES = $(patsubst %.c, $(BIN_DEBUG_DIR)/%.o, $(notdir $(TEST_FILES)))
OBJ_FILES = $(SRC_OBJ_FILES) $(TEST_OBJ_FILES)

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	@$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -o $@ $^
	@rm -f $(TEST_OBJ_FILES)

$(BIN_DEBUG_DIR)/%.o: $(SRC_DIR)/%.c | $(BIN_DEBUG_DIR)
	@$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

$(BIN_DEBUG_DIR)/%.o: $(TEST_DIR)/%.c | $(BIN_DEBUG_DIR)
	@$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

$(BIN_DEBUG_DIR):
	@mkdir -p $(BIN_DEBUG_DIR)

run: $(TARGET)
	@./$(TARGET)

clean:
	rm -f $(SRC_OBJ_FILES) $(TEST_OBJ_FILES) $(TARGET)
