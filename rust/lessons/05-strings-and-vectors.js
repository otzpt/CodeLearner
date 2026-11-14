module.exports = {
  id: "strings-and-vectors",
  title: "Text and lists",
  titlePt: "Texto e listas",
  screens: [
    [
      "String is text you can grow. String::from makes one from a fixed piece of text; push_str adds to it; len() counts its bytes, which is the number of letters for plain text:",
      { code: 'fn main() {\n    let mut name = String::from("Ana");\n    name.push_str(" Silva");\n    println!("{}", name);\n    println!("{}", name.len());\n    println!("{}", name.to_uppercase());\n}' },
      "format! builds a String the same way println! prints: let label = format!(\"score {}\", 42);",
    ],
    [
      "A Vec holds many values of one type, in order, and grows. vec![...] makes one; push adds to the end; positions start at 0:",
      { code: 'fn main() {\n    let mut scores = vec![15, 12, 18];\n    scores.push(20);\n    println!("{} {}", scores[0], scores[3]);\n    println!("{}", scores.len());\n    for s in &scores {\n        println!("score {}", s);\n    }\n}' },
      "for s in &scores goes through every value. The & lets the loop look at them without taking the vector away; lesson 6 explains.",
    ],
    [
      "Rust checks every position. Going past the end stops the program with a clear message:",
      { code: 'fn main() {\n    let scores = vec![15, 12, 18];\n    println!("{}", scores[3]);\n}', error: true },
      ".get(i) is the gentle version: it gives Some(value), or None when the position is not there. Lesson 8 shows how to use that.",
      { quiz: "A vector has 3 values. What is the last valid position?", choices: ["3", "2"], answer: 2, why: "Positions go from 0 to len() - 1." },
    ],
    [
      {
        exercise: "vectors",
        task: 'Make a vector of the words "rice", "beans", "oil" and push "salt". Print how many items there are, then every item on its own line starting with "- ".',
        expected: ["4", "- rice", "- beans", "- oil", "- salt"],
        hints: ['let mut items = vec!["rice", "beans", "oil"]; then items.push("salt");', 'for item in &items { println!("- {}", item); }'],
        solution: 'fn main() {\n    let mut items = vec!["rice", "beans", "oil"];\n    items.push("salt");\n    println!("{}", items.len());\n    for item in &items {\n        println!("- {}", item);\n    }\n}',
      },
    ],
  ],
  recapPt: [
    "String::from(\"...\") cria texto que cresce; push_str junta; format! constrói um String.",
    "vec![1, 2, 3] cria um Vec; push junta ao fim; len() conta.",
    "As posições vão de 0 a len() - 1; passar do fim para o programa.",
    "for x in &v percorre os valores.",
  ],
  recap: [
    "String::from(\"...\") makes growable text; push_str adds; format! builds a String.",
    "vec![1, 2, 3] makes a Vec; push adds to the end; len() counts.",
    "Positions go from 0 to len() - 1; going past the end stops the program.",
    "for x in &v visits every value.",
  ],
};
