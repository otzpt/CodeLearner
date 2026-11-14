# Rust course

For someone who has never programmed. Plain `rustc`, no Cargo.

## Lessons

1. Your first program
2. Variables and numbers
3. Decisions and loops
4. Functions
5. Text and lists
6. Ownership and borrowing
7. Reading what the user types
8. Structs: values that belong together
9. Choices: enum, match and Option
10. When things go wrong: Result
11. Reading and writing files
12. Project: a guessing game

## Running the examples yourself

```bash
rustc --crate-name main -o program main.rs && ./program
```

Needs `rustc`. Examples build without warnings; an example that would warn
about something unrelated to the lesson is changed until it does not.

## Running

From the launcher, or directly:

```bash
node rust/learn.js
```

Your practice files and progress go in `~/codelearner/rust/`.

## Checking

```bash
node learn/check.js rust            # every example, question, solution and starter
python3 learn/check-course.py rust  # every lesson played to the end
```

