# Assembly course
#
#   make          builds ./asm-course
#   make run      builds and runs
#   make check    runs every module and every example solution (needs python3)
#   make clean    removes build output
#
# NASM assembles (Intel syntax), ld links directly. There is no cc and no C
# runtime here on purpose: every syscall in this course is made by hand.

NASM     = nasm
LD       = ld
TARGET   = asm-course
SOURCES  = src/ui.asm src/lessons_basics.asm src/lessons_flow.asm src/lessons_input.asm src/lessons_memory.asm src/lessons_bits.asm src/lessons_c.asm src/lessons_boot.asm src/lessons_kernel.asm src/main.asm
OBJECTS  = $(SOURCES:.asm=.o)
# The lessons print the real example files with incbin, so a change to one
# of them has to rebuild the lesson that shows it.
EXAMPLES = $(wildcard solutions/*.asm examples/*/* os/*.asm os/*/* os/kernel/*.asm os/kernel/*.c os/kernel/*.ld os/interrupts/*.asm os/interrupts/*.c os/interrupts/*.h)

$(TARGET): $(OBJECTS)
	$(LD) $(OBJECTS) -o $@

$(OBJECTS): src/course.inc $(EXAMPLES)

src/%.o: src/%.asm
	$(NASM) -f elf64 -I src/ -I solutions/ -I examples/ -I os/ $< -o $@

run: $(TARGET)
	./$(TARGET)

check: $(TARGET)
	python3 check-course.py

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: run check clean
