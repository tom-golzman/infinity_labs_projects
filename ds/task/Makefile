CC = gcc
CFLAGS = -ansi -pedantic-errors -Wall -Wextra -g
GCOV_FLAGS = -fprofile-arcs -ftest-coverage

INCLUDE_DIR = include
SRC_DIR = src
TEST_DIR = test
BIN_DEBUG_DIR = bin/debug

PROJECT_NAME = task
TARGET = $(PROJECT_NAME)

SRC_FILES = $(SRC_DIR)/$(PROJECT_NAME).c
TEST_FILES = $(TEST_DIR)/$(PROJECT_NAME)_test.c
INC_FILES = $(INCLUDE_DIR)/$(PROJECT_NAME).h

SRC_OBJ_FILES = $(BIN_DEBUG_DIR)/$(notdir $(SRC_FILES:.c=.o))
TEST_OBJ_FILES = $(BIN_DEBUG_DIR)/$(notdir $(TEST_FILES:.c=.o))
OBJ_FILES = $(SRC_OBJ_FILES) $(TEST_OBJ_FILES) /home/tom/git/ds/uid/bin/debug/uid.o

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	@$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -iquote /home/tom/git/ds/uid/include/ -o $@ $^

$(BIN_DEBUG_DIR)/%.o: $(SRC_DIR)/%.c | $(BIN_DEBUG_DIR)
	@$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -iquote /home/tom/git/ds/uid/include/ -c $< -o $@

$(BIN_DEBUG_DIR)/%.o: $(TEST_DIR)/%.c | $(BIN_DEBUG_DIR)
	@$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -iquote /home/tom/git/ds/uid/include/ -c $< -o $@

$(BIN_DEBUG_DIR):
	mkdir -p $(BIN_DEBUG_DIR)

run: $(TARGET)
	@./$(TARGET)

coverage: clean
	$(CC) $(CFLAGS) $(GCOV_FLAGS) -I$(INCLUDE_DIR) $(SRC_FILES) $(TEST_FILES) -o $(TARGET)
	./$(TARGET)
	gcov $(SRC_FILES)

vlg: $(TARGET)
	valgrind --leak-check=yes --track-origins=yes ./$(TARGET)

clean:
	rm -f $(SRC_OBJ_FILES) $(TEST_OBJ_FILES) $(TARGET)
