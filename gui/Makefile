# GUI course
#
#   make          builds ./gui-course
#   make run      builds and runs
#   make clean    removes build output
#
# The binary this Makefile builds is plain C with zero dependencies, same
# as every other course -- it teaches GTK, it does not link against it. See
# src/lessons_gtk.c's header comment for why, and what actually needs GTK4
# installed (the example code the student compiles separately).

CC      = cc
CFLAGS  = -std=c11 -Wall -Wextra -g
TARGET  = gui-course
SOURCES = src/main.c src/ui.c src/lessons_gtk.c
OBJECTS = $(SOURCES:.c=.o)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS)

%.o: %.c src/ui.h src/lessons.h
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: run clean
