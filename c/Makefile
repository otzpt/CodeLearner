# C course
#
#   make          builds ./c-course
#   make run      builds and runs
#   make clean    removes build output
#
# -Wall -Wextra stay on always. In a C course a compiler warning is
# teaching material: nearly all of them catch a real bug before it happens.

CC      = cc
CFLAGS  = -std=c11 -Wall -Wextra -g
TARGET  = c-course
SOURCES = src/main.c src/ui.c src/lessons_basics.c src/lessons_memory.c
OBJECTS = $(SOURCES:.c=.o)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS)

# Rebuild a .o when its .c or any header changes. Without the header
# dependency, editing ui.h would rebuild nothing and leave a stale binary.
%.o: %.c src/ui.h src/lessons.h
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: run clean
