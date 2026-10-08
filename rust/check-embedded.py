#!/usr/bin/env python3
"""Compile hardware/regs.rs for the Raspberry Pi Pico's CPU and check the
instructions module 11 quotes.

The Pico's RP2040 has Arm Cortex-M0+ cores, which Rust calls the
thumbv6m-none-eabi target. This cannot RUN that code on a PC; it compiles it
and reads the assembly rustc produces, which is what the lesson shows.

    rustup target add thumbv6m-none-eabi     # once
    python3 check-embedded.py

Exits 0 when every quoted instruction is really in the compiler's output.
"""

import os
import re
import subprocess
import sys
import tempfile

HERE = os.path.dirname(os.path.abspath(__file__))
TARGET = "thumbv6m-none-eabi"


def installed_targets():
    done = subprocess.run(["rustup", "target", "list", "--installed"],
                          capture_output=True, text=True)
    return done.stdout.split()


def functions(assembly):
    """Map function name -> its list of instruction lines."""
    result = {}
    name = None
    for line in assembly.splitlines():
        label = re.match(r"^_R.*\d([a-z_]+):", line)
        if label:
            name = label.group(1)
            result[name] = []
        elif line.startswith(".Lfunc_end"):
            name = None
        elif name and line.startswith("\t") and not line.strip().startswith("."):
            result[name].append(line.strip())
    return result


def main():
    if TARGET not in installed_targets():
        sys.exit("target missing. Install it with: rustup target add " + TARGET)

    with tempfile.TemporaryDirectory() as scratch:
        out = os.path.join(scratch, "regs.s")
        done = subprocess.run(
            ["rustc", "--edition", "2021", "--target", TARGET, "--crate-type", "lib",
             "-C", "opt-level=s", "-C", "debuginfo=0", "--emit", "asm", "-o", out,
             os.path.join(HERE, "hardware", "thumb.rs")],
            capture_output=True, text=True)
        if done.returncode != 0 or done.stderr.strip():
            sys.exit("compile problem:\n" + done.stderr)
        listing = functions(open(out).read())

    def opcodes(name):
        return [text.split()[0] for text in listing.get(name, [])]

    checks = [
        ("set_bit reads, ORs, then writes", [o for o in opcodes("set_bit") if o in ("ldr", "orrs", "str")] == ["ldr", "orrs", "str"]),
        ("clear_bit reads, clears with bics, then writes", [o for o in opcodes("clear_bit") if o in ("ldr", "bics", "str")] == ["ldr", "bics", "str"]),
        ("two plain writes keep ONE store", opcodes("two_plain_writes").count("str") == 1),
        ("two volatile writes keep BOTH stores", opcodes("two_volatile_writes").count("str") == 2),
    ]

    failed = 0
    for label, ok in checks:
        print("{}  {}".format("ok  " if ok else "FAIL", label))
        failed += 0 if ok else 1
    if failed:
        print("functions found:", sorted(listing))
    sys.exit(1 if failed else 0)


if __name__ == "__main__":
    main()
