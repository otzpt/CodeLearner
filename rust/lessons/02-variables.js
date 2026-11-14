module.exports = {
  id: "variables",
  title: "Variables and numbers",
  titlePt: "Variáveis e números",
  screens: [
    [
      "let makes a variable: a name for a value. By default it can never change. To allow changes, write let mut:",
      { code: 'fn main() {\n    let age = 25;\n    let mut score = 10;\n    score = score + 5;\n    println!("{} {}", age, score);\n}' },
      '= means "store this", not "is equal to". Changing a variable that was not made with mut is refused, as the next screen shows.',
    ],
    [
      "Here is what happens when a variable without mut gets a new value:",
      { code: 'fn main() {\n    let age = 25;\n    age = 26;\n    println!("{}", age);\n}', error: true },
      "The compiler even says which line to change and how. Making changes something you ask for prevents a whole class of mistakes.",
    ],
    [
      "Every value has a type. Rust works it out, or you write it after a colon. i32 is a whole number, f64 a number with decimals. Dividing two whole numbers drops the decimals; % gives the remainder:",
      { code: 'fn main() {\n    let a: i32 = 7;\n    let b: f64 = 7.0;\n    println!("{} {}", a / 2, a % 2);\n    println!("{}", b / 2.0);\n}' },
      "Rust never mixes types silently: a / 2.0 with a as i32 is refused. Convert with as: a as f64 / 2.0.",
      { quiz: "What is 9 / 4 when both are i32?", choices: ["2.25", "2", "3"], answer: 2, why: "Whole-number division drops the decimals." },
    ],
    [
      "Each type has a range. Going past it is called overflow, and while you are learning, Rust stops the program rather than give a wrong number:",
      { code: 'fn main() {\n    let mut n: u8 = 250;\n    for _ in 0..10 {\n        n += 1;\n    }\n    println!("{}", n);\n}', error: true },
      "u8 holds 0 to 255, so adding 1 ten times to 250 does not fit, and the program panics: Rust's word for stopping on a problem it cannot continue from. The _ in the loop means the count itself is not needed.",
      { tip: "let is fixed; let mut can change. i32 whole numbers, f64 decimals; as converts." },
    ],
    [
      {
        exercise: "variables",
        task: "Make price = 20 and quantity = 3 and print price times quantity. Then make price 25 and print it again. Then print 17 divided by 5 as a whole number, and as f64 with the decimals (3.4).",
        expected: ["60", "75", "3", "3.4"],
        hints: ["price changes, so it needs let mut.", "17.0 / 5.0 keeps the decimals."],
        solution: 'fn main() {\n    let mut price = 20;\n    let quantity = 3;\n    println!("{}", price * quantity);\n    price = 25;\n    println!("{}", price * quantity);\n    println!("{}", 17 / 5);\n    println!("{}", 17.0 / 5.0);\n}',
      },
    ],
  ],
  recapPt: [
    "let cria uma variável que não muda; let mut deixa mudar.",
    "i32 é um número inteiro, f64 tem casas decimais; as converte entre tipos.",
    "Inteiro / inteiro deita fora as casas decimais; % é o resto.",
    "Passar o limite de um tipo (overflow) faz o programa parar enquanto aprendes.",
  ],
  recap: [
    "let makes a variable that does not change; let mut allows changes.",
    "i32 is a whole number, f64 has decimals; as converts between them.",
    "Whole / whole drops the decimals; % is the remainder.",
    "Going past a type's range (overflow) stops the program while you learn.",
  ],
};
