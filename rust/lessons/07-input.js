module.exports = {
  id: "input",
  title: "Reading what the user types",
  titlePt: "Ler o que o utilizador escreve",
  screens: [
    [
      "std::io::stdin().read_line adds the line the user types to a String, including the ENTER at the end. trim() removes that. print! (without ln) does not end the line, and flush makes it appear before waiting:",
      { code: 'use std::io::{self, Write};\n\nfn main() {\n    print!("What is your name? ");\n    io::stdout().flush().unwrap();\n    let mut name = String::new();\n    io::stdin().read_line(&mut name).unwrap();\n    println!("Hello, {}!", name.trim());\n}', output: ["What is your name? {Ana}", "Hello, Ana!"] },
      "The bold part is what was typed. use at the top brings io and Write into the file. .unwrap() means \"stop if this failed\"; lesson 9 shows gentler ways.",
    ],
    [
      "What you read is always text. .parse() turns it into a number, and the type goes on the variable so Rust knows which kind of number:",
      { code: 'use std::io;\n\nfn main() {\n    let mut text = String::new();\n    io::stdin().read_line(&mut text).unwrap();\n    let age: i32 = text.trim().parse().unwrap();\n    println!("Next year you will be {}", age + 1);\n}', output: ["{20}", "Next year you will be 21"] },
    ],
    [
      "If the text is not a number, parse fails, and unwrap stops the program:",
      { code: 'use std::io;\n\nfn main() {\n    let mut text = String::new();\n    io::stdin().read_line(&mut text).unwrap();\n    let age: i32 = text.trim().parse().unwrap();\n    println!("{}", age);\n}', output: ["{twenty}"], error: true },
      { quiz: "Why call .trim() before .parse()?", choices: ["to make it faster", "to remove the ENTER at the end of the line, which is not a digit"], answer: 2, why: "read_line keeps the new line, and \"20\\n\" is not a number." },
    ],
    [
      {
        exercise: "ask",
        task: 'Ask "Width? " and "Height? " (whole numbers) and print: Area: W*H',
        expected: ["Width? {4}", "Height? {5}", "Area: 20"],
        starter: "use std::io::{self, Write};\n\nfn main() {\n    // your code here\n}",
        hints: ["For each: print!, flush, read_line into a new String, then trim().parse().unwrap() into an i32.", 'println!("Area: {}", width * height);'],
        solution: 'use std::io::{self, Write};\n\nfn ask(question: &str) -> i32 {\n    print!("{}", question);\n    io::stdout().flush().unwrap();\n    let mut text = String::new();\n    io::stdin().read_line(&mut text).unwrap();\n    text.trim().parse().unwrap()\n}\n\nfn main() {\n    let width = ask("Width? ");\n    let height = ask("Height? ");\n    println!("Area: {}", width * height);\n}',
      },
    ],
  ],
  recapPt: [
    "io::stdin().read_line(&mut s) junta a linha escrita, com o ENTER, a um String.",
    ".trim() tira o ENTER; .parse() transforma em número (diz o tipo na variável).",
    "print! não muda de linha; io::stdout().flush() mostra-o já.",
    ".unwrap() para o programa se algo falhar.",
  ],
  recap: [
    "io::stdin().read_line(&mut s) adds the typed line, ENTER included, to a String.",
    ".trim() removes the ENTER; .parse() makes a number (say the type on the variable).",
    "print! does not end the line; io::stdout().flush() shows it now.",
    ".unwrap() stops the program if something failed.",
  ],
};
