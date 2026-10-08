//! Modules 1 to 4 - printing, variables, control flow and functions.
//!
//! Rule used throughout: nothing is claimed without being shown. Where an
//! example can run here, it runs, and the compiler's messages quoted in the
//! lessons were copied from running rustc, not written from memory.

use crate::ui::{challenge, clear_screen, exercise, heading, question, say, title, wait_enter};
use std::mem::size_of;

pub fn lesson_01_println() {
    title("MODULE 1 - COMPILING AND println!");

    heading("PART 1: from source to a program");

    say("  Rust is compiled, like C: rustc turns a .rs file into a native");
    say("  program, and you run that.");
    println!();
    say("    rustc main.rs");
    say("    ./main");
    println!();
    say("  Real projects use Cargo, which wraps rustc (cargo new, cargo run).");
    say("  This course uses plain rustc so there is nothing hidden.");
    println!();
    say("  A program starts at fn main(). The smallest one:");
    println!();
    say("    fn main() {");
    say("        println!(\"Hello\");");
    say("    }");
    println!();
    say("  The ! in println! means it is a macro, not a function. The");
    say("  difference matters here: the macro reads its format string at");
    say("  compile time and checks it against the arguments. A mismatch is");
    say("  an error before the program exists, not a crash later:");
    println!();
    say("    println!(\"{} {}\", 1);");
    println!();
    say("    error: 2 positional arguments in format string, but there is 1 argument");

    wait_enter();
    clear_screen();
    heading("PART 2: format specifiers");

    println!("    println!(\"{{}} {{:?}}\", 42, \"hi\");");
    println!("  Running:  {} {:?}", 42, "hi");
    println!();
    say("  {} prints a value as people read it; {:?} prints it as code would");
    say("  write it (note the quotes). More, with a width and an alignment:");
    println!();
    println!("    {{:>6}}  ->  [{:>6}]", 42);
    println!("    {{:<6}}  ->  [{:<6}]", 42);
    println!("    {{:^6}}  ->  [{:^6}]", 42);
    println!("    {{:06}}  ->  [{:06}]", 42);
    println!("    {{:.2}}  ->  [{:.2}]    (two decimals of 3.14159)", 3.14159);
    println!("    {{:x}}   ->  [{:x}]     (hexadecimal of 255)", 255);
    println!("    {{:b}}   ->  [{:b}]  (binary of 5)", 5);
    println!("    {{:#b}}  ->  [{:#b}] (binary of 5, with its prefix)", 5);
    println!();
    let name = "Ana";
    let age = 31;
    say("  A name in scope can go straight into the braces:");
    println!();
    say("    let name = \"Ana\"; let age = 31;");
    say("    println!(\"{name} is {age}\");");
    println!("  Running:  {name} is {age}");
    println!();
    say("  To print an actual brace, double it: {{ and }}.");

    wait_enter();
    clear_screen();
    heading("PART 3: print!, eprintln!, and comments");

    say("  print! is println! without the line break:");
    println!();
    say("    print!(\"a\"); print!(\"b\"); println!(\"c\");");
    print!("  Running:  ");
    print!("a");
    print!("b");
    println!("c");
    println!();
    say("  eprintln! writes to standard error, a second output channel that");
    say("  a terminal shows next to the first but a pipe keeps apart. Errors");
    say("  and diagnostics belong there, results on standard output.");
    println!();
    say("    // a comment to the end of the line");
    say("    /* a block comment */");
    say("    /// a documentation comment, which cargo doc turns into pages");

    wait_enter();
    clear_screen();
    exercise(1);

    question(
        "What does the ! in println! tell you? (one word)",
        "macro",
        "It is a macro: checked at compile time, not an ordinary function.",
    );
    question(
        "println!(\"{:>5}|\", 42)  How many spaces come before the 42?",
        "3",
        "A width of 5 right-aligns the 2-digit number, padding with 3 spaces.",
    );

    challenge(
        &[
            "Given let a = 7; and let b = 6;, print  7 times 6 is 42  using the",
            "names inside the braces for a and b, and a * b for the result.",
        ],
        &[],
        &["7 times 6 is 42"],
        &[
            "fn main() {",
            "    let a = 7;",
            "    let b = 6;",
            "    println!(\"{a} times {b} is {}\", a * b);",
            "}",
        ],
    );

    wait_enter();
    clear_screen();
    heading("SUMMARY");

    say("   - rustc file.rs builds a native program; fn main() starts it");
    say("   - println! is a macro; its format string is checked at compile time");
    say("   - {} for display, {:?} for debug, {:>6} {:06} {:.2} {:x} {:b} for layout");
    say("   - {name} reads a variable; {{ }} prints braces");
    println!();
    say("  Module 2: variables, and what Rust refuses to let you change.");
    wait_enter();
}

