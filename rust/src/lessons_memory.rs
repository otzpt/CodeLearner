//! Modules 5 to 7 - ownership, borrowing, and what memory looks like.
//!
//! Every compiler message quoted here was copied from running rustc on the
//! lines shown. Nothing here demonstrates undefined behaviour: the lines that
//! the compiler refuses are quoted, not run.

use crate::ui::{challenge, clear_screen, exercise, heading, question, say, title, wait_enter};
use std::mem::size_of;

fn consume(text: String) -> usize {
    text.len()
}

fn pass_through(text: String) -> String {
    text
}

pub fn lesson_05_ownership() {
    title("MODULE 5 - OWNERSHIP");

    heading("PART 1: one owner, and moving");

    say("  C asks you to free what you malloc and trusts you to do it once.");
    say("  Rust has no free() to call and no garbage collector. Instead:");
    println!();
    say("    1. every value has exactly one owner (a variable)");
    say("    2. when the owner goes out of scope, the value is dropped");
    say("    3. assigning or passing a value MOVES it to a new owner");
    println!();
    say("  Numbers are cheap, so they are copied, as module 4 said. A");
    say("  String owns memory on the heap, so it is moved:");
    println!();
    say("    let s = String::from(\"hi\");");
    say("    let t = s;");
    say("    println!(\"{s} {t}\");");
    println!();
    say("    error[E0382]: borrow of moved value: `s`");
    say("      move occurs because `s` has type `String`, which does not");
    say("      implement the `Copy` trait");
    say("      help: consider cloning the value if the performance cost is");
    say("      acceptable");
    println!();
    say("  s is dead after `let t = s`. That is what stops a double free:");
    say("  there is only ever one owner left to free the memory.");

    wait_enter();
    clear_screen();
    heading("PART 2: a move does not copy the data");

    let text = String::from("robot");
    let before = text.as_ptr();
    let moved = text;
    let after = moved.as_ptr();
    let copy = moved.clone();
    say("    let text = String::from(\"robot\");");
    say("    let before = text.as_ptr();        // where its bytes live");
    say("    let moved = text;");
    say("    let after = moved.as_ptr();");
    println!("  Running:  same address after the move: {}", before == after);
    println!();
    say("  The three-word String itself (pointer, length, capacity) was");
    say("  copied; the bytes on the heap never moved. A move is cheap.");
    println!();
    say("  An actual deep copy is explicit:");
    println!();
    say("    let copy = moved.clone();");
    println!("  Running:  clone has its own buffer: {}", copy.as_ptr() != moved.as_ptr());
    println!();
    println!("  size_of::<String>() = {}   (pointer + length + capacity, 8 bytes each)", size_of::<String>());

    wait_enter();
    clear_screen();
    heading("PART 3: ownership through a function");

    say("  Passing a String to a function moves it in:");
    println!();
    say("    fn consume(text: String) -> usize { text.len() }");
    let owned = String::from("antenna");
    let length = consume(owned);
    println!("  Running:  consume(owned) = {length}");
    println!();
    say("  `owned` was moved into the call and dropped when consume ended.");
    say("  Using it afterwards is the same E0382 error as above. To keep it,");
    say("  hand it back:");
    println!();
    say("    fn pass_through(text: String) -> String { text }");
    let owned = String::from("antenna");
    let owned = pass_through(owned);
    println!("  Running:  still have it: {owned}");
    println!();
    say("  That works, and it is clumsy, which is exactly what module 6's");
    say("  borrowing fixes. Meanwhile a String grows when it has to:");
    println!();
    let mut growing = String::new();
    let mut last = growing.capacity();
    println!("  Running:  len 0, capacity {last}");
    for _ in 0..20 {
        growing.push('a');
        if growing.capacity() != last {
            last = growing.capacity();
            println!("            len {:>2}, capacity {}", growing.len(), last);
        }
    }
    println!();
    say("  Each jump is a new, bigger buffer and a copy. The exact growth");
    say("  rule is not promised by the standard library; these numbers are");
    say("  what this version did.");

    wait_enter();
    clear_screen();
    exercise(5);

    question(
        "let s = String::from(\"a\"); let t = s;   Can s still be used?\n  (yes or no)",
        "no",
        "The String moved to t; s no longer owns anything.",
    );
    question(
        "Which method makes an independent copy of a String?",
        "clone",
        "clone() allocates a new buffer and copies the bytes.",
    );

    challenge(
        &[
            "Write fn shout(mut text: String) -> String that adds a ! to the end",
            "of the text it is given and returns it. Print shout(String::from(\"hey\")).",
        ],
        &[],
        &["hey!"],
        &[
            "fn shout(mut text: String) -> String {",
            "    text.push('!');",
            "    text",
            "}",
            "",
            "fn main() {",
            "    println!(\"{}\", shout(String::from(\"hey\")));",
            "}",
        ],
    );

    wait_enter();
    clear_screen();
    heading("SUMMARY");

    say("   - one owner; dropped when it goes out of scope; no free() to forget");
    say("   - assigning or passing a String moves it; the old name is dead");
    say("   - a move copies the 3-word header, not the heap bytes");
    say("   - .clone() is the explicit deep copy");
    println!();
    say("  Module 6: borrowing, to use a value without taking it.");
    wait_enter();
}

