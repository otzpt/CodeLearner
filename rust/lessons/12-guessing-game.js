module.exports = {
  id: "guessing-game",
  title: "Project: a guessing game",
  titlePt: "Projeto: um jogo de adivinhar",
  screens: [
    [
      "Time to put everything together in one real program: a number guessing game.",
      "The program thinks of a secret number. The player guesses; the program says \"Too low\" or \"Too high\" until the guess is right, then says how many guesses it took.",
      "Plan it first:\n1. Keep the secret number and a guess counter.\n2. Repeat until the guess is right: ask, count, say low or high.\n3. Say Correct and the number of guesses.",
    ],
    [
      "loop repeats forever and break leaves it. read_line gives back how many bytes it read; 0 means there is nothing more to read, so stop then too:",
      { code: 'fn main() {\n    let mut n = 0;\n    loop {\n        n += 1;\n        if n == 3 {\n            break;\n        }\n    }\n    println!("{}", n);\n}' },
      "Comparing two numbers can be done with match on a.cmp(&b), which gives Less, Greater or Equal, from std::cmp::Ordering.",
    ],
    [
      {
        exercise: "game",
        task: 'Write the game with the secret number 42, so the checker knows the answer. Ask with "Guess: ". Print Too low, Too high, or at the end Correct! It took N guesses.',
        expected: ["Guess: {50}", "Too high", "Guess: {25}", "Too low", "Guess: {42}", "Correct! It took 3 guesses."],
        starter: "use std::io::{self, Write};\n\nfn main() {\n    let secret = 42;\n    // your code here\n}",
        hints: [
          "let mut guesses = 0; loop { ... }",
          'print!("Guess: "); flush; let mut text = String::new(); if io::stdin().read_line(&mut text).unwrap() == 0 { break; }',
          "let guess: i32 = text.trim().parse().unwrap(); guesses += 1; then if/else if/else.",
        ],
        solution: 'use std::io::{self, Write};\n\nfn main() {\n    let secret = 42;\n    let mut guesses = 0;\n    loop {\n        print!("Guess: ");\n        io::stdout().flush().unwrap();\n        let mut text = String::new();\n        if io::stdin().read_line(&mut text).unwrap() == 0 {\n            break;\n        }\n        let guess: i32 = text.trim().parse().unwrap();\n        guesses += 1;\n        if guess < secret {\n            println!("Too low");\n        } else if guess > secret {\n            println!("Too high");\n        } else {\n            println!("Correct! It took {} guesses.", guesses);\n            break;\n        }\n    }\n}',
      },
    ],
    [
      "Once it passes, make it a real game. Rust's standard library has no random numbers; real projects add a package for that with cargo. Without one, the current time in nanoseconds is a usable stand-in:",
      { code: 'use std::time::{SystemTime, UNIX_EPOCH};\n\nfn main() {\n    let nanos = SystemTime::now().duration_since(UNIX_EPOCH).unwrap().subsec_nanos();\n    let secret = nanos % 100 + 1;\n    println!("{}", secret >= 1 && secret <= 100);\n}' },
      "Build it and play it yourself. The checker cannot test this version, because it would not know the secret.",
    ],
  ],
  recapPt: [
    "Planeia primeiro em palavras simples, depois passa cada passo a código.",
    "loop com break repete até o código decidir parar.",
    "read_line devolve 0 quando não há mais nada para ler.",
    "A biblioteca padrão não tem números aleatórios; o relógio serve para um jogo.",
  ],
  recap: [
    "Plan in plain words first, then turn each step into code.",
    "loop with break repeats until the code decides to stop.",
    "read_line gives 0 when there is nothing more to read.",
    "The standard library has no random numbers; the clock will do for a game.",
  ],
};
