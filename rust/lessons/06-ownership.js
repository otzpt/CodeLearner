module.exports = {
  id: "ownership",
  title: "Ownership and borrowing",
  titlePt: "Posse e empréstimo",
  screens: [
    [
      "This is the idea Rust is built on. Every value has one owner, a variable. When the owner's block ends, the value is cleaned up. Giving a String to another variable moves it: the old name no longer owns anything:",
      { code: 'fn main() {\n    let a = String::from("hello");\n    let b = a;\n    println!("{}", a);\n    println!("{}", b);\n}', error: true },
      "Simple numbers are copied instead of moved, so this only happens with values like String and Vec.",
    ],
    [
      "Passing a String to a function moves it too. To let a function look at a value without taking it, lend it with &. That is called borrowing:",
      { code: 'fn shout(text: &String) -> String {\n    text.to_uppercase()\n}\n\nfn main() {\n    let word = String::from("hello");\n    let loud = shout(&word);\n    println!("{} {}", word, loud);\n}' },
      "word still owns its text after the call, because shout only borrowed it. .clone() makes a full copy, when you really need two.",
    ],
    [
      "&mut lends a value so the borrower can change it. The variable must itself be mut:",
      { code: 'fn add_world(text: &mut String) {\n    text.push_str(" world");\n}\n\nfn main() {\n    let mut greeting = String::from("hello");\n    add_world(&mut greeting);\n    println!("{}", greeting);\n}' },
      "The rule: at any time, either any number of & borrows, or exactly one &mut. Never both. That is how Rust stops two parts of a program changing the same thing at once.",
      { quiz: "let a = String::from(\"x\"); let b = a; Can you still print a?", choices: ["yes", "no, the value moved to b"], answer: 2, why: "A String moves; a no longer owns anything." },
    ],
    [
      {
        exercise: "ownership",
        task: 'Write fn count_vowels(text: &String) -> i32 and fn add_exclamation(text: &mut String). With let mut s = String::from("banana"): print count_vowels(&s), call add_exclamation(&mut s), then print s.',
        expected: ["3", "banana!"],
        hints: ["for c in text.chars() goes through each letter; \"aeiou\".contains(c) checks a vowel.", "add_exclamation: text.push_str(\"!\");"],
        solution: 'fn count_vowels(text: &String) -> i32 {\n    let mut count = 0;\n    for c in text.chars() {\n        if "aeiou".contains(c) {\n            count += 1;\n        }\n    }\n    count\n}\n\nfn add_exclamation(text: &mut String) {\n    text.push_str("!");\n}\n\nfn main() {\n    let mut s = String::from("banana");\n    println!("{}", count_vowels(&s));\n    add_exclamation(&mut s);\n    println!("{}", s);\n}',
      },
    ],
  ],
  recapPt: [
    "Cada valor tem um dono; quando o bloco do dono acaba, o valor é limpo.",
    "Dar um String a outra variável ou função move-o; o nome antigo deixa de o ter.",
    "& empresta para ler; &mut empresta para mudar; .clone() copia.",
    "Ou vários &, ou um só &mut, nunca os dois ao mesmo tempo.",
  ],
  recap: [
    "Every value has one owner; when the owner's block ends, the value is cleaned up.",
    "Giving a String to another variable or function moves it; the old name loses it.",
    "& lends to read; &mut lends to change; .clone() copies.",
    "Either any number of &, or one &mut, never both at once.",
  ],
};
