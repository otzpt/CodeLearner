# raylib course
#
#   make          builds ./raylib-course
#   make run      builds and runs
#   make clean    removes build output
#
# The binary this Makefile builds is plain C with zero dependencies, same
# as every other course -- it teaches raylib and raygui, it does not link
# against them. See src/lessons_raylib.c's header comment, and module 1 for
# what the student installs to compile the example code separately.

CC      = cc
CFLAGS  = -std=c11 -Wall -Wextra -g
TARGET  = raylib-course
SOURCES = src/main.c src/ui.c src/strings_pt.c src/lessons_raylib.c
OBJECTS = $(SOURCES:.c=.o)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS)

%.o: %.c src/ui.h src/lessons.h src/strings_pt.h
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: run clean
