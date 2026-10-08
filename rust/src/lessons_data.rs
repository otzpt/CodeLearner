//! Modules 8 to 10 - structs, enums and Option, and Result.
//!
//! Compiler messages and panics quoted here were copied from running rustc
//! (1.98.1) on the lines shown. The robot-command parser built up across
//! modules 9 and 10 is the same job the Arduino course's module 9 and the
//! MicroPython course do with strings, so the three can be compared.

use crate::ui::{challenge, clear_screen, exercise, heading, question, say, title, wait_enter};
use std::io::{self, Write};
use std::mem::{align_of, offset_of, size_of};
use std::num::ParseIntError;

#[derive(Debug, Clone, Copy)]
struct Motor {
    pin: u8,
    speed: u8,
}

impl Motor {
    fn new(pin: u8) -> Motor {
        Motor { pin, speed: 0 }
    }

    fn set_speed(&mut self, speed: u8) {
        self.speed = speed;
    }

    fn is_running(&self) -> bool {
        self.speed > 0
    }
}

// These two exist only to be measured, so their fields are never read.
#[allow(dead_code)]
#[repr(C)]
struct LayoutC {
    first: u8,
    second: u32,
    third: u8,
}

#[allow(dead_code)]
struct LayoutRust {
    first: u8,
    second: u32,
    third: u8,
}

struct Celsius(f64);

fn to_fahrenheit(temperature: Celsius) -> f64 {
    temperature.0 * 9.0 / 5.0 + 32.0
}

