# Rust course
#
#   make          builds ./rust-course
#   make run      builds and runs
#   make asm-demo builds and runs the NASM example in asm/ (needs nasm)
#   make clean    removes build output
#
# Plain rustc, no Cargo: the course is a handful of files with `mod`
# declarations, and rustc follows those by itself. Warnings stay on (rustc's
# default); the course builds with none.

RUSTC      = rustc
RUSTFLAGS  = --edition 2021
TARGET     = rust-course
SOURCES    = src/main.rs src/ui.rs src/lessons_basics.rs src/lessons_memory.rs src/lessons_advanced.rs

$(TARGET): $(SOURCES)
	$(RUSTC) $(RUSTFLAGS) -o $@ src/main.rs

run: $(TARGET)
	./$(TARGET)

# Module 8's NASM example. Separate from the course on purpose: the course
# builds with rustc alone, and only this target needs nasm. The course runs
# asm/use_asm itself, when it exists, to show its real output.
asm-demo:
	nasm -f elf64 asm/add.asm -o asm/add.o
	$(RUSTC) $(RUSTFLAGS) asm/use_asm.rs -C link-arg=asm/add.o -o asm/use_asm
	./asm/use_asm

clean:
	rm -f $(TARGET) asm/add.o asm/use_asm

.PHONY: run asm-demo clean
