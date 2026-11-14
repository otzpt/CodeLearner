module.exports = {
  id: "decisions-and-loops",
  title: "Decisions and loops",
  titlePt: "Decisões e ciclos",
  screens: [
    [
      "if runs a block only when its condition is true; else if checks another; else runs when none was. No brackets around the condition. == compares, != means \"not equal\", && is \"and\", || is \"or\":",
      { code: 'fn main() {\n    let score = 72;\n    if score >= 90 {\n        println!("A");\n    } else if score >= 50 {\n        println!("pass");\n    } else {\n        println!("fail");\n    }\n}' },
      "Rust checks from the top and runs only the first block whose condition is true.",
    ],
    [
      "if gives back a value, so it can choose what to store:",
      { code: 'fn main() {\n    let age = 20;\n    let group = if age >= 18 { "adult" } else { "child" };\n    println!("{}", group);\n}' },
      "A condition must be true or false (the type bool). if x = 0 is refused, so that slip cannot happen.",
    ],
    [
      "for goes through a range. 1..4 counts 1, 2, 3, stopping before the end; 1..=4 includes it:",
      { code: 'fn main() {\n    let mut total = 0;\n    for i in 1..=4 {\n        total += i;\n    }\n    println!("{}", total);\n}' },
      "total += i is short for total = total + i. while condition { } repeats as long as it is true, loop { } repeats forever, and break leaves any loop.",
      { quiz: "How many times does for i in 1..4 run?", choices: ["3", "4"], answer: 1, why: "1..4 stops before 4: 1, 2, 3." },
    ],
    [
      {
        exercise: "count",
        task: 'Print the numbers 1 to 5, one per line. Then print the sum of the numbers 1 to 100. Then print "even" or "odd" for 2026.',
        expected: ["1", "2", "3", "4", "5", "5050", "even"],
        hints: ["for i in 1..=5", "2026 % 2 == 0 means even."],
        solution: 'fn main() {\n    for i in 1..=5 {\n        println!("{}", i);\n    }\n    let mut total = 0;\n    for i in 1..=100 {\n        total += i;\n    }\n    println!("{}", total);\n    if 2026 % 2 == 0 {\n        println!("even");\n    } else {\n        println!("odd");\n    }\n}',
      },
    ],
  ],
  recapPt: [
    "if condição { } else if { } else { } corre só o primeiro bloco verdadeiro, sem parênteses.",
    "if devolve um valor: let x = if ... { a } else { b };",
    "for i in 1..4 conta 1, 2, 3; 1..=4 inclui o 4.",
    "while, loop e break: repetir enquanto, para sempre, e sair.",
  ],
  recap: [
    "if condition { } else if { } else { } runs only the first true block, no brackets.",
    "if gives a value: let x = if ... { a } else { b };",
    "for i in 1..4 counts 1, 2, 3; 1..=4 includes 4.",
    "while, loop and break: repeat while, repeat forever, and leave.",
  ],
};
