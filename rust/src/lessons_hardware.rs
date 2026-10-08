//! Module 11 - Rust meets hardware.
//!
//! Registers are memory, so everything here is the raw-pointer material of
//! module 7 aimed at a chip. This PC has no registers to drive, so the demo
//! runs against a struct in RAM; what is shown of the Raspberry Pi Pico's CPU
//! is the assembly rustc really produces for it, read by check-embedded.py
//! from a compile for thumbv6m-none-eabi. Compiled, not run.

use crate::ui::{challenge, clear_screen, exercise, heading, question, say, title, wait_enter};
use std::mem::offset_of;

// The same source file compiled here as an ordinary module and, by
// hardware/thumb.rs, into a #![no_std] library for the Pico's CPU.
// two_plain_writes and two_volatile_writes are only quoted here, for their
// Thumb output, never called, hence the allow.
#[path = "../hardware/regs.rs"]
#[allow(dead_code)]
mod regs;

const REGS_SOURCE: &str = include_str!("../hardware/regs.rs");

/// Print one function of regs.rs, from its signature to its closing brace.
fn show_function(name: &str) {
    let signature = format!("pub unsafe fn {name}(");
    let mut printing = false;
    for line in REGS_SOURCE.lines() {
        if line.starts_with(&signature) {
            printing = true;
        }
        if printing {
            println!("    {line}");
            if line == "}" {
                break;
            }
        }
    }
}

// A register block as a C layout, one 32-bit word after another. On a real
// chip these words live at fixed addresses; here they are ordinary memory.
#[allow(dead_code)]
#[repr(C)]
struct FakeGpio {
    out: u32,
    enable: u32,
    input: u32,
}

