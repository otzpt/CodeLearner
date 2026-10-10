# Libraries: a reference browser
#
#   make          builds ./libraries
#   make run      builds and runs
#   make clean    removes build output
#
# The browser is the only thing to build. The pages (c/*.c, python/*.py) are
# source files it compiles and runs on demand, so adding a page is dropping a
# file in the folder. Linux only: the pages read /proc and /sys.

CC      = cc
CFLAGS  = -std=c11 -Wall -Wextra -g
TARGET  = libraries

$(TARGET): src/main.c
	$(CC) $(CFLAGS) -o $@ $<

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: run clean
