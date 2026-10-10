# Universal launcher
#
#   make                  builds ./launcher
#   make VERSION=1.5.1    the same, with that version shown and compared by
#                         "Check for updates" (the release workflow passes the
#                         tag; a plain make is a "dev" build). Run make clean
#                         first when changing it.
#   make run              builds and runs
#   make check            builds and runs the tests for src/update.c
#   make clean            removes build output

CC      = cc
VERSION ?= dev
CFLAGS  = -std=c11 -Wall -Wextra -g -DCODELEARNER_VERSION=\"$(VERSION)\"
TARGET  = launcher
SOURCES = src/main.c src/update.c
OBJECTS = $(SOURCES:.c=.o)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

check: test_update
	./test_update

test_update: test_update.c src/update.c src/update.h
	$(CC) -std=c11 -Wall -Wextra -g -o $@ test_update.c src/update.c

clean:
	rm -f $(OBJECTS) $(TARGET) test_update

.PHONY: run check clean
