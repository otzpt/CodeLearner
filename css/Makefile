# CSS course
#
#   make          builds ./css-course
#   make run      builds and runs
#   make clean    removes build output
#
# Plain C with no dependencies, same as gui/. It teaches CSS and does
# not open a browser.

CC      = cc
CFLAGS  = -std=c11 -Wall -Wextra -g
TARGET  = css-course
SOURCES = src/main.c src/ui.c src/lessons_css.c
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
