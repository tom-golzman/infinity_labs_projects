CC = gcc

SRC_DIR = src
TEST_DIR = test
INC_DIR = include

SRC = $(SRC_DIR)/foo.c
MAIN = $(TEST_DIR)/main.c
INC = $(INC_DIR)/foo.h

TARGET = fooer.out
OBJS = foo.o main.o

CFLAGS = -Wall -Wextra -g
CPPFLAGS = -I$(INC_DIR)

.PHONY: all clean debug release

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^
	
foo.o: $(SRC) $(INC)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@
	
main.o: $(MAIN)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@
	
libfoo.ar: foo.o
	ar rcs $@ $^
	
debug: CFLAGS += -g
debug: all

release: CFLAGS += -O2
release: all

clean:
	rm -f $(OBJS) $(TARGET)
