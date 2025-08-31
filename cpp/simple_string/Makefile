CC = g++
BASE_CFLAGS = -std=c++98 -pedantic-errors -Wall -Wextra -fPIC
DEBUG_FLAGS = -g -DDEBUG
RELEASE_FLAGS = -O3 -DNDEBUG

CFLAGS_DEBUG = $(BASE_CFLAGS) $(DEBUG_FLAGS)
CFLAGS_RELEASE = $(BASE_CFLAGS) $(RELEASE_FLAGS)

INCLUDE_DIR = include
SRC_DIR = src
TEST_DIR = test
BIN_DEBUG_DIR = bin/debug
BIN_RELEASE_DIR = bin/release

PROJECT_NAME = simple_string

TARGET_DEBUG = $(PROJECT_NAME)_debug
TARGET_RELEASE = $(PROJECT_NAME)_release

SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
SRC_OBJS_DEBUG = $(patsubst $(SRC_DIR)/%.cpp,$(BIN_DEBUG_DIR)/%.o,$(SRC_FILES))
SRC_OBJS_RELEASE = $(patsubst $(SRC_DIR)/%.cpp,$(BIN_RELEASE_DIR)/%.o,$(SRC_FILES))

TEST_FILES = $(wildcard $(TEST_DIR)/*.cpp)

UTILS_INCLUDE = -I ../../utils/include
TEST_UTILS_INCLUDE = -I ../../utils/test

SHARED_LIB_DEBUG = $(BIN_DEBUG_DIR)/lib_debug_$(PROJECT_NAME).so
SHARED_LIB_RELEASE = $(BIN_RELEASE_DIR)/lib_release_$(PROJECT_NAME).so

HEADER_FILES = $(wildcard $(INCLUDE_DIR)/*.hpp)

.PHONY: all debug release clean run run_release vlg gdb check_headers

all: check_headers debug

check_headers:
	@echo "Checking header files in $(INCLUDE_DIR)..."
	@for hdr in $(HEADER_FILES); do \
		echo "  $$hdr"; \
		$(CC) $(CFLAGS_DEBUG) -I$(INCLUDE_DIR) $(UTILS_INCLUDE) $(TEST_UTILS_INCLUDE) -fsyntax-only $$hdr || exit 1; \
	done

debug: $(TARGET_DEBUG)
release: $(TARGET_RELEASE)

$(BIN_DEBUG_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BIN_DEBUG_DIR)
	@$(CC) $(CFLAGS_DEBUG) -I$(INCLUDE_DIR) $(UTILS_INCLUDE) $(TEST_UTILS_INCLUDE) -c $< -o $@

$(BIN_RELEASE_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BIN_RELEASE_DIR)
	@$(CC) $(CFLAGS_RELEASE) -I$(INCLUDE_DIR) $(UTILS_INCLUDE) $(TEST_UTILS_INCLUDE) -c $< -o $@

$(SHARED_LIB_DEBUG): $(SRC_OBJS_DEBUG) | $(BIN_DEBUG_DIR)
	@$(CC) -shared -o $@ $^

$(SHARED_LIB_RELEASE): $(SRC_OBJS_RELEASE) | $(BIN_RELEASE_DIR)
	@$(CC) -shared -o $@ $^

$(TARGET_DEBUG): $(TEST_FILES) $(SHARED_LIB_DEBUG)
	@$(CC) $(CFLAGS_DEBUG) -I$(INCLUDE_DIR) $(UTILS_INCLUDE) $(TEST_UTILS_INCLUDE) $^ -o $@

$(TARGET_RELEASE): $(TEST_FILES) $(SHARED_LIB_RELEASE)
	@$(CC) $(CFLAGS_RELEASE) -I$(INCLUDE_DIR) $(UTILS_INCLUDE) $(TEST_UTILS_INCLUDE) $^ -o $@

run: debug
	@echo "DEBUG MODE"
	@LD_LIBRARY_PATH=$(BIN_DEBUG_DIR) ./$(TARGET_DEBUG)

run_release: release
	@echo "RELEASE MODE"
	@LD_LIBRARY_PATH=$(BIN_RELEASE_DIR) ./$(TARGET_RELEASE)

vlg: $(TARGET_DEBUG)
	LD_LIBRARY_PATH=$(BIN_DEBUG_DIR) valgrind --leak-check=full --track-origins=yes ./$(TARGET_DEBUG)

gdb: $(TARGET_DEBUG)
	@echo "GDB MODE"
	@LD_LIBRARY_PATH=$(BIN_DEBUG_DIR) gdb ./$(TARGET_DEBUG)
	
$(BIN_DEBUG_DIR) $(BIN_RELEASE_DIR):
	mkdir -p $@

clean:
	@rm -rf $(TARGET_DEBUG) $(TARGET_RELEASE) \
	       $(BIN_DEBUG_DIR)/*.o $(BIN_DEBUG_DIR)/*.so \
	       $(BIN_RELEASE_DIR)/*.o $(BIN_RELEASE_DIR)/*.so