fn length_of(text: &String) -> usize {
    text.len()
}

fn add_exclaim(text: &mut String) {
    text.push('!');
}

fn first_word(text: &str) -> &str {
    match text.find(' ') {
        Some(position) => &text[..position],
        None => text,
    }
}

pub fn lesson_06_borrowing() {
    title("MODULE 6 - REFERENCES AND BORROWING");

    heading("PART 1: & lends, &mut lends and allows changes");

    say("  A reference lets a function use a value without owning it:");
    println!();
    say("    fn length_of(text: &String) -> usize { text.len() }");
    let owned = String::from("antenna");
    let length = length_of(&owned);
    println!("  Running:  length_of(&owned) = {length}, and owned is still: {owned}");
    println!();
    say("  &owned lends it; the function gives it back automatically. To");
    say("  change what was lent, the borrow must be &mut, and the variable");
    say("  itself must be mut:");
    println!();
    say("    fn add_exclaim(text: &mut String) { text.push('!'); }");
    let mut louder = String::from("hey");
    add_exclaim(&mut louder);
    add_exclaim(&mut louder);
    println!("  Running:  after two calls: {louder}");

    wait_enter();
    clear_screen();
    heading("PART 2: the rule, and why it exists");

    say("  At any moment a value may have EITHER any number of & borrows,");
    say("  OR exactly one &mut. Never both. The compiler checks it:");
    println!();
    say("    let mut v = vec![1, 2, 3];");
    say("    let first = &v[0];");
    say("    v.push(4);");
    say("    println!(\"{first}\");");
    println!();
    say("    error[E0502]: cannot borrow `v` as mutable because it is also");
    say("    borrowed as immutable");
    println!();
    say("  Why it matters: push may need a bigger buffer, which moves every");
    say("  element. `first` would then point at memory that was freed -- a");
    say("  use-after-free, the C bug that the C course can only show you the");
    say("  sanitizer's error for. Rust refuses to build it.");
    println!();
    say("  The same idea stops a reference outliving what it points to:");
    println!();
    say("    fn dangle() -> &String { let s = String::from(\"hi\"); &s }");
    println!();
    say("    error[E0106]: missing lifetime specifier");
    say("      this function's return type contains a borrowed value, but");
    say("      there is no value for it to be borrowed from");

    wait_enter();
    clear_screen();
    heading("PART 3: slices");

    say("  A slice is a borrowed window onto part of a string or array: a");
    say("  pointer and a length, no copy.");
    println!();
    say("    fn first_word(text: &str) -> &str { ... up to the first space }");
    println!("  Running:  first_word(\"left motor fault\") = {:?}", first_word("left motor fault"));
    println!();
    let numbers = [10, 20, 30, 40, 50];
    let middle = &numbers[1..4];
    println!("  Running:  &[10, 20, 30, 40, 50][1..4] = {:?}, len {}", middle, middle.len());
    println!();
    say("  &str is the type of a string literal and of any slice of a");
    say("  String; &[i32] is a slice of integers. Function parameters are");
    say("  usually written &str and &[T], so they accept both.");
    println!();
    say("  One trap: string slices count BYTES, not letters, and cutting a");
    say("  character in half is refused at run time, with this message:");
    println!();
    say("    end byte index 1 is not a char boundary; it is inside 'é'");
    say("    (bytes 0..2 of string)");

    wait_enter();
    clear_screen();
    exercise(6);

    question(
        "How many &mut borrows of one value can exist at the same time?\n  (a number)",
        "1",
        "Exactly one &mut, and then no & borrows alongside it.",
    );
    question(
        "Does length_of(&owned) move owned into the function? (yes or no)",
        "no",
        "& only lends; the caller keeps ownership.",
    );

    challenge(
        &[
            "Write fn add_one(x: &mut i32) that adds 1 to the number it is lent.",
            "Start with let mut n = 41;, call it, and print n.",
        ],
        &[],
        &["42"],
        &[
            "fn add_one(x: &mut i32) {",
            "    *x += 1;",
            "}",
            "",
            "fn main() {",
            "    let mut n = 41;",
            "    add_one(&mut n);",
            "    println!(\"{n}\");",
            "}",
        ],
    );

    wait_enter();
    clear_screen();
    heading("SUMMARY");

    say("   - &value lends read-only; &mut value lends with permission to change");
    say("   - many & XOR one &mut, checked at compile time");
    say("   - a reference cannot outlive its value: no dangling pointers");
    say("   - a slice (&str, &[T]) is a pointer and a length");
    println!();
    say("  Module 7: what all of this looks like in memory.");
    wait_enter();
}

