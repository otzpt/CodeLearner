module.exports = {
  id: "enums-and-match",
  title: "Choices: enum, match and Option",
  titlePt: "Escolhas: enum, match e Option",
  screens: [
    [
      "An enum is a type that is exactly one of a fixed list of choices. match looks at a value and runs the branch for the choice it is:",
      { code: 'enum Light {\n    Red,\n    Yellow,\n    Green,\n}\n\nfn action(light: Light) -> &\'static str {\n    match light {\n        Light::Red => "stop",\n        Light::Yellow => "slow down",\n        Light::Green => "go",\n    }\n}\n\nfn main() {\n    for light in [Light::Red, Light::Yellow, Light::Green] {\n        println!("{}", action(light));\n    }\n}' },
      "&'static str is the type of text written straight in the program. match must cover every choice; forget one and the compiler refuses.",
    ],
    [
      "Rust has no null, no \"nothing\" value that can sneak in anywhere. Instead, a value that might be missing is an Option: either Some(value) or None. .get(i) on a vector gives one:",
      { code: 'fn main() {\n    let scores = vec![15, 12, 18];\n    for i in [1, 5] {\n        match scores.get(i) {\n            Some(s) => println!("position {} is {}", i, s),\n            None => println!("no position {}", i),\n        }\n    }\n}' },
      "Some(s) => ... runs with s set to the value inside. The compiler makes you handle None, so a missing value cannot be forgotten.",
      { quiz: "What does vec![1, 2].get(7) give?", choices: ["an error", "None", "0"], answer: 2, why: ".get gives None for a position that is not there." },
    ],
    [
      "if let is a short match for when only one choice matters:",
      { code: 'fn main() {\n    let names = vec!["Ana", "Rui"];\n    if let Some(first) = names.first() {\n        println!("first is {}", first);\n    }\n}' },
      { tip: "match must cover every choice. Option is Some(value) or None; handle both." },
    ],
    [
      {
        exercise: "match",
        task: "Write fn grade(points: i32) -> &'static str using match with ranges: 90..=100 gives \"A\", 50..=89 gives \"pass\", anything else (_) gives \"fail\". Print grade(95), grade(64) and grade(12).",
        expected: ["A", "pass", "fail"],
        hints: ["match points { 90..=100 => \"A\", 50..=89 => \"pass\", _ => \"fail\" }", "_ matches anything not matched above."],
        solution: 'fn grade(points: i32) -> &\'static str {\n    match points {\n        90..=100 => "A",\n        50..=89 => "pass",\n        _ => "fail",\n    }\n}\n\nfn main() {\n    println!("{}", grade(95));\n    println!("{}", grade(64));\n    println!("{}", grade(12));\n}',
      },
    ],
  ],
  recapPt: [
    "enum é um tipo que é exatamente uma de várias escolhas.",
    "match corre o ramo da escolha; tem de cobrir todas (_ apanha o resto).",
    "Option é Some(valor) ou None: o Rust não tem null.",
    "if let Some(x) = ... trata só o caso que interessa.",
  ],
  recap: [
    "An enum is a type that is exactly one of several choices.",
    "match runs the branch for the choice; it must cover all of them (_ catches the rest).",
    "Option is Some(value) or None: Rust has no null.",
    "if let Some(x) = ... handles just the case you care about.",
  ],
};
