module.exports = {
  id: "first-program",
  title: "Your first program",
  titlePt: "O teu primeiro programa",
  screens: [
    [
      "A program is a list of instructions for the computer, written in a text file. Rust is one language for writing them.",
      "The computer cannot read Rust directly. The compiler, rustc, first translates your file into a program the computer can run, then you run that. In a terminal, it is two steps:",
      { code: "rustc hello.rs\n./hello", run: false },
      "The Rust compiler is strict and explains its errors in detail. Every example in this course is compiled and run while you read it.",
    ],
    [
      "Here is a whole Rust program. fn main is where it starts, and its body is between { and }. println! prints a line:",
      { code: 'fn main() {\n    println!("Hello!");\n    println!("A second line");\n}' },
      "The ! means println! is a macro, a tool that writes code for you. For now, just remember the ! is part of its name.",
    ],
    [
      "{} inside the text is a slot, filled by the values after it, in order. Numbers need no quotes, and a sum is worked out first:",
      { code: 'fn main() {\n    println!("two plus three is {}", 2 + 3);\n    println!("{} and {}", "first", 42);\n    println!("2 + 3");\n}' },
      "In quotes, 2 + 3 is just text. // starts a comment that Rust ignores.",
      { quiz: 'What does println!("{}", 10 + 5); print?', choices: ["15", "10 + 5", "{}"], answer: 1, why: "The slot is filled with the value 10 + 5, which is worked out first." },
    ],
    [
      "Every statement ends with a semicolon. Without it, the compiler refuses and points at the exact spot:",
      { code: 'fn main() {\n    println!("hi")\n    println!("there");\n}', error: true },
      "Reading the compiler's messages is a normal part of programming. Rust's usually say what to change.",
      { tip: "fn main() { } is where it starts. println!(\"{}\", value) fills the slot." },
    ],
    [
      {
        exercise: "hello",
        task: "Write a program that prints your name on one line and the number 2026 on the next. For the checker, use the name Ana.",
        expected: ["Ana", "2026"],
        hints: ["The file already has fn main. Write two println! lines inside it.", 'println!("{}", 2026); or println!("2026");'],
        solution: 'fn main() {\n    println!("Ana");\n    println!("{}", 2026);\n}',
      },
    ],
  ],
  recapPt: [
    "rustc traduz o ficheiro .rs num programa; depois corres esse programa.",
    "fn main() { } é onde o programa começa.",
    "println!(\"texto {}\", valor) mostra uma linha; {} é preenchido pelo valor.",
    "Cada instrução acaba em ;.",
  ],
  recap: [
    "rustc turns your .rs file into a program; then you run that program.",
    "fn main() { } is where the program starts.",
    "println!(\"text {}\", value) prints a line; {} is filled by the value.",
    "Every statement ends with ;.",
  ],
};
