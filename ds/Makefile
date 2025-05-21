CC = gcc
CFLAGS = -ansi -pedantic-errors -Wall -Wextra -fPIC
DEBUG_FLAGS = -g -DDEBUG
RELEASE_FLAGS = -O3 -DNDEBUG
VLG = valgrind --leak-check=yes --track-origins=yes

CFLAGS_DEBUG = $(CFLAGS) $(DEBUG_FLAGS)
CFLAGS_RELEASE = $(CFLAGS) $(RELEASE_FLAGS)

PROJECT ?=

SRC_FILES := $(wildcard */src/*.c)
ALL_PROJECTS := $(sort $(foreach f, $(SRC_FILES),$(firstword $(subst /, ,$(f)))))

ifeq ($(PROJECT),)
	PROJECTS := $(ALL_PROJECTS)
else
	PROJECTS := $(PROJECT)
endif

ALL_INCLUDE_DIRS := $(foreach proj,$(ALL_PROJECTS),-I$(proj)/include)
ALL_SRCS := $(foreach proj,$(ALL_PROJECTS),$(wildcard $(proj)/src/*.c))

OBJ_DEBUG_FILES := $(foreach f,$(SRC_FILES),$(patsubst %/src/%.c,%/bin/debug/%_debug.o,$(f)))
DEP_DEBUG_FILES := $(OBJ_DEBUG_FILES:.o=.d)

OBJ_RELEASE_FILES := $(foreach f,$(SRC_FILES),$(patsubst %/src/%.c,%/bin/release/%_release.o,$(f)))
DEP_RELEASE_FILES := $(OBJ_RELEASE_FILES:.o=.d)

DEBUG_SO := lib/libds_debug.so
RELEASE_SO := lib/libds_release.so
DEBUG_EXES := $(foreach proj,$(PROJECTS),$(proj)/bin/debug/$(proj)_debug.out)
RELEASE_EXES := $(foreach proj,$(PROJECTS),$(proj)/bin/release/$(proj)_release.out)

LDLIBS_DEBUG := -Llib -lds_debug
LDLIBS_RELEASE := -Llib -lds_release

.PHONY: all debug release run run_release vlg gdb clean

all: debug release

debug: CFLAGS_DEBUG
debug: $(OBJ_DEBUG_FILES) $(DEBUG_SO) $(DEBUG_EXES)

release: CFLAGS_RELEASE
release: $(OBJ_RELEASE_FILES) $(RELEASE_SO) $(RELEASE_EXES)

lib/libds_debug.so: $(OBJ_DEBUG_FILES)
	@$(CC) -shared -o $@ $^

lib/libds_release.so: $(OBJ_RELEASE_FILES)
	@$(CC) -shared -o $@ $^

%/bin/debug/%_debug.o: %/src/%.c
	@$(CC) $(CFLAGS_DEBUG) $(ALL_INCLUDE_DIRS) -MMD -MP -c $< -o $@

%/bin/release/%_release.o: %/src/%.c
	@$(CC) $(CFLAGS_RELEASE) $(ALL_INCLUDE_DIRS) -MMD -MP -c $< -o $@

define make_exe_rules
$(1)/bin/debug/$(1)_debug.out: $(1)/test/$(1)_test.c lib/libds_debug.so
	@$(CC) $(CFLAGS_DEBUG) -o $$@ $$< $(ALL_INCLUDE_DIRS) $(LDLIBS_DEBUG)

$(1)/bin/release/$(1)_release.out: $(1)/test/$(1)_test.c lib/libds_release.so
	@$(CC) $(CFLAGS_RELEASE) -o $$@ $$< $(ALL_INCLUDE_DIRS) $(LDLIBS_RELEASE)
endef

$(foreach proj,$(ALL_PROJECTS),$(eval $(call make_exe_rules,$(proj))))

run: debug
	@echo "DEBUG MODE"
	@for exe in $(DEBUG_EXES); do \
		echo "Running $$exe"; \
		LD_LIBRARY_PATH=lib $$exe || exit $$?; \
	done

run_release: release
	@echo "RELEASE MODE"
	@for exe in $(RELEASE_EXES); do \
		echo "Running $$exe"; \
		LD_LIBRARY_PATH=lib $$exe || exit $$?; \
	done

vlg: debug
	@echo "Valgrind Mode"
	@for exe in $(DEBUG_EXES); do \
		echo "Valgrind $$exe"; \
		LD_LIBRARY_PATH=lib $(VLG) $$exe || exit $$?; \
	done

gdb:
ifeq ($(PROJECT),)
	$(error You must specify a project: make gdb PROJECT=your_project)
endif
	@exe=$(PROJECT)/bin/debug/$(PROJECT)_debug.out; \
	if [ ! -f $$exe ]; then echo "Executable not found. Build it with: make debug PROJECT=$(PROJECT)"; exit 1; fi; \
	echo "Starting GDB on $$exe"; \
	LD_LIBRARY_PATH=lib gdb $$exe

clean:
	@for d in $(ALL_PROJECTS); do \
		rm -f $$d/bin/debug/*.out $$d/bin/debug/*.o $$d/bin/debug/*.d; \
		rm -f $$d/bin/release/*.out $$d/bin/release/*.o $$d/bin/release/*.d; \
	done
	@rm -f lib/libds_debug.so lib/libds_release.so

-include $(DEP_DEBUG_FILES)
-include $(DEP_RELEASE_FILES)
