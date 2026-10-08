# Rust course

Eight modules, written in Rust: `println!` and variables, control flow and
functions, then ownership, borrowing, memory (stack, heap, raw pointers),
and inline assembly with NASM. It stays on the basics on purpose: no structs
or traits yet (see the root `ROADMAP.md`).

## Running

```bash
cd rust
make
./rust-course
```

Needs `rustc` only. Plain `rustc`, no Cargo.

Module 8 runs `asm!` blocks for real on x86-64. Its NASM example is built
separately, so the course itself needs no assembler:

```bash
make asm-demo         # needs nasm; builds and runs asm/use_asm
```

Once built, module 8 runs `asm/use_asm` and prints its real output.

## How it was verified

```bash
python3 check-course.py       # every module runs to its SUMMARY, no panics
python3 check-solutions.py    # every challenge solution builds warning-free and prints what it promises
python3 ../tools/check-teaching-order.py
```

Every compiler message and panic quoted in the lessons was copied from
running `rustc` (1.98.1) on the lines shown, not written from memory.
