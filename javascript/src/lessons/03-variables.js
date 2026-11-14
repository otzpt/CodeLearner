module.exports = {
  id: "variables",
  title: "Variables: giving values a name",
  screens: [
    [
      "A variable is a name holding a value:",
      { code: "let age = 25;\nconsole.log(age + 1);" },
      '= means "store this", not "is equal to".',
    ],
    [
      "A let variable can get a new value. Write let only the first time:",
      { code: "let score = 10;\nscore = score + 5;\nconsole.log(score);" },
      "Read score = score + 5 right to left: take 10, add 5, store 15.",
    ],
    [
      "const can never get a new value:",
      { code: 'const name = "Ana";\nname = "Bea";', error: true },
      "Use const by default, let only when the value must change.",
      { quiz: "Which one can change later?", choices: ["const", "let", "both"], answer: 2, why: "Only let." },
    ],
    [
      "Capitals matter. total and Total are different names:",
      { code: "let total = 3;\nconsole.log(Total);", error: true },
      "Most beginner errors are a name spelled two ways.",
    ],
    [
      {
        exercise: "variables",
        task: "Make price = 20 and quantity = 3. Print price times quantity. Change price to 25 and print it again.",
        expected: ["60", "75"],
        hints: ["price changes, so use let for it.", "price = 25; then print again."],
        solution: "let price = 20;\nconst quantity = 3;\nconsole.log(price * quantity);\nprice = 25;\nconsole.log(price * quantity);",
      },
    ],
  ],
  recap: [
    "let age = 25; makes a variable. = stores.",
    "let can change; const cannot.",
    "Names are case sensitive.",
  ],
};
