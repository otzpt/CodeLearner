//! Module 8 - inline assembly and NASM.
//!
//! Everything here runs for real on this x86-64 PC: the asm! blocks are
//! compiled into the course itself, and the NASM example is the real file
//! (included below with include_str!) assembled and linked by `make
//! asm-demo`. On any other CPU the asm! examples are skipped and said so.

use crate::ui::{challenge, clear_screen, exercise, heading, question, say, title, wait_enter};

#[cfg(target_arch = "x86_64")]
use std::arch::asm;

/// The NASM source, read from the real file at compile time so the lesson
/// cannot drift from the code that was actually assembled.
const NASM_SOURCE: &str = include_str!("../asm/add.asm");

/// The output of asm/use_asm, if `make asm-demo` has been run.
fn nasm_demo_output() -> Option<String> {
    let exe = std::env::current_exe().ok()?;
    let demo = exe.parent()?.join("asm").join("use_asm");
    let output = std::process::Command::new(demo).output().ok()?;
    if output.status.success() {
        Some(String::from_utf8_lossy(&output.stdout).into_owned())
    } else {
        None
    }
}

#[cfg(target_arch = "x86_64")]
fn demo_arithmetic() {
    let (first, second) = (40u64, 2u64);
    let sum: u64;
    unsafe {
        asm!(
            "mov {result}, {a}",
            "add {result}, {b}",
            result = out(reg) sum,
            a = in(reg) first,
            b = in(reg) second,
            options(pure, nomem, nostack),
        );
    }
    println!("  Running:  mov, add            -> {sum}");

    let mut doubled: u64 = 21;
    unsafe {
        asm!("add {0}, {0}", inout(reg) doubled, options(nomem, nostack));
    }
    println!("  Running:  add {{0}}, {{0}}        -> {doubled}   (21 doubled, in place)");

    let seven: u64 = 7;
    let times_five: u64;
    unsafe {
        asm!(
            "lea {0}, [{1} + {1}*4]",
            out(reg) times_five,
            in(reg) seven,
            options(pure, nomem, nostack),
        );
    }
    println!("  Running:  lea [x + x*4]       -> {times_five}   (7 times 5, no multiply instruction)");
}

#[cfg(not(target_arch = "x86_64"))]
fn demo_arithmetic() {
    println!("  This CPU is not x86-64, so the asm! examples are skipped.");
}

#[cfg(target_arch = "x86_64")]
fn demo_memory() {
    let data = [10u8, 20, 30];
    let second: u32;
    unsafe {
        asm!(
            "movzx {0:e}, byte ptr [{1}]",
            out(reg) second,
            in(reg) data.as_ptr().add(1),
            options(pure, readonly, nostack),
        );
    }
    println!("  Running:  movzx from [data + 1] -> {second}");
}

#[cfg(not(target_arch = "x86_64"))]
fn demo_memory() {}

