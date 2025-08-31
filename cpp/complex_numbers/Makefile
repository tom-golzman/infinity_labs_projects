# Makefile for Complex Numbers project (header-only)

CXX = g++
BASE_CXXFLAGS = -std=c++98 -pedantic-errors -Wall -Wextra -fPIC
DEBUG_FLAGS = -g -DDEBUG
RELEASE_FLAGS = -O3 -DNDEBUG

CXXFLAGS_DEBUG = $(BASE_CXXFLAGS) $(DEBUG_FLAGS)
CXXFLAGS_RELEASE = $(BASE_CXXFLAGS) $(RELEASE_FLAGS)

PROJECT_NAME = complex_numbers
TEST_DIR = test
INC_DIR = include
BIN_DEBUG = bin/debug
BIN_RELEASE = bin/release

UTILS_INCLUDE = -I ../../utils/include -I ../../utils/test
UTILS_OBJ = ../../utils/utils.o

TARGET_DEBUG = $(BIN_DEBUG)/$(PROJECT_NAME).out
TARGET_RELEASE = $(BIN_RELEASE)/$(PROJECT_NAME).out

TEST_SRC = $(TEST_DIR)/test_$(PROJECT_NAME).cpp

.PHONY: all debug release run run_release clean vlg gdb

all: debug

debug: $(BIN_DEBUG) $(TARGET_DEBUG)

release: $(BIN_RELEASE) $(TARGET_RELEASE)

$(BIN_DEBUG):
	mkdir -p $(BIN_DEBUG)

$(BIN_RELEASE):
	mkdir -p $(BIN_RELEASE)

# Build debug executable
$(TARGET_DEBUG): $(TEST_SRC)
	@[ -f $(UTILS_OBJ) ] || (echo "utils.o not found. Please build utils first." && exit 1)
	$(CXX) $(CXXFLAGS_DEBUG) $(UTILS_INCLUDE) -I $(INC_DIR) -o $@ $^ $(UTILS_OBJ)

# Build release executable
$(TARGET_RELEASE): $(TEST_SRC)
	@[ -f $(UTILS_OBJ) ] || (echo "utils.o not found. Please build utils first." && exit 1)
	$(CXX) $(CXXFLAGS_RELEASE) $(UTILS_INCLUDE) -I $(INC_DIR) -o $@ $^ $(UTILS_OBJ)

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