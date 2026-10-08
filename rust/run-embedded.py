#!/usr/bin/env python3
"""Run the Cortex-M0+ code module 11 quotes on an emulated Cortex-M0+.

check-embedded.py proves rustc compiles hardware/regs.rs to the instructions
the lesson shows. This runs them: the functions are compiled for the Pico's
CPU (thumbv6m-none-eabi), their machine code is pulled out of the object file
and executed by the Unicorn CPU emulator against a fake register at a
memory-mapped address, with every read and write recorded.

    pip install unicorn pyelftools      # dev-time tools, not course dependencies
    rustup target add thumbv6m-none-eabi
    python3 run-embedded.py

Exits 0 when the recorded accesses are what the lesson says. An emulated CPU
is not a Pico: peripherals, timing and the real interrupt controller are not
modelled, only the instructions and the memory accesses they make.
"""

import os
import subprocess
import sys
import tempfile

try:
    from elftools.elf.elffile import ELFFile
    from unicorn import (UC_ARCH_ARM, UC_HOOK_CODE, UC_HOOK_MEM_READ, UC_HOOK_MEM_WRITE,
                         UC_MODE_MCLASS, UC_MODE_THUMB, Uc)
    from unicorn.arm_const import (UC_ARM_REG_LR, UC_ARM_REG_MSP, UC_ARM_REG_R0, UC_ARM_REG_R1,
                               UC_ARM_REG_SP)
except ImportError:
    sys.exit("needs unicorn and pyelftools: pip install unicorn pyelftools")

HERE = os.path.dirname(os.path.abspath(__file__))
CODE, STACK, REGISTER = 0x08000000, 0x20000000, 0x40000000
RETURN = CODE + 0x800  # where every function "returns" to; execution stops there


def compile_functions(scratch):
    obj = os.path.join(scratch, "thumb.o")
    done = subprocess.run(
        ["rustc", "--edition", "2021", "--target", "thumbv6m-none-eabi", "--crate-type", "lib",
         "-C", "opt-level=s", "-C", "debuginfo=0", "--emit", "obj", "-o", obj,
         os.path.join(HERE, "hardware", "thumb.rs")], capture_output=True, text=True)
    if done.returncode != 0:
        sys.exit("compile failed (rustup target add thumbv6m-none-eabi?):\n" + done.stderr)

    functions = {}
    with open(obj, "rb") as handle:
        elf = ELFFile(handle)
        for symbol in elf.get_section_by_name(".symtab").iter_symbols():
            if symbol["st_info"]["type"] == "STT_FUNC":
                # Bit 0 of a Thumb function's symbol value marks it as Thumb code.
                # It is not part of the address.
                start = symbol["st_value"] & ~1
                data = elf.get_section(symbol["st_shndx"]).data()[start:start + symbol["st_size"]]
                for name in ("set_bit", "clear_bit", "two_plain_writes", "two_volatile_writes"):
                    if symbol.name.endswith(name):
                        functions[name] = data
    return functions


def run(code, register_value, bit, interrupt_value=None):
    """Execute one function. Returns (reads, writes, final register value).

    If `interrupt_value` is given, an "interrupt" overwrites the register with
    it right after the first read, the way a handler running between the ldr
    and the str would."""
    emulator = Uc(UC_ARCH_ARM, UC_MODE_THUMB | UC_MODE_MCLASS)
    emulator.mem_map(CODE, 0x1000)
    emulator.mem_map(STACK, 0x1000)
    emulator.mem_map(REGISTER, 0x1000)
    emulator.mem_write(CODE, code)
    emulator.mem_write(REGISTER, register_value.to_bytes(4, "little"))
    emulator.reg_write(UC_ARM_REG_R0, REGISTER)
    emulator.reg_write(UC_ARM_REG_R1, bit)
    # An M-profile core keeps its main stack pointer in a banked register.
    emulator.reg_write(UC_ARM_REG_MSP, STACK + 0x800)
    emulator.reg_write(UC_ARM_REG_SP, STACK + 0x800)
    emulator.reg_write(UC_ARM_REG_LR, RETURN | 1)  # bit 0 set: stay in Thumb state

    reads, writes, state = [], [], {"read": False, "fired": False}

    def on_read(uc, access, address, size, value, user):
        reads.append(address)
        state["read"] = True

    def on_write(uc, access, address, size, value, user):
        writes.append(value & 0xFFFFFFFF)

    def on_code(uc, address, size, user):
        if interrupt_value is not None and state["read"] and not state["fired"]:
            state["fired"] = True
            uc.mem_write(REGISTER, interrupt_value.to_bytes(4, "little"))

    emulator.hook_add(UC_HOOK_MEM_READ, on_read, begin=REGISTER, end=REGISTER + 0xFFF)
    emulator.hook_add(UC_HOOK_MEM_WRITE, on_write, begin=REGISTER, end=REGISTER + 0xFFF)
    emulator.hook_add(UC_HOOK_CODE, on_code)
    emulator.emu_start(CODE | 1, RETURN, count=200)
    return reads, writes, int.from_bytes(emulator.mem_read(REGISTER, 4), "little")


def main():
    with tempfile.TemporaryDirectory() as scratch:
        functions = compile_functions(scratch)
    missing = {"set_bit", "clear_bit", "two_plain_writes", "two_volatile_writes"} - set(functions)
    if missing:
        sys.exit("functions not found in the object file: " + ", ".join(sorted(missing)))

    reads, writes, final = run(functions["set_bit"], 0b0100, 5)
    set_ok = (len(reads), writes, final) == (1, [0b100100], 0b100100)
    reads, writes, final = run(functions["clear_bit"], 0xFFFFFFFF, 3)
    clear_ok = (len(reads), writes, final) == (1, [0xFFFFFFF7], 0xFFFFFFF7)
    _, writes, _ = run(functions["two_plain_writes"], 0, 0)
    plain_ok = writes == [2]
    _, writes, _ = run(functions["two_volatile_writes"], 0, 0)
    volatile_ok = writes == [1, 2]
    # The register holds bit 2. An interrupt handler sets bit 0 between
    # set_bit's read and write; set_bit then writes back its stale copy.
    _, writes, final = run(functions["set_bit"], 0b0100, 5, interrupt_value=0b0101)
    lost_ok = final == 0b100100 and final & 1 == 0

    checks = [
        ("set_bit(5) on 0b100 reads once, writes 0b100100", set_ok),
        ("clear_bit(3) on all ones leaves 0xFFFFFFF7", clear_ok),
        ("two plain writes: ONE store reaches the register, the value 2", plain_ok),
        ("two volatile writes: BOTH stores reach it, 1 then 2", volatile_ok),
        ("an interrupt between set_bit's read and write is lost (bit 0 gone)", lost_ok),
    ]
    failed = 0
    for label, ok in checks:
        print("{}  {}".format("ok  " if ok else "FAIL", label))
        failed += 0 if ok else 1
    sys.exit(1 if failed else 0)


if __name__ == "__main__":
    main()
