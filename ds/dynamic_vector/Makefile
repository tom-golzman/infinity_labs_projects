# Auto-generated Makefile for dynamic_vector

CC = gcc
BASE_CFLAGS = -ansi -pedantic-errors -Wall -Wextra -fPIC
DEBUG_FLAGS = -g -DDEBUG
RELEASE_FLAGS = -O3 -DNDEBUG

CFLAGS_DEBUG = $(BASE_CFLAGS) $(DEBUG_FLAGS)
CFLAGS_RELEASE = $(BASE_CFLAGS) $(RELEASE_FLAGS)

PROJECT_NAME = dynamic_vector
SRC_DIR = src
TEST_DIR = test
INC_DIR = include
BIN_DEBUG = bin/debug
BIN_RELEASE = bin/release

UTILS_INCLUDE = -I ../../utils/include -I ../../utils/test
UTILS_OBJ = ../../utils/utils.o

TARGET_DEBUG = $(BIN_DEBUG)/$(PROJECT_NAME).out
TARGET_RELEASE = $(BIN_RELEASE)/$(PROJECT_NAME).out

SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_DEBUG = $(patsubst $(SRC_DIR)/%.c,$(BIN_DEBUG)/%.o,$(SRC_FILES))
OBJ_RELEASE = $(patsubst $(SRC_DIR)/%.c,$(BIN_RELEASE)/%.o,$(SRC_FILES))

# ➤ Add dependency project names here
DEPENDENCIES = 

DEP_INCLUDE = $(foreach dep,$(DEPENDENCIES),-I ../$(dep)/include)
DEP_OBJ_DEBUG = $(foreach dep,$(DEPENDENCIES),$(wildcard ../$(dep)/bin/debug/*.o))
DEP_OBJ_RELEASE = $(foreach dep,$(DEPENDENCIES),$(wildcard ../$(dep)/bin/release/*.o))

.PHONY: all debug release run run_release clean vlg gdb

all: debug

debug: $(BIN_DEBUG) $(TARGET_DEBUG)

release: $(BIN_RELEASE) $(TARGET_RELEASE)

$(BIN_DEBUG):
	mkdir -p $(BIN_DEBUG)

$(BIN_RELEASE):
	mkdir -p $(BIN_RELEASE)

# Compile source files
$(BIN_DEBUG)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS_DEBUG) $(UTILS_INCLUDE) $(DEP_INCLUDE) -I $(INC_DIR) -c $< -o $@

$(BIN_RELEASE)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS_RELEASE) $(UTILS_INCLUDE) $(DEP_INCLUDE) -I $(INC_DIR) -c $< -o $@

# Build executable with dependencies + shared utils.o
$(TARGET_DEBUG): $(OBJ_DEBUG) $(DEP_OBJ_DEBUG) $(TEST_DIR)/$(PROJECT_NAME)_test.c
	@for dep in $(DEPENDENCIES); do \
		make -C ../$$dep || exit 1; \
	done
	$(CC) $(CFLAGS_DEBUG) $(UTILS_INCLUDE) $(DEP_INCLUDE) -I $(INC_DIR) -o $@ $(OBJ_DEBUG) $(DEP_OBJ_DEBUG) $(UTILS_OBJ) $(TEST_DIR)/$(PROJECT_NAME)_test.c

$(TARGET_RELEASE): $(OBJ_RELEASE) $(DEP_OBJ_RELEASE) $(TEST_DIR)/$(PROJECT_NAME)_test.c
	@for dep in $(DEPENDENCIES); do \
		make -C ../$$dep release || exit 1; \
	done
	$(CC) $(CFLAGS_RELEASE) $(UTILS_INCLUDE) $(DEP_INCLUDE) -I $(INC_DIR) -o $@ $(OBJ_RELEASE) $(DEP_OBJ_RELEASE) $(UTILS_OBJ) $(TEST_DIR)/$(PROJECT_NAME)_test.c

# Run targets
run: debug
	./$(TARGET_DEBUG)

run_release: release
	./$(TARGET_RELEASE)

# Tools
vlg: debug
	valgrind --leak-check=full ./$(TARGET_DEBUG)

gdb: debug
	gdb ./$(TARGET_DEBUG)

# Cleanup
clean:
	rm -rf $(BIN_DEBUG) $(BIN_RELEASE)

