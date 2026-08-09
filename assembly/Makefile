# Assembly course
#
#   make          builds ./asm-course
#   make run      builds and runs
#   make clean    removes build output
#
# No cc, no C runtime -- as assembles, ld links directly. There is no libc
# here on purpose: every syscall in this course is made by hand.

AS      = as
LD      = ld
TARGET  = asm-course
SOURCES = src/ui.s src/lessons_asm.s src/main.s
OBJECTS = $(SOURCES:.s=.o)

$(TARGET): $(OBJECTS)
	$(LD) $(OBJECTS) -o $@

%.o: %.s
	$(AS) $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: run clean
