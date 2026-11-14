module.exports = {
  id: "decisions",
  title: "Making decisions with if",
  titlePt: "Tomar decisões com if",
  screens: [
    [
      "Comparing two values gives true or false. These two values are called booleans.",
      { code: "console.log(5 > 3);\nconsole.log(5 < 3);\nconsole.log(5 >= 5);\nconsole.log(4 === 4);\nconsole.log(4 !== 4);" },
      "=== means \"is equal to\" and !== means \"is not equal to\". Three characters, not one: a single = stores a value, as you saw with variables.",
    ],
    [
      "if runs a block of code only when a condition is true. The condition goes in ( ), the block in { }. else gives the block to run when it is false, and else if checks another condition in between:",
      { code: 'const score = 72;\nif (score >= 90) {\n  console.log("A");\n} else if (score >= 50) {\n  console.log("pass");\n} else {\n  console.log("fail");\n}' },
      "JavaScript checks from the top and runs only the first block whose condition is true. Then it skips the rest.",
    ],
    [
      "There is also ==, with two characters. It changes types before comparing, which gives surprises:",
      { code: 'console.log(1 == "1");\nconsole.log(1 === "1");\nconsole.log(0 == "");' },
      "Always use === and !==. They never change types, so a number is never equal to a string.",
      { quiz: 'What does 7 === "7" give?', choices: ["true", "false"], answer: 2, why: "One is a number and one is a string, and === never changes types." },
    ],
    [
      "&& means \"and\": both sides must be true. || means \"or\": at least one side must be true.",
      { code: "const age = 20;\nconsole.log(age >= 18 && age < 65);\nconsole.log(age < 13 || age > 65);" },
    ],
    [
      "A common slip: writing = instead of === inside if. JavaScript does not complain, it stores the value instead of comparing:",
      { code: 'let x = 3;\nif (x = 0) {\n  console.log("x is 0");\n}\nconsole.log(x);' },
      'Nothing about "x is 0" was printed, yet x is now 0. x = 0 put 0 into x, and an if treats 0 as false, so the block was skipped. No comparison happened at all.',
      { tip: "Inside if, compare with === . A single = changes the variable." },
    ],
    [
      {
        exercise: "grade",
        task: "Create const points = 64. Print \"great\" if points is 80 or more, \"good\" if it is 50 or more, and \"try again\" otherwise. Then, on a new line, print whether points is an even number (true or false).",
        expected: ["good", "true"],
        hints: ["Use if, else if and else, checking 80 first.", "points % 2 === 0 is true for an even number."],
        solution: 'const points = 64;\nif (points >= 80) {\n  console.log("great");\n} else if (points >= 50) {\n  console.log("good");\n} else {\n  console.log("try again");\n}\nconsole.log(points % 2 === 0);',
      },
    ],
  ],
  recapPt: [
    "As comparações dão true ou false: > < >= <= === !==",
    "if (condição) { ... } else if (...) { ... } else { ... } corre só o primeiro bloco verdadeiro.",
    "Usa === e !==, nunca == e !=.",
    "&& é \"e\", || é \"ou\".",
    "= dentro de um if guarda em vez de comparar, e o JavaScript não avisa.",
  ],
  recap: [
    "Comparisons give true or false: > < >= <= === !==",
    "if (condition) { ... } else if (...) { ... } else { ... } runs only the first true block.",
    "Use === and !==, never == and !=.",
    "&& is and, || is or.",
    "= inside if stores instead of comparing, and JavaScript does not warn you.",
  ],
};
