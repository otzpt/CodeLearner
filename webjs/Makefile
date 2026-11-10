# Web JavaScript course
#
#   make          builds ./webjs-course
#   make run      builds and runs
#   make clean    removes build output
#
# Plain C with no dependencies, same as gtk/. It teaches Web JavaScript and does
# not open a browser.

CC      = cc
CFLAGS  = -std=c11 -Wall -Wextra -g
TARGET  = webjs-course
SOURCES = src/main.c src/ui.c src/strings_pt.c src/lessons_webjs.c
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