pub fn lesson_08_structs() {
    title("MODULE 8 - STRUCTS AND METHODS");

    heading("PART 1: grouping fields, and a struct that cannot be half-changed");

    say("  A struct groups named values, like C's struct:");
    println!();
    say("    #[derive(Debug, Clone, Copy)]");
    say("    struct Motor { pin: u8, speed: u8 }");
    println!();
    let mut motor = Motor { pin: 3, speed: 120 };
    say("    let mut motor = Motor { pin: 3, speed: 120 };");
    println!("  Running:  motor.speed = {}, motor.pin = {}", motor.speed, motor.pin);
    motor.speed = 200;
    say("    motor.speed = 200;");
    println!("  Running:  {:?}", motor);
    println!();
    say("  #[derive(Debug)] asks the compiler to write the code that lets");
    say("  {:?} print the struct; {:#?} lays it out over several lines.");
    println!();
    say("  Mutability belongs to the whole variable, not to one field. Leave");
    say("  out mut and no field can change:");
    println!();
    say("    let motor = Motor { pin: 3, speed: 120 };");
    say("    motor.speed = 5;");
    println!();
    say("    error[E0594]: cannot assign to `motor.speed`, as `motor` is not");
    say("    declared as mutable");

    wait_enter();
    clear_screen();
    heading("PART 2: methods, and the three kinds of self");

    say("  impl attaches functions to a struct:");
    println!();
    say("    impl Motor {");
    say("        fn new(pin: u8) -> Motor { Motor { pin, speed: 0 } }");
    say("        fn set_speed(&mut self, speed: u8) { self.speed = speed; }");
    say("        fn is_running(&self) -> bool { self.speed > 0 }");
    say("    }");
    println!();
    let mut left = Motor::new(5);
    println!("  Running:  Motor::new(5).is_running() = {}", left.is_running());
    left.set_speed(90);
    println!("  Running:  after set_speed(90), is_running() = {}", left.is_running());
    println!();
    say("  Motor::new has no self: an associated function, called with ::,");
    say("  the usual way to build one. The others are methods, called with a");
    say("  dot, and the first parameter says how they use the value, which is");
    say("  modules 5 and 6 again:");
    println!();
    say("    &self       borrows it to read             (is_running)");
    say("    &mut self   borrows it to change           (set_speed)");
    say("    self        takes ownership, and consumes it");
    println!();
    say("  Calling left.set_speed(90) is shorthand for Motor::set_speed(&mut");
    say("  left, 90); the compiler takes the borrow for you.");
    println!();
    let copy = left;
    let mut other = copy;
    other.set_speed(10);
    say("  Because Motor derived Copy (all its fields are plain numbers),");
    say("  assigning it copies it, like an int, instead of moving it:");
    println!();
    println!("  Running:  copy.speed = {}, other.speed = {}", copy.speed, other.speed);

    wait_enter();
    clear_screen();
    heading("PART 3: layout in memory, and types that cannot be mixed");

    say("  A struct's size is not just the sum of its fields:");
    println!();
    println!("    size_of::<Motor>()      = {}   (two u8)", size_of::<Motor>());
    println!("    size_of::<LayoutC>()    = {}  (#[repr(C)]: u8, u32, u8, in that order)", size_of::<LayoutC>());
    println!(
        "    offsets in LayoutC      = {}, {}, {}   (align_of = {})",
        offset_of!(LayoutC, first),
        offset_of!(LayoutC, second),
        offset_of!(LayoutC, third),
        align_of::<LayoutC>()
    );
    println!("    size_of::<LayoutRust>() = {}   (the same fields, default layout)", size_of::<LayoutRust>());
    println!();
    say("  #[repr(C)] gives C's layout: fields in order, each aligned to its");
    say("  own size, so the u32 starts at 4 and there is padding after the");
    say("  first u8 and after the last. That is what memory-mapped hardware");
    say("  and a C library expect. Without it Rust may reorder the fields to");
    say("  waste less, as it did here; the order is not promised.");
    println!();
    let boiling = Celsius(100.0);
    say("  A tuple struct wraps one value in its own type. Celsius and");
    say("  Fahrenheit can both hold an f64 and still never be confused:");
    println!();
    say("    struct Celsius(f64);   fn to_fahrenheit(t: Celsius) -> f64");
    println!("  Running:  to_fahrenheit(Celsius(100.0)) = {}", to_fahrenheit(boiling));
    println!();
    say("    error[E0308]: mismatched types");
    say("      expected `Celsius`, found `Fahrenheit`");
    println!();
    say("  That is a compile-time guard against passing the wrong unit. It");
    say("  costs nothing at run time: Celsius is the same 8 bytes as the f64");
    say("  inside it.");

    wait_enter();
    clear_screen();
    exercise(8);

    question(
        "A method that only READS the struct takes which first parameter?\n  (&self or &mut self)",
        "&self",
        "&self borrows to read; &mut self is for methods that change it.",
    );
    question(
        "A #[repr(C)] struct of u8, u32, u8 has what size on this PC?",
        "12",
        "The u32 sits at offset 4 and the struct is padded to a multiple of 4.",
    );

    challenge(
        &[
            "Write struct Counter { count: u32 } with fn new() -> Counter, a method",
            "increment(&mut self) and a method get(&self) -> u32. Create one,",
            "increment it twice, and print get().",
        ],
        &[],
        &["2"],
        &[
            "struct Counter {",
            "    count: u32,",
            "}",
            "",
            "impl Counter {",
            "    fn new() -> Counter {",
            "        Counter { count: 0 }",
            "    }",
            "",
            "    fn increment(&mut self) {",
            "        self.count += 1;",
            "    }",
            "",
            "    fn get(&self) -> u32 {",
            "        self.count",
            "    }",
            "}",
            "",
            "fn main() {",
            "    let mut counter = Counter::new();",
            "    counter.increment();",
            "    counter.increment();",
            "    println!(\"{}\", counter.get());",
            "}",
        ],
    );

    wait_enter();
    clear_screen();
    heading("SUMMARY");

    say("   - struct groups named fields; mutability belongs to the whole variable");
    say("   - impl adds methods: &self reads, &mut self changes, self consumes");
    say("   - #[derive(Debug, Clone, Copy)] writes the boring code for you");
    say("   - #[repr(C)] fixes the layout; a tuple struct makes a distinct type");
    println!();
    say("  Module 9: enums, and a value that can be missing.");
    wait_enter();
}

#[derive(Debug, Clone, Copy, PartialEq)]
enum Direction {
    Forward,
    Left,
    Right,
    Stop,
}

fn turn_angle(direction: Direction) -> i32 {
    match direction {
        Direction::Forward => 0,
        Direction::Left => -90,
        Direction::Right => 90,
        Direction::Stop => 0,
    }
}

