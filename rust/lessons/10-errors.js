module.exports = {
  id: "errors",
  title: "When things go wrong: Result",
  titlePt: "Quando as coisas correm mal: Result",
  screens: [
    [
      "Something that can fail gives back a Result: either Ok(value) or Err(problem). parse is one. match handles both, so the program carries on instead of stopping:",
      { code: 'fn main() {\n    for text in ["42", "hello"] {\n        match text.parse::<i32>() {\n            Ok(n) => println!("number {}", n),\n            Err(e) => println!("not a number: {} ({})", text, e),\n        }\n    }\n}' },
      "parse::<i32>() says which type to parse into, when there is no variable type to tell it.",
    ],
    [
      "Your own functions can give back a Result too. The ? after a call means \"if this is an Err, give that Err back from my function right now\":",
      { code: 'fn total(a: &str, b: &str) -> Result<i32, std::num::ParseIntError> {\n    let x: i32 = a.parse()?;\n    let y: i32 = b.parse()?;\n    Ok(x + y)\n}\n\nfn main() {\n    println!("{:?}", total("2", "3"));\n    println!("{:?}", total("2", "three"));\n}' },
      "{:?} prints the Result itself, so you see Ok(5) or the Err.",
      { quiz: "What does ? do after a call that gave an Err?", choices: ["stops the program", "gives the Err back from the current function", "ignores it"], answer: 2, why: "? passes the Err up to the caller." },
    ],
    [
      "For your own errors, a String is the simplest problem type. unwrap() is the opposite of handling: it stops the program on an Err:",
      { code: 'fn withdraw(balance: i32, amount: i32) -> Result<i32, String> {\n    if amount > balance {\n        return Err(String::from("not enough money"));\n    }\n    Ok(balance - amount)\n}\n\nfn main() {\n    println!("{}", withdraw(100, 30).unwrap());\n    println!("{}", withdraw(100, 500).unwrap());\n}', error: true },
      { tip: "Result is Ok(value) or Err(problem). match to handle; ? to pass it up; unwrap only when failure is impossible." },
    ],
    [
      {
        exercise: "errors",
        task: 'Write fn divide(a: f64, b: f64) -> Result<f64, String> giving Err("cannot divide by 0") when b is 0.0, else Ok(a / b). For (10.0, 4.0) and then (1.0, 0.0), match the result: print the number, or "Error: " and the message. Then print "done".',
        expected: ["2.5", "Error: cannot divide by 0", "done"],
        hints: ['if b == 0.0 { return Err(String::from("cannot divide by 0")); }', 'match divide(a, b) { Ok(v) => println!("{}", v), Err(e) => println!("Error: {}", e) }'],
        solution: 'fn divide(a: f64, b: f64) -> Result<f64, String> {\n    if b == 0.0 {\n        return Err(String::from("cannot divide by 0"));\n    }\n    Ok(a / b)\n}\n\nfn main() {\n    for (a, b) in [(10.0, 4.0), (1.0, 0.0)] {\n        match divide(a, b) {\n            Ok(v) => println!("{}", v),\n            Err(e) => println!("Error: {}", e),\n        }\n    }\n    println!("done");\n}',
      },
    ],
  ],
  recapPt: [
    "Algo que pode falhar devolve Result: Ok(valor) ou Err(problema).",
    "match trata os dois casos e o programa continua.",
    "? depois de uma chamada devolve logo o Err a quem chamou.",
    "unwrap() para o programa num Err: só quando a falha é impossível.",
  ],
  recap: [
    "Something that can fail gives a Result: Ok(value) or Err(problem).",
    "match handles both and the program carries on.",
    "? after a call gives the Err straight back to the caller.",
    "unwrap() stops the program on an Err: only when failure is impossible.",
  ],
};
