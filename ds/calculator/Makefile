CC = gcc
BASE_CFLAGS = -ansi -pedantic-errors -Wall -Wextra -fPIC
DEBUG_FLAGS = -g -DDEBUG
RELEASE_FLAGS = -O3 -DNDEBUG

CFLAGS_DEBUG = $(BASE_CFLAGS) $(DEBUG_FLAGS)
CFLAGS_RELEASE = $(BASE_CFLAGS) $(RELEASE_FLAGS)

INCLUDE_DIR = include
SRC_DIR = src
TEST_DIR = test
BIN_DEBUG_DIR = bin/debug
BIN_RELEASE_DIR = bin/release

STACK_INC = ../stack/include
STACK_BIN_DEBUG_DIR = ../stack/bin/debug
STACK_BIN_RELEASE_DIR = ../stack/bin/release

PROJECT_NAME = calculator

TARGET_DEBUG = $(PROJECT_NAME)_debug
TARGET_RELEASE = $(PROJECT_NAME)_release

SRC_FILE = $(SRC_DIR)/$(PROJECT_NAME).c
TEST_FILE = $(TEST_DIR)/$(PROJECT_NAME)_test.c
INC_FILE = $(INCLUDE_DIR)/$(PROJECT_NAME).h

SRC_OBJ_DEBUG = $(BIN_DEBUG_DIR)/$(PROJECT_NAME).o
SRC_OBJ_RELEASE = $(BIN_RELEASE_DIR)/$(PROJECT_NAME).o

STACK_OBJ_DEBUG = $(STACK_BIN_DEBUG_DIR)/stack.o
STACK_OBJ_RELEASE = $(STACK_BIN_RELEASE_DIR)/stack.o

SHARED_LIB_DEBUG = $(BIN_DEBUG_DIR)/lib_debug_$(PROJECT_NAME).so
SHARED_LIB_RELEASE = $(BIN_RELEASE_DIR)/lib_release_$(PROJECT_NAME).so

.PHONY: all debug release coverage clean run run_release vlg

all: debug

debug: $(TARGET_DEBUG)
release: $(TARGET_RELEASE)

$(SHARED_LIB_DEBUG): $(SRC_OBJ_DEBUG) $(STACK_OBJ_DEBUG) | $(BIN_DEBUG_DIR) 
	@$(CC) -shared -o $@ $^

$(SHARED_LIB_RELEASE): $(SRC_OBJ_RELEASE) $(STACK_OBJ_RELEASE) | $(BIN_RELEASE_DIR)
	@$(CC) -shared -o $@ $^

$(BIN_DEBUG_DIR)/%.o: $(SRC_DIR)/%.c | $(BIN_DEBUG_DIR)
	@$(CC) $(CFLAGS_DEBUG) -I$(INCLUDE_DIR) -I$(STACK_INC) -c $< -o $@

$(BIN_RELEASE_DIR)/%.o: $(SRC_DIR)/%.c | $(BIN_RELEASE_DIR)
	@$(CC) $(CFLAGS_RELEASE) -I$(INCLUDE_DIR) -I$(STACK_INC) -c $< -o $@

$(TARGET_DEBUG): $(TEST_FILE) $(SHARED_LIB_DEBUG)
	@$(CC) $(CFLAGS_DEBUG) -I$(INCLUDE_DIR) -I$(STACK_INC) \
		$(TEST_FILE) \
		-L$(STACK_BIN_DEBUG_DIR) -Wl,-rpath=$(STACK_BIN_DEBUG_DIR) -l_debug_stack \
		-L$(BIN_DEBUG_DIR) -Wl,-rpath=$(BIN_DEBUG_DIR) -l_debug_$(PROJECT_NAME) \
		-o $@

$(TARGET_RELEASE): $(TEST_FILE) $(SHARED_LIB_RELEASE)
	@$(CC) $(CFLAGS_RELEASE) -I$(INCLUDE_DIR) -I$(STACK_INC) \
		$(TEST_FILE) \
		-L$(STACK_BIN_RELEASE_DIR) -Wl,-rpath=$(STACK_BIN_RELEASE_DIR) -l_release_stack \
		-L$(BIN_RELEASE_DIR) -Wl,-rpath=$(BIN_RELEASE_DIR) -l_release_$(PROJECT_NAME) \
		-o $@
		
run: debug
	@echo "DEBUG MODE"
	@LD_LIBRARY_PATH=$(BIN_DEBUG_DIR):$(dir $(STACK_BIN_DEBUG_DIR)) ./$(TARGET_DEBUG)

run_release: release
	@echo "RELEASE MODE"
	@LD_LIBRARY_PATH=$(BIN_RELEASE_DIR):$(dir $(STACK_BIN_RELEASE_DIR)) ./$(TARGET_RELEASE)

vlg: $(TARGET_DEBUG)
	LD_LIBRARY_PATH=$(BIN_DEBUG_DIR):$(dir $(STACK_BIN_DEBUG_DIR)) valgrind --leak-check=full --track-origins=yes ./$(TARGET_DEBUG)

$(BIN_DEBUG_DIR) $(BIN_RELEASE_DIR):
	mkdir -p $@

clean:
	@rm -rf $(TARGET_DEBUG) $(TARGET_RELEASE) \
	       $(BIN_DEBUG_DIR)/*.o $(BIN_DEBUG_DIR)/*.so \
	       $(BIN_RELEASE_DIR)/*.o $(BIN_RELEASE_DIR)/*.so