pub fn lesson_07_memory() {
    title("MODULE 7 - MEMORY: STACK, HEAP AND RAW POINTERS");

    heading("PART 1: how big things are");

    say("  size_of::<T>() is the number of bytes a T occupies. A fixed array");
    say("  is its elements; String and Vec are a three-word header, the data");
    say("  itself living on the heap; a &str is a pointer and a length:");
    println!();
    println!("    size_of::<[i32; 4]>()  = {}", size_of::<[i32; 4]>());
    println!("    size_of::<&str>()      = {}   (pointer + length)", size_of::<&str>());
    println!("    size_of::<String>()    = {}   (pointer + length + capacity)", size_of::<String>());
    println!("    size_of::<Vec<i32>>()  = {}", size_of::<Vec<i32>>());
    println!("    size_of::<Box<i32>>()  = {}    (just a pointer)", size_of::<Box<i32>>());
    println!("    size_of::<Option<Box<i32>>>() = {}", size_of::<Option<Box<i32>>>());
    println!();
    say("  The last line is a Rust specialty: a Box is never null, so an");
    say("  Option of one can use the null address to mean None, and costs");
    say("  nothing extra. C needs a NULL check on every pointer; here the");
    say("  type says whether it can be missing.");
    println!();
    let on_stack = 5;
    let on_heap = Box::new(5);
    println!("  Running:  a local sits at {:p}", &on_stack);
    println!("            a Box'd value at {:p}", &*on_heap);
    say("  (the addresses differ on every run)");

    wait_enter();
    clear_screen();
    heading("PART 2: a Vec grows");

    say("    let mut v: Vec<i32> = Vec::new();");
    say("    // push 40 numbers, printing when the capacity changes");
    let mut v: Vec<i32> = Vec::new();
    let mut last = v.capacity();
    println!("  Running:  capacity {last}");
    for i in 0..40 {
        v.push(i);
        if v.capacity() != last {
            last = v.capacity();
            println!("            len {:>2}  ->  capacity {}", v.len(), last);
        }
    }
    println!();
    say("  Each change is a new heap block and a copy of every element: a");
    say("  realloc, done for you. If the final size is known, ask for it up");
    say("  front with Vec::with_capacity(n) and it happens once. (The growth");
    say("  steps are an implementation detail, not a promise.)");
    println!();
    say("  Indexing is checked. v[10] on 3 elements does not read whatever");
    say("  is nearby; it stops the program with this message:");
    println!();
    say("    index out of bounds: the len is 3 but the index is 10");
    println!();
    let three = vec![1, 2, 3];
    say("  .get(10) is the polite version: it returns an Option.");
    println!("  Running:  vec![1,2,3].get(10) = {:?},  .get(1) = {:?}", three.get(10), three.get(1));

    wait_enter();
    clear_screen();
    heading("PART 3: raw bytes, raw pointers, unsafe");

    say("  Seeing memory as bytes:");
    println!();
    say("    0x12345678u32.to_le_bytes()");
    println!("  Running:  {:x?}", 0x12345678u32.to_le_bytes());
    println!();
    say("  Little-endian: the least significant byte first, as on x86-64.");
    println!();
    say("  Swapping or replacing in place, safely:");
    let mut a = 1;
    let mut b = 2;
    std::mem::swap(&mut a, &mut b);
    println!("  Running:  mem::swap(&mut a, &mut b) -> a = {a}, b = {b}");
    println!();
    say("  Under all the safe types are raw pointers, *const T and *mut T,");
    say("  which are plain addresses like C's. Making one is safe; using it");
    say("  is not, because the compiler can no longer prove it is valid, so");
    say("  the use sits in an unsafe block you are promising is correct:");
    println!();
    let mut value = 10;
    let pointer = &mut value as *mut i32;
    unsafe {
        *pointer += 5;
    }
    say("    let pointer = &mut value as *mut i32;");
    say("    unsafe { *pointer += 5; }");
    println!("  Running:  value = {value}");
    println!();
    let bytes = [1u8, 2, 3, 4];
    let start = bytes.as_ptr();
    let third = unsafe { *start.add(2) };
    say("    let third = unsafe { *bytes.as_ptr().add(2) };   // pointer arithmetic");
    println!("  Running:  third = {third}");
    println!();
    say("  start.add(2) is the address two elements on. Nothing checks it");
    say("  is inside the array: that is the promise unsafe asks for.");

    wait_enter();
    clear_screen();
    exercise(7);

    question(
        "Does v[10] on a 3-element Vec return None? (yes or no)",
        "no",
        "Indexing panics with 'index out of bounds'; .get(10) is the one that returns None.",
    );
    question(
        "Which keyword marks code the compiler cannot verify, like using a\n  raw pointer?",
        "unsafe",
        "An unsafe block is a promise by the programmer that the code is valid.",
    );

    challenge(
        &[
            "Start with let mut a = 1; and let mut b = 2;. Swap them with",
            "std::mem::swap, then print a and b on separate lines.",
        ],
        &[],
        &["2", "1"],
        &[
            "fn main() {",
            "    let mut a = 1;",
            "    let mut b = 2;",
            "    std::mem::swap(&mut a, &mut b);",
            "    println!(\"{a}\");",
            "    println!(\"{b}\");",
            "}",
        ],
    );

    wait_enter();
    clear_screen();
    heading("SUMMARY");

    say("   - size_of::<T>(); String and Vec are a 3-word header over heap data");
    say("   - a Vec reallocates as it grows; with_capacity avoids it");
    say("   - v[i] is bounds-checked and panics; .get(i) returns an Option");
    say("   - raw pointers are C's pointers; using one needs unsafe");
    println!();
    say("  Module 8: structs, to group values and give them methods.");
    wait_enter();
}