pub fn lesson_08_assembly() {
    title("MODULE 8 - INLINE ASSEMBLY AND NASM");

    heading("PART 1: asm!, the CPU's own instructions inside Rust");

    say("  Assembly is the CPU's instruction set. Rust lets you write it in");
    say("  the middle of a function with the asm! macro, from core::arch.");
    say("  Three honest reasons: an instruction the language has no word");
    say("  for, exact control of what runs, or learning what the compiler");
    say("  produces. It is always inside unsafe, because the compiler");
    say("  cannot look inside the string to check it.");
    println!();
    say("    unsafe {");
    say("        asm!(");
    say("            \"mov {result}, {a}\",");
    say("            \"add {result}, {b}\",");
    say("            result = out(reg) sum,");
    say("            a = in(reg) first,");
    say("            b = in(reg) second,");
    say("        );");
    say("    }");
    println!();
    say("  {result}, {a}, {b} are placeholders the compiler fills with");
    say("  registers it picks. out writes a variable, in reads one, inout");
    say("  does both to the same variable. On x86 the default syntax is");
    say("  Intel's: destination first, so  add rax, rbx  means rax += rbx.");
    println!();
    demo_arithmetic();
    println!();
    say("  The options tell the compiler what the asm will NOT do, so it");
    say("  can optimise around it: nomem (touches no memory), nostack (does");
    say("  not use the stack), pure (no side effects: it may be removed if");
    say("  the result is unused). Claim one that is false and the program is");
    say("  wrong in ways that depend on the optimiser.");

    wait_enter();
    clear_screen();
    heading("PART 2: memory, and what you owe the compiler");

    say("  An asm! block can read memory through a pointer:");
    println!();
    say("    asm!(\"movzx {0:e}, byte ptr [{1}]\",");
    say("         out(reg) second, in(reg) data.as_ptr().add(1));");
    println!();
    demo_memory();
    println!();
    say("  {0:e} asks for the 32-bit name of the register (eax rather than");
    say("  rax): movzx widens a byte into a 32-bit register. The out value");
    say("  is a u32 because reg does not hold a bare u8; reg_byte does.");
    println!();
    say("  Two things the compiler does not do for you here:");
    println!();
    say("    - it does not check the pointer. [rdi] is read exactly as");
    say("      written, so an address past the array is a read past the");
    say("      array, the same as a C pointer.");
    say("    - it does not know about registers the asm changes unless you");
    say("      say so. An instruction that clobbers rcx needs  out(\"rcx\") _");
    say("      in the operand list, or the compiler keeps a live value in rcx");
    say("      and gets it overwritten.");
    println!();
    say("  Rust's asm! is not GCC's. C uses AT&T syntax (source first) by");
    say("  default and a different operand format; the C course shows it.");
    say("  options(att_syntax) switches Rust to the AT&T order.");

    wait_enter();
    clear_screen();
    heading("PART 3: NASM, a whole file of assembly");

    say("  For more than a few instructions, write a separate .asm file,");
    say("  assemble it with NASM, and link it. NASM's syntax is Intel's, as");
    say("  in asm!. The file below is the real asm/add.asm of this course,");
    say("  read in when the course was built:");
    println!();
    for line in NASM_SOURCE.lines().take_while(|line| !line.starts_with("; Tells the linker")) {
        println!("    {line}");
    }
    println!();
    say("  The function names and registers follow the System V AMD64 ABI,");
    say("  the calling convention Linux uses: first integer argument in rdi,");
    say("  second in rsi, then rdx, rcx, r8, r9; the result in rax. Rust");
    say("  declares them, trusting that declaration:");
    println!();
    say("    extern \"C\" { fn add_numbers(a: u64, b: u64) -> u64; }");
    println!();
    say("  and the build is two commands (or `make asm-demo`):");
    println!();
    say("    nasm -f elf64 asm/add.asm -o asm/add.o");
    say("    rustc --edition 2021 asm/use_asm.rs -C link-arg=asm/add.o -o asm/use_asm");
    println!();
    match nasm_demo_output() {
        Some(output) => {
            say("  asm/use_asm is built, and this is what it printed just now:");
            println!();
            for line in output.lines() {
                println!("  Running:  {line}");
            }
        }
        None => {
            say("  asm/use_asm has not been built, so there is no output to show.");
            say("  Run `make asm-demo` in the rust/ folder (it needs nasm) and");
            say("  come back to this module.");
        }
    }
    println!();
    say("  Which to choose: asm! when the assembly belongs inside one Rust");
    say("  function, with the compiler picking registers; a NASM file when");
    say("  it is a routine on its own, to be tested and read as assembly.");

    wait_enter();
    clear_screen();
    exercise(8);

    question(
        "In the System V x86-64 ABI, which register holds the first integer\n  argument?",
        "rdi",
        "Then rsi, rdx, rcx, r8 and r9.",
    );
    question(
        "Which register carries the integer result back to the caller?",
        "rax",
        "A function leaves its return value in rax.",
    );

    challenge(
        &[
            "Use asm! (x86-64) with a lea instruction to compute 14 * 3 without",
            "multiplying: lea {0}, [{1} + {1}*2]. Print the result.",
        ],
        &[],
        &["42"],
        &[
            "use std::arch::asm;",
            "",
            "fn main() {",
            "    let x: u64 = 14;",
            "    let y: u64;",
            "    unsafe {",
            "        asm!(\"lea {0}, [{1} + {1}*2]\", out(reg) y, in(reg) x);",
            "    }",
            "    println!(\"{y}\");",
            "}",
        ],
    );

    wait_enter();
    clear_screen();
    heading("SUMMARY");

    say("   - asm! sits in unsafe; in / out / inout name the Rust variables");
    say("   - Intel syntax by default on x86; options say what the asm avoids");
    say("   - nothing checks a pointer or an unlisted clobber but you");
    say("   - NASM files link in through extern \"C\" and the System V ABI");
    println!();
    say("  That is the course. The ROADMAP lists traits, Result/Option and");
    say("  lifetimes as the next modules.");
    wait_enter();
}