pub fn lesson_02_variables() {
    title("MODULE 2 - VARIABLES AND TYPES");

    heading("PART 1: immutable by default, and shadowing");

    say("  let makes a variable, and it cannot be changed:");
    println!();
    say("    let x = 5;");
    say("    x = 6;");
    println!();
    say("    error[E0384]: cannot assign twice to immutable variable `x`");
    println!();
    say("  Say mut when it should change:");
    println!();
    let mut count = 5;
    count += 1;
    say("    let mut count = 5;  count += 1;");
    println!("  Running:  count = {count}");
    println!();
    say("  Different from changing: SHADOWING makes a new variable with the");
    say("  same name, and it may even have a different type:");
    println!();
    let spaces = "   ";
    let spaces = spaces.len();
    say("    let spaces = \"   \";");
    say("    let spaces = spaces.len();");
    println!("  Running:  spaces = {spaces}");

    wait_enter();
    clear_screen();
    heading("PART 2: integers have a size, and overflow is not silent");

    say("  The integer types say their size: i8 i16 i32 i64 i128 (signed),");
    say("  u8 u16 u32 u64 u128 (unsigned), and isize / usize, which are the");
    say("  size of a pointer on the machine:");
    println!();
    println!("    size_of::<u8>()    = {}", size_of::<u8>());
    println!("    size_of::<i32>()   = {}", size_of::<i32>());
    println!("    size_of::<i64>()   = {}", size_of::<i64>());
    println!("    size_of::<usize>() = {}   (8 on a 64-bit PC)", size_of::<usize>());
    println!();
    println!("    u8::MAX = {},  i32::MAX = {},  i8::MIN = {}", u8::MAX, i32::MAX, i8::MIN);
    println!();
    say("  Going past the top is where Rust differs from C. A plain");
    say("  250u8 + 10 is checked: a program built without optimisation stops");
    say("  with  attempt to add with overflow  (copied from running one), and");
    say("  one built with -O silently wraps. Not run here for that reason.");
    say("  When you know what you want, say it:");
    println!();
    let near_top: u8 = 250;
    println!("    250u8.checked_add(10)    = {:?}   (None: it did not fit)", near_top.checked_add(10));
    println!("    250u8.wrapping_add(10)   = {}      (wraps around)", near_top.wrapping_add(10));
    println!("    250u8.saturating_add(10) = {}    (sticks at the top)", near_top.saturating_add(10));
    println!("    250u8.overflowing_add(10)= {:?}", near_top.overflowing_add(10));

    wait_enter();
    clear_screen();
    heading("PART 3: floats, casts, and the compound types");

    say("    7 / 2   -> 3     integer division, fraction dropped");
    println!("  Running:  {}", 7 / 2);
    say("    7.0 / 2.0   -> 3.5");
    println!("  Running:  {}", 7.0 / 2.0);
    say("    -7 / 2 and -7 % 2 truncate toward zero, like C:");
    println!("  Running:  {} and {}", -7 / 2, -7 % 2);
    println!();
    say("  There is no automatic conversion between number types. as is the");
    say("  explicit one, and it does not complain when it loses something:");
    println!();
    println!("    300i32 as u8   = {}     (kept the low 8 bits)", 300i32 as u8);
    println!("    -1i32 as u32   = {}", -1i32 as u32);
    println!("    3.99f64 as i32 = {}       (fraction dropped)", 3.99f64 as i32);
    println!();
    say("  Mixing types is an error, not a guess:");
    println!();
    say("    let x: i32 = \"5\";");
    say("    error[E0308]: mismatched types");
    println!();
    let pair: (i32, f64) = (3, 2.5);
    let row: [i32; 3] = [10, 20, 30];
    say("  A tuple groups values of different types; an array holds a fixed");
    say("  number of one type. The length is part of the array's type:");
    println!();
    println!("    let pair: (i32, f64) = (3, 2.5);   pair.0 = {}, pair.1 = {}", pair.0, pair.1);
    println!("    let row: [i32; 3] = [10, 20, 30];  row[1] = {}, row.len() = {}", row[1], row.len());

    wait_enter();
    clear_screen();
    exercise(2);

    question(
        "let x = 5; x = 6;   Does it compile? (yes or no)",
        "no",
        "Variables are immutable unless declared with mut.",
    );
    question(
        "What does 250u8.wrapping_add(10) give?",
        "4",
        "260 wraps around the 256 values of a u8, leaving 4.",
    );

    challenge(
        &[
            "Put the text \"robot\" in a variable and print its length. Then shadow",
            "that variable with the length times 2, and print that too.",
        ],
        &[],
        &["5", "10"],
        &[
            "fn main() {",
            "    let word = \"robot\";",
            "    let size = word.len();",
            "    println!(\"{size}\");",
            "    let size = size * 2;",
            "    println!(\"{size}\");",
            "}",
        ],
    );

    wait_enter();
    clear_screen();
    heading("SUMMARY");

    say("   - let is immutable; mut allows change; shadowing makes a new variable");
    say("   - integer types name their size; overflow panics in a debug build");
    say("   - checked_, wrapping_, saturating_ say what to do at the edge");
    say("   - no implicit number conversion; as converts and may lose data");
    println!();
    say("  Module 3: making decisions and repeating.");
    wait_enter();
}