#[derive(Debug, PartialEq)]
enum Command {
    Move(i32),
    Turn { degrees: i32 },
    Stop,
}

fn describe(command: &Command) -> String {
    match command {
        Command::Move(amount) => format!("drive {amount}"),
        Command::Turn { degrees } => format!("turn {degrees} degrees"),
        Command::Stop => String::from("stop"),
    }
}

fn first_negative(values: &[i32]) -> Option<usize> {
    for index in 0..values.len() {
        if values[index] < 0 {
            return Some(index);
        }
    }
    None
}

pub fn lesson_09_enums() {
    title("MODULE 9 - ENUMS, match AND Option");

    heading("PART 1: a value that is one of a fixed set");

    say("  An enum is a type whose values are a fixed list of names:");
    println!();
    say("    enum Direction { Forward, Left, Right, Stop }");
    println!();
    for direction in [Direction::Forward, Direction::Left, Direction::Right, Direction::Stop] {
        println!(
            "  Running:  {:?} turns {:>3} degrees, and is the number {}",
            direction,
            turn_angle(direction),
            direction as u8
        );
    }
    println!();
    println!("  size_of::<Direction>() = {}   (four choices fit in one byte)", size_of::<Direction>());
    println!();
    say("  match must cover every variant. Forget one and the program does not");
    say("  build:");
    println!();
    say("    error[E0004]: non-exhaustive patterns: `Direction::Stop` not covered");
    println!();
    say("  Compare C, where a switch that forgets a case compiles (at most with");
    say("  a warning) and quietly does nothing. Add a fifth Direction later and");
    say("  the compiler lists every match that now needs a new line.");

    wait_enter();
    clear_screen();
    heading("PART 2: variants that carry data");

    say("  Each variant may hold values of its own:");
    println!();
    say("    enum Command { Move(i32), Turn { degrees: i32 }, Stop }");
    println!();
    for command in [Command::Move(120), Command::Turn { degrees: -90 }, Command::Stop] {
        println!("  Running:  {:?}  ->  {}", command, describe(&command));
    }
    println!();
    say("  match takes the data out by pattern:");
    println!();
    say("    match command {");
    say("        Command::Move(amount) => ...,");
    say("        Command::Turn { degrees } => ...,");
    say("        Command::Stop => ...,");
    say("    }");
    println!();
    println!("  size_of::<Command>() = {}   (a tag saying which variant, then room for the biggest)", size_of::<Command>());
    println!();
    say("  This is a C tagged union, the one the C course's module 15 builds");
    say("  by hand with an enum and a union, except that here the compiler");
    say("  keeps the tag and the data in step: reading the wrong variant's");
    say("  data is not expressible.");

    wait_enter();
    clear_screen();
    heading("PART 3: Option, the absence of null");

    say("  Rust has no null. A value that might be missing says so in its");
    say("  type: Option<T> is either Some(value) or None.");
    println!();
    say("    fn first_negative(values: &[i32]) -> Option<usize>");
    let with_negative = [4, 9, -2, 7];
    let without = [1, 2, 3];
    println!("  Running:  first_negative(&[4, 9, -2, 7]) = {:?}", first_negative(&with_negative));
    println!("  Running:  first_negative(&[1, 2, 3])     = {:?}", first_negative(&without));
    println!();
    say("  The caller cannot use the answer without deciding what None");
    say("  means:");
    println!();
    say("    if let Some(position) = first_negative(&values) { ... }");
    say("    first_negative(&values).unwrap_or(0)");
    println!("  Running:  unwrap_or(0) on the second list = {}", first_negative(&without).unwrap_or(0));
    println!();
    say("  Adding a number to an Option is refused, not guessed at:");
    println!();
    say("    error[E0369]: cannot add `{integer}` to `Option<i32>`");
    println!();
    say("  .unwrap() takes the value out and, on None, stops the program:");
    println!();
    say("    called `Option::unwrap()` on a `None` value");
    println!();
    say("  Use it only where None is truly impossible, and say so with a");
    say("  comment. Module 7 showed that an Option of a Box costs no extra");
    say("  space; for a type with no spare value, the tag is a real byte:");
    println!("  Running:  size_of::<Option<Box<i32>>>() = {}  (same as a Box)", size_of::<Option<Box<i32>>>());
    println!("  Running:  size_of::<Option<u8>>()       = {}  (a byte for the tag, one for the value)", size_of::<Option<u8>>());

    wait_enter();
    clear_screen();
    exercise(9);

    question(
        "You add a fifth variant to an enum. What does every existing\n  match that lacks it do? (compiles or fails)",
        "fails",
        "match must be exhaustive, so each one becomes a compile error.",
    );
    question(
        "What does .unwrap() do when it meets None? (one word)",
        "panics",
        "It stops the program with a message; use unwrap_or or match to cope instead.",
    );

    challenge(
        &[
            "Write fn first_even(values: &[i32]) -> Option<i32> returning the first",
            "even number, or None. Print the result for [3, 5, 8, 9] and for [1, 3],",
            "each with {:?}, on its own line.",
        ],
        &[],
        &["Some(8)", "None"],
        &[
            "fn first_even(values: &[i32]) -> Option<i32> {",
            "    for index in 0..values.len() {",
            "        if values[index] % 2 == 0 {",
            "            return Some(values[index]);",
            "        }",
            "    }",
            "    None",
            "}",
            "",
            "fn main() {",
            "    println!(\"{:?}\", first_even(&[3, 5, 8, 9]));",
            "    println!(\"{:?}\", first_even(&[1, 3]));",
            "}",
        ],
    );

    wait_enter();
    clear_screen();
    heading("SUMMARY");

    say("   - enum: a fixed set of variants; match must cover them all");
    say("   - variants can carry data: Move(i32), Turn { degrees: i32 }");
    say("   - Option<T> replaces null: Some(value) or None, handled before use");
    say("   - unwrap() panics on None; unwrap_or, if let and match do not");
    println!();
    say("  Module 10: Result, for operations that can fail.");
    wait_enter();
}

