# Universal launcher
#
#   make          builds ./launcher
#   make run      builds and runs
#   make clean    removes build output

CC      = cc
CFLAGS  = -std=c11 -Wall -Wextra -g
TARGET  = launcher
SOURCES = src/main.c
OBJECTS = $(SOURCES:.c=.o)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: run clean