pub fn lesson_03_control_flow() {
    title("MODULE 3 - CONTROL FLOW");

    heading("PART 1: if is an expression");

    let reading = 620;
    say("    let reading = 620;");
    say("    if reading > 800 { \"very bright\" }");
    say("    else if reading > 500 { \"bright\" }");
    say("    else { \"dark\" }");
    let label = if reading > 800 {
        "very bright"
    } else if reading > 500 {
        "bright"
    } else {
        "dark"
    };
    println!("  Running:  {label}");
    println!();
    say("  if produces a value, so it can sit on the right of a let; every");
    say("  branch must give the same type. The condition must be a bool.");
    say("  There is no 'non-zero is true' as in C:");
    println!();
    say("    if 1 { ... }");
    say("    error[E0308]: mismatched types: expected `bool`, found integer");

    wait_enter();
    clear_screen();
    heading("PART 2: loops");

    say("    for i in 1..4 { print!(\"{i} \"); }");
    print!("  Running:  ");
    for i in 1..4 {
        print!("{i} ");
    }
    println!();
    say("    for i in (1..=4).rev() { print!(\"{i} \"); }");
    print!("  Running:  ");
    for i in (1..=4).rev() {
        print!("{i} ");
    }
    println!();
    println!();
    say("  1..4 stops BEFORE 4; 1..=4 includes it. .rev() runs it backwards.");
    println!();
    say("    let mut n = 3;");
    say("    while n > 0 { print!(\"{n} \"); n -= 1; }");
    print!("  Running:  ");
    let mut n = 3;
    while n > 0 {
        print!("{n} ");
        n -= 1;
    }
    println!();
    println!();
    say("  loop runs until a break, and break can hand a value out:");
    println!();
    let mut tries = 0;
    let found = loop {
        tries += 1;
        if tries * tries > 50 {
            break tries;
        }
    };
    say("    let found = loop { tries += 1; if tries * tries > 50 { break tries; } };");
    println!("  Running:  found = {found}");

    wait_enter();
    clear_screen();
    heading("PART 3: match");

    say("  match compares a value against patterns, and the compiler checks");
    say("  that EVERY possible value is covered:");
    println!();
    say("    match score {");
    say("        0 => \"none\",");
    say("        1..=9 => \"few\",");
    say("        _ => \"many\",");
    say("    }");
    for score in [0, 4, 30] {
        let word = match score {
            0 => "none",
            1..=9 => "few",
            _ => "many",
        };
        println!("  Running:  score {score} -> {word}");
    }
    println!();
    say("  _ means 'anything else'. Leave it out and the program does not");
    say("  compile: a case forgotten is an error you see now, not a bug you");
    say("  find later.");

    wait_enter();
    clear_screen();
    exercise(3);

    question(
        "for i in 1..4 { ... }   How many times does the body run?",
        "3",
        "1..4 stops before 4, so i is 1, 2, 3.",
    );
    question(
        "for i in 1..=4 { println!(\"{i}\"); }   How many lines?",
        "4",
        "1..=4 includes the 4.",
    );

    challenge(
        &[
            "Print the multiples of 3 from 3 up to 12, each on its own line.",
            "Use a loop.",
        ],
        &[],
        &["3", "6", "9", "12"],
        &[
            "fn main() {",
            "    for i in 1..=4 {",
            "        println!(\"{}\", i * 3);",
            "    }",
            "}",
        ],
    );

    wait_enter();
    clear_screen();
    heading("SUMMARY");

    say("   - if is an expression; its condition must be a bool");
    say("   - for over ranges (1..4, 1..=4), while, and loop with break value");
    say("   - match must cover every case; _ is 'anything else'");
    println!();
    say("  Module 4: functions.");
    wait_enter();
}

