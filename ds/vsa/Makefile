CC = gcc
BASE_CFLAGS = -ansi -pedantic-errors -Wall -Wextra
DEBUG_FLAGS = -g -DDEBUG
RELEASE_FLAGS = -O3 -DNDEBUG

CFLAGS_DEBUG = $(BASE_CFLAGS) $(DEBUG_FLAGS)
CFLAGS_RELEASE = $(BASE_CFLAGS) $(RELEASE_FLAGS)

INCLUDE_DIR = include
SRC_DIR = src
TEST_DIR = test
BIN_DEBUG_DIR = bin/debug
BIN_RELEASE_DIR = bin/release

PROJECT_NAME = vsa

TARGET_DEBUG = $(PROJECT_NAME)_debug
TARGET_RELEASE = $(PROJECT_NAME)_release

SRC_FILE = $(SRC_DIR)/$(PROJECT_NAME).c
TEST_FILE = $(TEST_DIR)/$(PROJECT_NAME)_test.c
INC_FILE = $(INCLUDE_DIR)/$(PROJECT_NAME).h

SRC_OBJ_DEBUG = $(BIN_DEBUG_DIR)/$(PROJECT_NAME).o
SRC_OBJ_RELEASE = $(BIN_RELEASE_DIR)/$(PROJECT_NAME).o

.PHONY: all debug release coverage clean run run_release vlg

all: debug

debug: $(TARGET_DEBUG)
release: $(TARGET_RELEASE)

$(BIN_DEBUG_DIR)/%.o: $(SRC_DIR)/%.c | $(BIN_DEBUG_DIR)
	@$(CC) $(CFLAGS_DEBUG)   -I$(INCLUDE_DIR) -c $< -o $@

$(BIN_RELEASE_DIR)/%.o: $(SRC_DIR)/%.c | $(BIN_RELEASE_DIR)
	@$(CC) $(CFLAGS_RELEASE) -I$(INCLUDE_DIR) -c $< -o $@

$(TARGET_DEBUG): $(SRC_OBJ_DEBUG) $(TEST_FILE)
	@$(CC) $(CFLAGS_DEBUG)   $^ -I$(INCLUDE_DIR) -o $@

$(TARGET_RELEASE): $(SRC_OBJ_RELEASE) $(TEST_FILE)
	@$(CC) $(CFLAGS_RELEASE) $^ -I$(INCLUDE_DIR) -o $@
	
run: debug
	@./$(TARGET_DEBUG)

run_release: release
	@./$(TARGET_RELEASE)
	
vlg: debug
	valgrind --leak-check=full --track-origins=yes ./$(TARGET_DEBUG)

$(BIN_DEBUG_DIR) $(BIN_RELEASE_DIR):
	@mkdir -p $@
	
clean:
	rm -rf $(TARGET_DEBUG) $(TARGET_RELEASE)
