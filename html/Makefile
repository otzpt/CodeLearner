# HTML course
#
#   make          builds ./html-course
#   make run      builds and runs
#   make clean    removes build output
#
# Plain C with no dependencies, same as gui/. It teaches HTML and does
# not open a browser.

CC      = cc
CFLAGS  = -std=c11 -Wall -Wextra -g
TARGET  = html-course
SOURCES = src/main.c src/ui.c src/strings_pt.c src/lessons_html.c
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