fn square(value: i32) -> i32 {
    value * value
}

fn min_and_max(a: i32, b: i32) -> (i32, i32) {
    if a < b {
        (a, b)
    } else {
        (b, a)
    }
}

fn factorial(n: u64) -> u64 {
    if n <= 1 {
        1
    } else {
        n * factorial(n - 1)
    }
}

pub fn lesson_04_functions() {
    title("MODULE 4 - FUNCTIONS");

    heading("PART 1: parameters and the value that falls out");

    say("    fn square(value: i32) -> i32 {");
    say("        value * value");
    say("    }");
    println!();
    println!("  Running:  square(7) = {}", square(7));
    println!();
    say("  Parameter and return types are written out; the compiler does");
    say("  not guess them across a function boundary. The last expression,");
    say("  with NO semicolon, is the return value. return exists, for");
    say("  leaving early.");
    println!();
    say("  The semicolon is the classic trap, because it turns the value");
    say("  into a statement and the function returns nothing:");
    println!();
    say("    fn five() -> i32 { 5; }");
    println!();
    say("    error[E0308]: mismatched types");
    say("      expected `i32`, found `()`");
    say("      help: remove this semicolon to return this value");

    wait_enter();
    clear_screen();
    heading("PART 2: several values, and nothing");

    say("  A function can hand back a tuple:");
    println!();
    say("    fn min_and_max(a: i32, b: i32) -> (i32, i32) { ... }");
    let (low, high) = min_and_max(9, 4);
    println!("  Running:  min_and_max(9, 4) = ({low}, {high})");
    println!();
    say("  A function that returns nothing returns (), called the unit");
    say("  type. main() is one. Writing -> () is allowed and never needed.");
    println!();
    say("  Integers are Copy: passing one gives the function its own copy,");
    say("  exactly as in C, and the caller's variable is untouched. Module 5");
    say("  is about what happens when the value is not so cheap to copy.");

    wait_enter();
    clear_screen();
    heading("PART 3: functions calling functions, and constants");

    say("    fn factorial(n: u64) -> u64 {");
    say("        if n <= 1 { 1 } else { n * factorial(n - 1) }");
    say("    }");
    println!();
    println!("  Running:  factorial(10) = {}", factorial(10));
    println!("  Running:  factorial(20) = {}", factorial(20));
    println!();
    say("  Recursion uses the stack, one frame per call, and the stack is");
    say("  finite; a runaway recursion ends the program. And factorial(21)");
    say("  no longer fits in a u64, so it would hit module 2's overflow");
    say("  check. Both are why the type is written down.");
    println!();
    say("  A value fixed for the whole program is a const, with its type:");
    println!();
    say("    const MAX_SPEED: u32 = 255;");
    println!();
    say("  It is inlined wherever it is used and takes no memory of its own.");

    wait_enter();
    clear_screen();
    exercise(4);

    question(
        "fn five() -> i32 { 5; }   Does it compile? (yes or no)",
        "no",
        "The semicolon turns the 5 into a statement; the body returns ().",
    );
    question(
        "What is the type of a function that returns nothing? (the symbol)",
        "()",
        "It returns the unit type, written ().",
    );

    challenge(
        &[
            "Write fn larger(a: i32, b: i32) -> i32 that returns the bigger of two",
            "numbers. Print larger(4, 9) and larger(7, 2), one per line.",
        ],
        &[],
        &["9", "7"],
        &[
            "fn larger(a: i32, b: i32) -> i32 {",
            "    if a > b {",
            "        a",
            "    } else {",
            "        b",
            "    }",
            "}",
            "",
            "fn main() {",
            "    println!(\"{}\", larger(4, 9));",
            "    println!(\"{}\", larger(7, 2));",
            "}",
        ],
    );

    wait_enter();
    clear_screen();
    heading("SUMMARY");

    say("   - fn name(param: Type) -> Type; the last expression is the result");
    say("   - a stray semicolon on that last line breaks the return type");
    say("   - tuples return several values; () is 'nothing'");
    say("   - const for a value fixed at compile time");
    println!();
    say("  Module 5: ownership, the idea the rest of Rust is built on.");
    wait_enter();
}