pub fn lesson_11_hardware() {
    title("MODULE 11 - RUST MEETS HARDWARE");

    heading("PART 1: a register is a word at an address");

    say("  The Arduino course wrote PORTB |= (1 << 5). The MicroPython course");
    say("  could read and write machine.mem32[address]. All of it is the same");
    say("  fact: a peripheral's registers are 32-bit words at fixed addresses,");
    say("  and writing the word drives the pin. In Rust that is module 7's raw");
    say("  pointer, and two functions that say 'this really touches memory':");
    println!();
    show_function("set_bit");
    println!();
    say("  read_volatile and write_volatile live in core::ptr. Without them");
    say("  the compiler treats the pointer as plain memory (next part).");
    println!();
    say("  A register block is a #[repr(C)] struct, one word after another:");
    println!();
    say("    #[repr(C)]");
    say("    struct FakeGpio { out: u32, enable: u32, input: u32 }");
    println!();
    println!(
        "  Running:  offsets  out {}, enable {}, input {}",
        offset_of!(FakeGpio, out),
        offset_of!(FakeGpio, enable),
        offset_of!(FakeGpio, input)
    );
    let mut gpio = FakeGpio { out: 0, enable: 0, input: 0 };
    let out_register = &raw mut gpio.out;
    unsafe {
        regs::set_bit(out_register, 25);
        regs::set_bit(out_register, 3);
        regs::clear_bit(out_register, 3);
    }
    say("    set_bit(out, 25);  set_bit(out, 3);  clear_bit(out, 3);");
    println!("  Running:  out = {:032b}", gpio.out);
    println!();
    say("  Only bit 25 is left. This is a struct in RAM standing in for the");
    say("  chip: nothing here touches a pin. On the Pico the pointer would be");
    say("  the register's address from the datasheet, and bit 25 would be a");
    say("  pin, the one the on-board LED uses.");

    wait_enter();
    clear_screen();
    heading("PART 2: what volatile changes, in the CPU's own instructions");

    say("  Two functions that write the same register twice:");
    println!();
    show_function("two_plain_writes");
    println!();
    show_function("two_volatile_writes");
    println!();
    say("  rustc compiles them for the Pico's CPU, an Arm Cortex-M0+, which");
    say("  Rust calls the thumbv6m-none-eabi target. The instructions it");
    say("  produced (r0 holds the address):");
    println!();
    say("    two_plain_writes          two_volatile_writes");
    say("      movs r1, #2               movs r1, #1");
    say("      str  r1, [r0]             str  r1, [r0]");
    say("                                movs r1, #2");
    say("                                str  r1, [r0]");
    println!();
    say("  The plain version has ONE store. The compiler saw that nothing");
    say("  reads the first value before the second overwrites it, and deleted");
    say("  it. For memory that is correct. For a register where writing 1 then");
    say("  2 means 'start' then 'go', it is a bug. The volatile version keeps");
    say("  both, in order. It is the Rust spelling of the volatile you met in");
    say("  the Arduino course's interrupt module.");
    println!();
    say("  And set_bit, compiled for the same CPU:");
    println!();
    say("      ldr  r2, [r0]        read the whole register");
    say("      orrs r2, r1          set the bit in the copy");
    say("      str  r2, [r0]        write the whole register back");
    println!();
    say("  Three instructions, so an interrupt can land between the read and");
    say("  the write. If its handler changes the same register, this code then");
    say("  writes back the old value and the handler's change is lost: the");
    say("  race of the Arduino and MicroPython interrupt modules.");

    wait_enter();
    clear_screen();
    heading("PART 3: atomic aliases, no_std, and where to go next");

    say("  The RP2040 answers that race in hardware. For most peripheral");
    say("  registers, the same register is also reachable at three more");
    say("  addresses (RP2040 datasheet, section 2.1.2, 'Atomic Register");
    say("  Access'): add 0x1000 to the address to XOR the bits you write,");
    say("  0x2000 to SET them, 0x3000 to CLEAR them. One write, no read-");
    say("  modify-write, nothing for an interrupt to land inside. The SIO");
    say("  block, which holds the fast GPIO, has set and clear registers of");
    say("  its own instead.");
    println!();
    say("  A program for a chip like this has no operating system, so it is");
    say("  #![no_std]: only the `core` part of the standard library, with no");
    say("  heap by default, no println!, no files, and a panic handler you");
    say("  provide. regs.rs uses core only, which is how this same file");
    say("  compiles on a PC and for the Pico. The target is installed with:");
    println!();
    say("    rustup target add thumbv6m-none-eabi");
    say("    rustc --target thumbv6m-none-eabi --crate-type lib hardware/thumb.rs");
    println!();
    say("  In practice nobody writes the register pointers by hand. The");
    say("  rp-hal project provides the RP2040 drivers, rp-pico the board");
    say("  definition for the Pico, and cortex-m-rt the start-up code, so");
    say("  a pin is a safe type whose methods do the volatile writes inside.");
    say("  That layering is this module's idea, packaged: unsafe in a small");
    say("  library, safe code above it.");
    println!();
    say("  Honest scope: this course cannot flash a board. For RoboCup-style");
    say("  work, the Arduino and MicroPython courses are the practical path.");
    say("  This module is the bridge for when Rust is the language.");

    wait_enter();
    clear_screen();
    exercise(11);

    question(
        "Which core::ptr function writes a register so the compiler cannot\n  drop or merge the write? (the name)",
        "write_volatile",
        "A plain store may be deleted when nothing reads it; a volatile one may not.",
    );
    question(
        "On the RP2040, what offset added to a register's address gives the\n  atomic SET alias? (hex, like 0x1000)",
        "0x2000",
        "XOR is +0x1000, SET is +0x2000, CLR is +0x3000 (RP2040 datasheet 2.1.2).",
    );

    challenge(
        &[
            "Write unsafe fn set_bit(register: *mut u32, bit: u32) that reads the",
            "register with read_volatile and writes it back with bit `bit` set,",
            "using write_volatile. Start with let mut register: u32 = 0;, set bits 3",
            "and 5, and print the register with {:08b}.",
        ],
        &[],
        &["00101000"],
        &[
            "use std::ptr::{read_volatile, write_volatile};",
            "",
            "unsafe fn set_bit(register: *mut u32, bit: u32) {",
            "    let value = read_volatile(register);",
            "    write_volatile(register, value | (1 << bit));",
            "}",
            "",
            "fn main() {",
            "    let mut register: u32 = 0;",
            "    unsafe {",
            "        set_bit(&mut register, 3);",
            "        set_bit(&mut register, 5);",
            "    }",
            "    println!(\"{:08b}\", register);",
            "}",
        ],
    );

    wait_enter();
    clear_screen();
    heading("SUMMARY");

    say("   - a register is a 32-bit word at an address; Rust reaches it with");
    say("     raw pointers and read_volatile / write_volatile");
    say("   - volatile stops the compiler deleting or merging register writes");
    say("   - set_bit is read, modify, write: three instructions, not atomic");
    say("   - RP2040 atomic aliases: +0x1000 XOR, +0x2000 SET, +0x3000 CLR");
    say("   - #![no_std] code uses core only; rp-hal wraps this unsafe in safe types");
    println!();
    say("  Module 12: writing the CPU's own instructions inside Rust.");
    wait_enter();
}
