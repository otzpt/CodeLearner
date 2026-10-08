// regs.rs - helpers for memory-mapped hardware registers.
//
// Used two ways from one source file:
//   - module 11 includes it as an ordinary module and runs it on this PC,
//     against a register block that is just a struct in RAM;
//   - thumb.rs includes it into a #![no_std] crate that check-embedded.py
//     compiles for the Raspberry Pi Pico's CPU (thumbv6m-none-eabi), to show
//     the real instructions.
//
// So it uses `core` only, and carries no crate-level attributes.

use core::ptr::{read_volatile, write_volatile};

/// Set one bit of a register, leaving the others alone.
///
/// # Safety
/// `register` must be a valid, aligned address of a 32-bit register.
#[inline(never)]
pub unsafe fn set_bit(register: *mut u32, bit: u32) {
    let value = read_volatile(register);
    write_volatile(register, value | (1 << bit));
}

/// Clear one bit of a register, leaving the others alone.
///
/// # Safety
/// Same as `set_bit`.
#[inline(never)]
pub unsafe fn clear_bit(register: *mut u32, bit: u32) {
    let value = read_volatile(register);
    write_volatile(register, value & !(1 << bit));
}

/// Two ordinary writes to the same address. The compiler may keep only the
/// second, because as far as it can tell nothing ever reads the first.
///
/// # Safety
/// `register` must be valid for writes.
#[inline(never)]
pub unsafe fn two_plain_writes(register: *mut u32) {
    *register = 1;
    *register = 2;
}

/// Two volatile writes: both must happen, in this order.
///
/// # Safety
/// `register` must be valid for writes.
#[inline(never)]
pub unsafe fn two_volatile_writes(register: *mut u32) {
    write_volatile(register, 1);
    write_volatile(register, 2);
}
