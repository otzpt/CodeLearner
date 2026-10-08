// thumb.rs - regs.rs as a #![no_std] library, so it can be compiled for the
// Raspberry Pi Pico's CPU. check-embedded.py builds this file; nothing else
// uses it.

#![no_std]

include!("regs.rs");
