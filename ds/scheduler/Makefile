# Makefile for scheduler (using only list_priority_queue.c as priority_queue.o)

CC = gcc
BASE_CFLAGS = -ansi -pedantic-errors -Wall -Wextra -fPIC
DEBUG_FLAGS = -g -DDEBUG
RELEASE_FLAGS = -O3 -DNDEBUG

CFLAGS_DEBUG = $(BASE_CFLAGS) $(DEBUG_FLAGS)
CFLAGS_RELEASE = $(BASE_CFLAGS) $(RELEASE_FLAGS)

PROJECT_NAME = scheduler
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

# Only list_priority_queue.c will be used as priority_queue.o
LIST_PQ_SRC = ../priority_queue/src/list_priority_queue.c
LIST_PQ_OBJ_DEBUG = $(BIN_DEBUG)/priority_queue.o
LIST_PQ_OBJ_RELEASE = $(BIN_RELEASE)/priority_queue.o

# Dependencies without priority_queue
DEPENDENCIES = task uid oll dll dynamic_vector 

DEP_INCLUDE = $(foreach dep,$(DEPENDENCIES),-I ../$(dep)/include) -I ../priority_queue/include
DEP_OBJ_DEBUG = $(foreach dep,$(DEPENDENCIES),$(shell find ../$(dep)/bin/debug -type f -name "*.o" ! -name "*test*"))
DEP_OBJ_RELEASE = $(foreach dep,$(DEPENDENCIES),$(shell find ../$(dep)/bin/release -type f -name "*.o" ! -name "*test*"))

.PHONY: all debug release run run_release clean vlg gdb

all: debug

debug: $(BIN_DEBUG) $(TARGET_DEBUG)

release: $(BIN_RELEASE) $(TARGET_RELEASE)

$(BIN_DEBUG):
	mkdir -p $(BIN_DEBUG)

$(BIN_RELEASE):
	mkdir -p $(BIN_RELEASE)

# Compile scheduler source files
$(BIN_DEBUG)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS_DEBUG) $(UTILS_INCLUDE) $(DEP_INCLUDE) -I $(INC_DIR) -c $< -o $@

$(BIN_RELEASE)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS_RELEASE) $(UTILS_INCLUDE) $(DEP_INCLUDE) -I $(INC_DIR) -c $< -o $@

# Compile list_priority_queue.c as priority_queue.o
$(LIST_PQ_OBJ_DEBUG): $(LIST_PQ_SRC)
	$(CC) $(CFLAGS_DEBUG) $(UTILS_INCLUDE) $(DEP_INCLUDE) -I ../priority_queue/include -c $< -o $@

$(LIST_PQ_OBJ_RELEASE): $(LIST_PQ_SRC)
	$(CC) $(CFLAGS_RELEASE) $(UTILS_INCLUDE) $(DEP_INCLUDE) -I ../priority_queue/include -c $< -o $@

# Build debug executable
$(TARGET_DEBUG): $(OBJ_DEBUG) $(LIST_PQ_OBJ_DEBUG) $(DEP_OBJ_DEBUG) $(TEST_DIR)/$(PROJECT_NAME)_test.c
	@[ -f $(UTILS_OBJ) ] || (echo "utils.o not found. Please build utils first." && exit 1)
	@for dep in $(DEPENDENCIES); do \
		make -C ../$$dep || exit 1; \
	done
	$(CC) $(CFLAGS_DEBUG) $(UTILS_INCLUDE) $(DEP_INCLUDE) -I $(INC_DIR) -o $@ $(OBJ_DEBUG) $(LIST_PQ_OBJ_DEBUG) $(DEP_OBJ_DEBUG) $(UTILS_OBJ) $(TEST_DIR)/$(PROJECT_NAME)_test.c

# Build release executable
$(TARGET_RELEASE): $(OBJ_RELEASE) $(LIST_PQ_OBJ_RELEASE) $(DEP_OBJ_RELEASE) $(TEST_DIR)/$(PROJECT_NAME)_test.c
	@[ -f $(UTILS_OBJ) ] || (echo "utils.o not found. Please build utils first." && exit 1)
	@for dep in $(DEPENDENCIES); do \
		make -C ../$$dep release || exit 1; \
	done
	$(CC) $(CFLAGS_RELEASE) $(UTILS_INCLUDE) $(DEP_INCLUDE) -I $(INC_DIR) -o $@ $(OBJ_RELEASE) $(LIST_PQ_OBJ_RELEASE) $(DEP_OBJ_RELEASE) $(UTILS_OBJ) $(TEST_DIR)/$(PROJECT_NAME)_test.c

run: debug
	./$(TARGET_DEBUG)

run_release: release
	./$(TARGET_RELEASE)

vlg: debug
	valgrind --leak-check=full ./$(TARGET_DEBUG)

gdb: debug
	gdb ./$(TARGET_DEBUG)

clean:
	rm -rf $(BIN_DEBUG) $(BIN_RELEASE)