fn double(text: &str) -> Result<i32, ParseIntError> {
    let number: i32 = text.trim().parse()?;
    Ok(number * 2)
}

fn parse_command(line: &str) -> Result<Command, String> {
    let parts: Vec<&str> = line.split_whitespace().collect();
    match parts.as_slice() {
        ["M", amount] => match amount.parse::<i32>() {
            Ok(number) => Ok(Command::Move(number)),
            Err(error) => Err(format!("bad amount '{amount}': {error}")),
        },
        ["T", degrees] => match degrees.parse::<i32>() {
            Ok(number) => Ok(Command::Turn { degrees: number }),
            Err(error) => Err(format!("bad angle '{degrees}': {error}")),
        },
        ["S"] => Ok(Command::Stop),
        [] => Err(String::from("empty command")),
        _ => Err(format!("unknown command '{line}'")),
    }
}

pub fn lesson_10_results() {
    title("MODULE 10 - Result AND THE ? OPERATOR");

    heading("PART 1: an operation that can fail says so");

    say("  Option says 'there may be nothing'. Result says 'this may have");
    say("  failed, and here is why': Ok(value) or Err(reason).");
    println!();
    say("    \"12\".parse::<i32>()");
    println!("  Running:  {:?}", "12".parse::<i32>());
    for text in ["12x", "", "99999999999"] {
        match text.parse::<i32>() {
            Ok(number) => println!("  Running:  {text:?} -> {number}"),
            Err(error) => println!("  Running:  {text:?} -> Err: {error}"),
        }
    }
    println!("  Running:  \"300\".parse::<u8>() -> {:?}", "300".parse::<u8>());
    println!();
    say("  Three different failures, three different messages. Compare the");
    say("  Arduino's Serial.parseInt(): it returns 0 for 'no number', exactly");
    say("  as for a real 0, and the program cannot tell them apart. Here the");
    say("  type forces the question. The compiler also warns if a Result is");
    say("  simply ignored.");

    wait_enter();
    clear_screen();
    heading("PART 2: ? passes the failure up");

    say("  Handling every Err with a match gets long. Inside a function that");
    say("  itself returns a Result, the ? operator does it: on Ok it gives");
    say("  you the value, on Err it returns that error from the function.");
    println!();
    say("    fn double(text: &str) -> Result<i32, ParseIntError> {");
    say("        let number: i32 = text.trim().parse()?;");
    say("        Ok(number * 2)");
    say("    }");
    println!();
    println!("  Running:  double(\"21\")  = {:?}", double("21"));
    println!("  Running:  double(\" 8 \") = {:?}", double(" 8 "));
    println!("  Running:  double(\"x\")   = {:?}", double("x"));
    println!();
    say("  That one ? replaces a four-line match, and the error reaches whoever");
    say("  called double, who decides what to do. It is C's 'check the return");
    say("  code after every call', done by the compiler and impossible to skip.");
    println!();
    say("  Reading input is the same kind of call: read_line returns a Result.");
    print!("\n  Type a whole number and press ENTER: ");
    io::stdout().flush().ok();
    let mut line = String::new();
    match io::stdin().read_line(&mut line) {
        Ok(0) => println!("\n  Running:  no input at all (end of file)"),
        Ok(_) => println!("  Running:  double({:?}) = {:?}", line.trim(), double(&line)),
        Err(error) => println!("  Running:  could not read: {error}"),
    }

    wait_enter();
    clear_screen();
    heading("PART 3: a command parser, and when to give up");

    say("  Module 9's Command, parsed from text, the way a robot reads a");
    say("  line off a serial port. Anything wrong becomes an Err with a");
    say("  message, never a crash:");
    println!();
    say("    fn parse_command(line: &str) -> Result<Command, String>");
    println!();
    for line in ["M 120", "T -90", "S", "X 1", "M abc", ""] {
        println!("  Running:  {:<8} -> {:?}", format!("{line:?}"), parse_command(line));
    }
    println!();
    say("  The match uses a slice pattern: [\"M\", amount] matches a list of");
    say("  exactly two words, the first being M. .collect() gathers the words");
    say("  into a Vec first, and format! builds a String the way println!");
    say("  prints one.");
    println!();
    say("  For a failure that means the program itself is wrong, .unwrap()");
    say("  and .expect(\"message\") stop it on purpose:");
    println!();
    say("    let n: i32 = \"x\".parse().expect(\"not a number\");");
    println!();
    say("    not a number: ParseIntError { kind: InvalidDigit }");
    println!();
    say("  Handle a Result when the failure is something the program can");
    say("  recover from (bad input). Unwrap it only when the failure would be");
    say("  a bug.");

    wait_enter();
    clear_screen();
    exercise(10);

    question(
        "\"12x\".parse::<i32>() returns Ok or Err?",
        "Err",
        "The x is not a digit, so the parse fails with InvalidDigit.",
    );
    question(
        "Which operator returns early from a function when a Result is Err?\n  (the symbol)",
        "?",
        "On Err it returns the error to the caller; on Ok it unwraps the value.",
    );

    challenge(
        &[
            "Write fn double(text: &str) -> Result<i32, std::num::ParseIntError> that",
            "parses the text and returns twice the number, using the ? operator.",
            "Print double(\"21\") and double(\"x\") with {:?}, each on its own line.",
        ],
        &[],
        &["Ok(42)", "Err(ParseIntError { kind: InvalidDigit })"],
        &[
            "use std::num::ParseIntError;",
            "",
            "fn double(text: &str) -> Result<i32, ParseIntError> {",
            "    let number: i32 = text.parse()?;",
            "    Ok(number * 2)",
            "}",
            "",
            "fn main() {",
            "    println!(\"{:?}\", double(\"21\"));",
            "    println!(\"{:?}\", double(\"x\"));",
            "}",
        ],
    );

    wait_enter();
    clear_screen();
    heading("SUMMARY");

    say("   - Result<T, E> is Ok(value) or Err(reason); the type forces a decision");
    say("   - ? returns the Err to the caller, or unwraps the Ok");
    say("   - handle recoverable failures; unwrap/expect only for bugs");
    say("   - parse() tells a bad number from a zero, unlike parseInt()");
    println!();
    say("  Module 11: Rust meets hardware.");
    wait_enter();
}
