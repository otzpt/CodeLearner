module.exports = {
  id: "decisions",
  title: "Making decisions with if",
  screens: [
    [
      "A comparison gives true or false:",
      { code: "console.log(5 > 3);\nconsole.log(4 === 4);\nconsole.log(4 !== 4);" },
      "=== is \"equal to\", !== is \"not equal to\". A single = stores a value.",
    ],
    [
      "if runs its block only when the condition is true. Only the first true branch runs:",
      { code: 'const score = 72;\nif (score >= 90) {\n  console.log("A");\n} else if (score >= 50) {\n  console.log("pass");\n} else {\n  console.log("fail");\n}' },
    ],
    [
      "== with two characters changes types before comparing:",
      { code: 'console.log(1 == "1");\nconsole.log(1 === "1");' },
      "Always use === and !==.",
      { quiz: 'What does 7 === "7" give?', choices: ["true", "false"], answer: 2, why: "A number and a string are never === ." },
    ],
    [
      "&& is \"and\", || is \"or\":",
      { code: "const age = 20;\nconsole.log(age >= 18 && age < 65);\nconsole.log(age < 13 || age > 65);" },
    ],
    [
      "A common slip: = instead of === in an if. No error, but x changes:",
      { code: 'let x = 3;\nif (x = 0) {\n  console.log("x is 0");\n}\nconsole.log(x);' },
      "x = 0 stored 0, and an if treats 0 as false. Nothing was compared.",
    ],
    [
      {
        exercise: "grade",
        task: 'With const points = 64: print "great" for 80 or more, "good" for 50 or more, else "try again". Then print whether points is even.',
        expected: ["good", "true"],
        hints: ["if / else if / else, checking 80 first.", "points % 2 === 0"],
        solution: 'const points = 64;\nif (points >= 80) {\n  console.log("great");\n} else if (points >= 50) {\n  console.log("good");\n} else {\n  console.log("try again");\n}\nconsole.log(points % 2 === 0);',
      },
    ],
  ],
  recap: [
    "> < >= <= === !== give true or false.",
    "if / else if / else runs only the first true branch.",
    "Use ===, never ==.",
    "&& is and, || is or.",
    "= inside if stores instead of comparing.",
  ],
};
