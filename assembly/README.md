# Assembly course

x86-64 assembly in NASM's Intel syntax, for someone who has never
programmed. Linux only: the programs talk to Linux through system calls.

## Lessons

1. Your first program
2. Registers and arithmetic
3. Decisions: compare and jump
4. Loops
5. Memory: variables and arrays
6. The stack: push and pop
7. Functions: call and ret
8. Text, one byte at a time
9. Next: below the operating system

## Running the examples yourself

```bash
nasm -f elf64 -o program.o main.asm && ld -o program program.o && ./program
```

Needs `nasm` and `ld`. From lesson 2 on, a ready-made `print_int` routine
(printing a number is a small program in itself) sits above each example; it
is in every practice file the course gives you.

## Below the operating system

Lesson 9 shows a boot sector and points here. The files from the earlier,
longer course go on from there, and are kept and checked:

| Path | What |
| --- | --- |
| `os/` | boot sectors, a two-stage loader, protected and long mode, a kernel in C (`kernel/`), interrupts (`interrupts/`), and `traps/`, deliberately broken variants |
| `solutions/`, `examples/09/` | example programs those files build on |
| `vm.py` | boots a disk image in QEMU and reads back the screen, serial port, memory and registers |
| `check-os.py` | builds every image, boots it in QEMU, and compares what the machine does with what the text says |

`python3 check-os.py` needs `qemu-system-x86_64`. On 2026-11-14 (GCC 16.2,
QEMU 11.1) 31 of its checks pass and 2 fail: module 15's volatile store
listing and module 16's interrupt kernel. The same files fail the same way
on `main`, so the toolchain changed since they last passed (2026-10-17); not
investigated yet. **Nothing here was run on real hardware.**

## Running

From the launcher, or directly:

```bash
node assembly/learn.js
```

Your practice files and progress go in `~/codelearner/assembly/`.

## Checking

```bash
node learn/check.js assembly            # every example, question, solution and starter
python3 learn/check-course.py assembly  # every lesson played to the end
```

