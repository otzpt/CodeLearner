//! ui.rs - the pieces every lesson uses to draw the screen.
//!
//! Same purpose and the same visual style as the other courses' ui modules,
//! kept as a separate implementation rather than shared code.

use std::io::{self, Write};

const WIDTH: usize = 54; // inside width of the frame, matching the other courses

fn flush() {
    // Nothing useful to do if the terminal has gone away.
    io::stdout().flush().ok();
}

/// Clear the screen with ANSI codes, not a spawned process, for the same
/// reason as every course: no dependency on TERM being set.
pub fn clear_screen() {
    print!("\x1b[H\x1b[2J\x1b[3J");
    flush();
}

/// Print one line of lesson text exactly as given. `println!` would treat
/// the braces in code samples as placeholders; passing the text as an
/// argument does not.
pub fn say(text: &str) {
    println!("{text}");
}

/// Read one line without its line ending. `None` at end of input.
pub fn read_line() -> Option<String> {
    flush();
    let mut line = String::new();
    match io::stdin().read_line(&mut line) {
        Ok(0) | Err(_) => None,
        Ok(_) => Some(line.trim_end_matches(['\n', '\r']).to_string()),
    }
}

pub fn wait_enter() {
    print!("\n  Press ENTER to continue...");
    read_line();
}

pub fn rule() {
    println!("  {}", "-".repeat(WIDTH));
}

fn frame(fill: char) {
    println!("  +{}+", fill.to_string().repeat(WIDTH - 2));
}

fn padded_line(text: &str, border: char) {
    println!("  {border} {text:<width$}{border}", width = WIDTH - 3);
}

pub fn title(text: &str) {
    println!();
    frame('=');
    padded_line(text, '|');
    frame('=');
    println!();
}

pub fn heading(text: &str) {
    println!("\n  {text}");
    rule();
}

pub fn ask_yes(question_text: &str) -> bool {
    print!("\n  {question_text} (y/N): ");
    match read_line() {
        Some(answer) => matches!(answer.chars().next(), Some('y') | Some('Y')),
        None => false,
    }
}

pub fn exercise(number: u32) {
    println!("\n  >> EXERCISE - MODULE {number}");
    rule();
}

/// A short-answer question. Compares ignoring case and surrounding spaces,
/// and shows `why` either way. Nobody is blocked from continuing.
pub fn question(text: &str, correct: &str, why: &str) -> bool {
    println!("\n  {text}");
    print!("  Your answer: ");
    let answer = read_line().unwrap_or_default();

    let right = answer.trim().to_lowercase() == correct.trim().to_lowercase();
    if right {
        println!("\n  CORRECT.  {why}");
    } else {
        println!("\n  NOT QUITE. The answer is: {correct}");
        println!("             {why}");
    }
    right
}

/// A task to write in a real file.
///
/// `input` is every line the program will read while producing `expected`,
/// empty for a task that reads nothing. Together they are the actual
/// specification: run the solution, type `input`, get `expected`, however
/// the code that does it is written. `solution` is one way of getting there
/// and appears only after a confirmation.
pub fn challenge(task: &[&str], input: &[&str], expected: &[&str], solution: &[&str]) {
    println!("\n  >> WRITE THIS YOURSELF, in a real file");
    rule();

    for line in task {
        println!("  {line}");
    }

    if !input.is_empty() {
        println!("\n  Try it with this input:\n");
        for line in input {
            println!("      {line}");
        }
    }

    if !expected.is_empty() {
        println!("\n  It must print:\n");
        for line in expected {
            println!("      {line}");
        }
        println!("\n  That output is the whole specification. Any code that");
        println!("  produces it is correct.");
    }

    println!("\n  Try it first. Compile and run with:");
    println!("    rustc test.rs && ./test");

    if !ask_yes("Want to see example code?") {
        return;
    }

    println!();
    rule();
    for line in solution {
        println!("  {line}");
    }
    rule();
    println!("  This is EXAMPLE CODE, not the answer. It is one way to get");
    println!("  that output; yours may look nothing like it and still be");
    println!("  right -- or better. Compare the output, not the code.");
}
