# Assembly course

Sixteen modules of x86-64 assembly in NASM's Intel syntax, from registers and
syscalls to a kernel written in C that handles interrupts. Linux only: the
first nine modules are built on Linux system calls, and the rest boot a PC.
The module list is in the [root README](../README.md#the-assembly-course).

## Run it

```sh
make            # nasm + ld; no cc, no libc
./asm-course
```

Needs `nasm` and `ld`. Modules 9, 15 and 16 also use `gcc` for the C they show;
modules 10 to 16 are meant to be tried with `qemu-system-x86_64`.

## What is where

| Path | What |
| --- | --- |
| `src/` | the course itself. `ui.asm` and `main.asm` are the menu; `lessons_*.asm` are the modules; `course.inc` holds the few macros every lesson uses |
| `solutions/` | the example solution to each module's challenge, one file per module. The lesson prints the file (`incbin`), so what is shown is what was run |
| `examples/09/` | module 9's C and assembly examples |
| `os/` | modules 10 to 16: the boot sectors, the loader, the C kernel (`kernel/`) and the interrupt kernel (`interrupts/`), and `traps/`, the deliberately broken variants the lessons quote |
| `vm.py` | boots a disk image in QEMU and reads back the screen, serial port, memory and registers |
| `check-course.py`, `check-os.py` | the two checkers below |

## How it was verified

```sh
make check              # python3 check-course.py
python3 check-os.py     # needs qemu-system-x86_64
```

`check-course.py` starts the real binary on a pseudo-terminal and walks every
module to its SUMMARY, answering each prompt the way a person would (the
course reads with raw `read(2)`, so input has to arrive one line at a time).
It then assembles and runs every example solution and compares its output with
what the challenge promises, builds the examples under `examples/09` and
compares their output with the lines the lesson quotes, and checks every
number a lesson computes live against a value worked out in Python. The boot
challenges (modules 10 to 16) are booted in QEMU and the screen, serial port
or registers are read back.

`check-os.py` builds every image the operating-system lessons show, boots it,
and compares what the machine does with what the lesson says: the register
dumps, memory map, exception logs and serial lines quoted in the text are
asserted line for line. The broken variants in `os/traps/` are built and
booted too, so each failure the text describes was seen, not assumed. Without
QEMU it says so and checks nothing.

Run in QEMU 10.2 with SeaBIOS. **Nothing here was run on real hardware.**
Where QEMU is kinder than a real CPU, the lesson says so (module 14).

The release workflow runs `make -C assembly check`, which skips the parts
that need QEMU when it is not installed.
