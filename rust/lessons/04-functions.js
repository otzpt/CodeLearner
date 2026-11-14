module.exports = {
  id: "functions",
  title: "Functions",
  titlePt: "Funções",
  screens: [
    [
      "fn makes a function. Each parameter has a type, and -> says the type it gives back. The last line, written without a semicolon, is the value given back:",
      { code: 'fn area(width: i32, height: i32) -> i32 {\n    width * height\n}\n\nfn main() {\n    println!("{}", area(3, 4));\n    println!("{}", area(10, 2) + 1);\n}' },
      "Writing the function does not run it. Only area(3, 4) runs it. return value; also works, and is used to leave early.",
    ],
    [
      "A semicolon on that last line turns it into a statement that gives nothing back, and the types no longer match:",
      { code: "fn double(n: i32) -> i32 {\n    n * 2;\n}\n\nfn main() {\n    println!(\"{}\", double(4));\n}", error: true },
      "The compiler points at the semicolon and suggests removing it.",
      { quiz: "In a function ending with x + 1 (no semicolon), what does it give back?", choices: ["nothing", "the value of x + 1"], answer: 2, why: "The last expression without ; is the value given back." },
    ],
    [
      "A function with no -> gives back nothing. It is fine for functions that only print:",
      { code: 'fn greet(name: &str) {\n    println!("Hello, {}!", name);\n}\n\nfn main() {\n    greet("Ana");\n    greet("Rui");\n}' },
      "&str is the type of a piece of text. Lesson 6 explains the &.",
      { tip: "fn name(param: Type) -> Type { last expression }. A ; on the last line gives nothing back." },
    ],
    [
      {
        exercise: "functions",
        task: "Write fn is_even(n: i32) -> bool and fn largest(a: i32, b: i32) -> i32. Print is_even(7), is_even(10) and largest(3, 9), one per line.",
        expected: ["false", "true", "9"],
        hints: ["fn is_even(n: i32) -> bool { n % 2 == 0 }", "fn largest(a: i32, b: i32) -> i32 { if a > b { a } else { b } }"],
        solution: 'fn is_even(n: i32) -> bool {\n    n % 2 == 0\n}\n\nfn largest(a: i32, b: i32) -> i32 {\n    if a > b { a } else { b }\n}\n\nfn main() {\n    println!("{}", is_even(7));\n    println!("{}", is_even(10));\n    println!("{}", largest(3, 9));\n}',
      },
    ],
  ],
  recapPt: [
    "fn nome(param: Tipo) -> Tipo { ... } define uma função.",
    "A última expressão sem ; é o valor devolvido; return sai mais cedo.",
    "Um ; nessa linha faz a função não devolver nada.",
    "Sem -> a função não devolve nada.",
  ],
  recap: [
    "fn name(param: Type) -> Type { ... } defines a function.",
    "The last expression without ; is the value given back; return leaves early.",
    "A ; on that line makes the function give nothing back.",
    "Without -> the function gives nothing back.",
  ],
};
