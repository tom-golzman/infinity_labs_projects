CC = gcc
CFLAGS = -ansi -pedantic-errors -Wall -Wextra -fPIC
VLG = valgrind --leak-check=yes --track-origins=yes

PROJECT := $(if $(p),$(p),$(PROJECT))
PROJECT ?=

TEST_FILES := $(wildcard */test/*.c)
ALL_PROJECTS := $(sort $(foreach f, $(TEST_FILES), $(firstword $(subst /, ,$(f)))))

ifeq ($(PROJECT),)
    PROJECTS := $(ALL_PROJECTS)
else
    PROJECTS := $(PROJECT)
endif

ALL_INCLUDE_DIRS := $(foreach proj,$(ALL_PROJECTS),-I$(proj)/include)
ALL_SRCS := $(foreach proj,$(ALL_PROJECTS),$(wildcard $(proj)/src/$(proj).c))

UTILS_INCLUDE = -I ../utils/include
TEST_UTILS_INCLUDE = -I ../utils/test
UTILS_SRC := ../utils/src/utils.c

DEBUG_SO := lib/liball_debug.so
RELEASE_SO := lib/liball_release.so
TEST_EXES := $(foreach proj,$(PROJECTS),$(proj)/bin/debug/$(proj).out)
RELEASE_TEST_EXES := $(foreach proj,$(PROJECTS),$(proj)/bin/release/$(proj).out)

LDLIBS := -Llib -lall_debug

all: debug release

debug: CFLAGS += -g
debug: $(DEBUG_SO)

release: CFLAGS += -O3 -DNDEBUG
release: $(RELEASE_SO)

test: $(TEST_EXES)

release_test: release $(RELEASE_TEST_EXES)

lib/liball_debug.so: $(ALL_SRCS) $(UTILS_SRC)
	@mkdir -p lib
	@$(CC) $(CFLAGS) -g -shared -o $@ $^ $(ALL_INCLUDE_DIRS) $(UTILS_INCLUDE)

lib/liball_release.so: $(ALL_SRCS) $(UTILS_SRC)
	@mkdir -p lib
	@$(CC) $(CFLAGS) -O3 -DNDEBUG -shared -o $@ $^ $(ALL_INCLUDE_DIRS) $(UTILS_INCLUDE)

define make_test_rules
$(1)/bin/debug/$(1).out: $(1)/test/*.c lib/liball_debug.so
	@mkdir -p $(1)/bin/debug
	@$(CC) $(CFLAGS) -g -o $$@ $$< $(ALL_INCLUDE_DIRS) $(TEST_UTILS_INCLUDE) $(LDLIBS)

$(1)/bin/release/$(1).out: $(1)/test/*.c lib/liball_release.so
	@mkdir -p $(1)/bin/release
	@$(CC) $(CFLAGS) -O3 -DNDEBUG -o $$@ $$< $(ALL_INCLUDE_DIRS) $(TEST_UTILS_INCLUDE) -Llib -lall_release
endef

$(foreach proj,$(PROJECTS),$(eval $(call make_test_rules,$(proj))))

run: debug test
	@for exe in $(TEST_EXES); do \
		echo "Running $$exe"; \
		LD_LIBRARY_PATH=lib $$exe || exit $$?; \
	done

run_release: release_test
	@for exe in $(RELEASE_TEST_EXES); do \
		echo "Running $$exe"; \
		LD_LIBRARY_PATH=lib $$exe || exit $$?; \
	done

vlg: test
	@for exe in $(TEST_EXES); do \
		echo "Valgrind $$exe"; \
		LD_LIBRARY_PATH=lib $(VLG) $$exe || exit $$?; \
	done

gdb:
ifeq ($(PROJECT),)
	@$(error You must specify a project: make gdb PROJECT=your_project)
endif
	@exe=$(PROJECT)/bin/debug/$(PROJECT).out; \
	if [ ! -f $$exe ]; then echo "Executable not found. Build it with: make debug PROJECT=$(PROJECT)"; exit 1; fi; \
	echo "Starting GDB on $$exe"; \
	LD_LIBRARY_PATH=lib gdb $$exe

clean:
	@for d in $(ALL_PROJECTS); do \
		rm -f $$d/bin/debug/*.out $$d/bin/release/*.out; \
	done
	@rm -f lib/liball_debug.so lib/liball_release.so

.PHONY: all debug release test release_test clean run run_release vlg gdb
