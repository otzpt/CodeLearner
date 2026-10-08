#!/usr/bin/env python3
"""Check, with the real AVR compiler, what lesson 12 and lesson 2 say about it.

The lessons quote avr-gcc output and Uno sizes. Quoted output goes stale and
a size claimed from memory is a guess, so this recompiles everything in this
folder and fails if any of it stopped being true.

    python3 check-avr.py

Needs avr-gcc and avr-objdump. Exits 0 when every check holds.
"""

import os
import shutil
import subprocess
import sys
import tempfile

HERE = os.path.dirname(os.path.abspath(__file__))
MCU = "-mmcu=atmega328p"


def run(*command):
    done = subprocess.run(command, cwd=HERE, capture_output=True, text=True)
    if done.returncode != 0:
        sys.exit(f"FAILED: {' '.join(command)}\n{done.stderr}")
    return done.stdout


def main():
    for tool in ("avr-gcc", "avr-objdump"):
        if shutil.which(tool) is None:
            sys.exit(f"{tool} not found (Ubuntu: sudo apt install gcc-avr avr-libc)")

    failures = []

    with tempfile.TemporaryDirectory() as scratch:
        # Sizes: a static_assert in sizes.cpp is the check.
        run("avr-gcc", MCU, "-std=gnu++11", "-c", "sizes.cpp", "-o", f"{scratch}/s.o")
        print("ok   sizes.cpp: int 2, long 4, float 4, double 4, pointer 2 bytes")

        # The instructions lesson 12 quotes.
        run("avr-gcc", MCU, "-Os", "-c", "snippets.cpp", "-o", f"{scratch}/n.o")
        listing = run("avr-objdump", "-d", f"{scratch}/n.o")
        for label, needle in (
            ("PORTB |= (1 << 5) is one sbi", "sbi\t0x05, 5"),
            ("inline swap is a swap instruction", "swap\tr24"),
            ("reading SREG is an in from 0x3f", "in\tr24, 0x3f"),
            ("critical section has a cli", "cli"),
            ("critical section restores SREG with out", "out\t0x3f, r25"),
        ):
            ok = needle in listing
            print(f"{'ok  ' if ok else 'FAIL'} snippets.cpp: {label}")
            if not ok:
                failures.append(label)

        # The plain C version must come out as the same two instructions as
        # the inline asm one: that is the lesson's point.
        swaps = [line for line in listing.splitlines() if "swap\tr24" in line]
        ok = len(swaps) == 2
        print(f"{'ok  ' if ok else 'FAIL'} snippets.cpp: C rotate and asm swap compile identically")
        if not ok:
            failures.append("swap equivalence")

        # A .S file linked with C++.
        run("avr-gcc", MCU, "-Os", "-o", f"{scratch}/u.elf", "use_swap.cpp", "swap_nibbles.S")
        listing = run("avr-objdump", "-d", f"{scratch}/u.elf")
        ok = "swap_nibbles" in listing and "call" in listing
        print(f"{'ok  ' if ok else 'FAIL'} swap_nibbles.S links and is called from C++")
        if not ok:
            failures.append("S file")

    sys.exit(1 if failures else 0)


if __name__ == "__main__":
    main()
