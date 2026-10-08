# Rust course

Twelve modules, written in Rust: `println!` and variables, control flow and
functions; ownership, borrowing and memory (stack, heap, raw pointers);
structs, enums with `Option`, and `Result`; then Rust meeting hardware
(volatile registers, `no_std`) and inline assembly with NASM. Lifetimes,
traits, generics and async are still to come (see the root `ROADMAP.md`).

## Running

```bash
cd rust
make
./rust-course
```

Needs `rustc` only. Plain `rustc`, no Cargo.

Module 12 runs `asm!` blocks for real on x86-64. Its NASM example is built
separately, so the course itself needs no assembler:

```bash
make asm-demo         # needs nasm; builds and runs asm/use_asm
```

Once built, module 12 runs `asm/use_asm` and prints its real output.

## How it was verified

```bash
python3 check-course.py       # every module runs to its SUMMARY, no panics
python3 check-solutions.py    # every challenge solution builds warning-free and prints what it promises
python3 check-embedded.py     # module 11's Thumb listings, compiled for the Pico's CPU
python3 run-embedded.py       # the same code executed on an emulated Cortex-M0+ (Unicorn)
python3 ../tools/check-teaching-order.py
```

Every compiler message and panic quoted in the lessons was copied from
running `rustc` (1.98.1) on the lines shown, not written from memory.

`check-embedded.py` needs the Cortex-M0+ target (`rustup target add
thumbv6m-none-eabi`). It compiles `hardware/regs.rs` for the Raspberry Pi
Pico's CPU and checks the instructions module 11 quotes, for example that two
plain writes to one address keep a single store and two volatile writes keep
both. That is the listing; `run-embedded.py` (needs `pip install unicorn
pyelftools`) goes further and executes the same machine code on an emulated
Cortex-M0+: it counts the stores that reach a fake register, and it injects an
"interrupt" between `set_bit`'s read and write to show the handler's change
being overwritten. An emulator is not a Pico: only the instructions and their
memory accesses are modelled.
