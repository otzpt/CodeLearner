# C course
#
#   make          builds ./c-course
#   make run      builds and runs
#   make asm-demo builds and runs the NASM example in asm/ (needs nasm)
#   make clean    removes build output
#
# -Wall -Wextra stay on always. In a C course a compiler warning is
# teaching material: nearly all of them catch a real bug before it happens.

CC      = cc
CFLAGS  = -std=c11 -Wall -Wextra -g
TARGET  = c-course
SOURCES = src/main.c src/ui.c src/lessons_basics.c src/lessons_memory.c src/lessons_advanced.c src/lessons_tooling.c src/lessons_io.c src/lessons_lang_features.c src/lessons_assembly.c
OBJECTS = $(SOURCES:.c=.o)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS)

# Rebuild a .o when its .c or any header changes. Without the header
# dependency, editing ui.h would rebuild nothing and leave a stale binary.
%.o: %.c src/ui.h src/lessons.h
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

# Module 20's NASM example. Separate from the course on purpose: the course
# builds with a C compiler alone, and only this target needs nasm. The course
# runs asm/use_asm itself, when it exists, to show its real output.
asm-demo:
	nasm -f elf64 asm/add.asm -o asm/add.o
	$(CC) -std=c11 -Wall -Wextra asm/use_asm.c asm/add.o -o asm/use_asm
	./asm/use_asm

clean:
	rm -f $(OBJECTS) $(TARGET) asm/add.o asm/use_asm

.PHONY: run asm-demo clean
