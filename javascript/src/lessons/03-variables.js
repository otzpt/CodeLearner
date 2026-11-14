module.exports = {
  id: "variables",
  title: "Variables: giving values a name",
  screens: [
    [
      "A variable is a name that holds a value, so you can use it later. Think of a labelled box.",
      "You create one with let, a name, = and the value:",
      { code: 'let age = 25;\nconsole.log(age);\nconsole.log(age + 1);' },
      "= here does not mean \"equals\" like in maths. It means \"put this value in the box\". The name age stands for 25 from then on.",
    ],
    [
      "A variable made with let can get a new value later. Then the old one is gone:",
      { code: "let score = 10;\nconsole.log(score);\nscore = 15;\nconsole.log(score);\nscore = score + 5;\nconsole.log(score);" },
      "score = score + 5 reads strange at first. Read it right to left: take score (15), add 5, put the result (20) back into score.",
      "You write let only once, when the variable is created. After that, just the name.",
    ],
    [
      "const creates a variable that can never get a new value. Use it for things that should not change:",
      { code: 'const name = "Ana";\nconsole.log(name);\nname = "Bea";', error: true },
      "A good habit: use const by default, and let only when you know the value has to change.",
      { quiz: "Which one lets you change the value later?", choices: ["const", "let", "both"], answer: 2, why: "let can be changed. const stays the same forever." },
    ],
    [
      "Names can use letters, digits, _ and $, but cannot start with a digit or contain spaces. When a name has several words, the custom is to start each new word with a capital letter: totalPrice, firstName.",
      "Capitals matter. total and Total are two different variables. Using a name that was never created is an error:",
      { code: "let total = 3;\nconsole.log(Total);", error: true },
      { tip: "Most beginner errors are a name spelled two different ways. Read the name in the error message letter by letter." },
    ],
    [
      {
        exercise: "variables",
        task: "Create a variable price with the value 20 and a variable quantity with the value 3. Print price times quantity. Then change price to 25 and print price times quantity again.",
        expected: ["60", "75"],
        hints: ["price has to change, so make it with let.", "After the first console.log, write price = 25; and print again."],
        solution: "let price = 20;\nconst quantity = 3;\nconsole.log(price * quantity);\nprice = 25;\nconsole.log(price * quantity);",
      },
    ],
  ],
  recap: [
    "A variable is a named box for a value: let age = 25;",
    "= means \"store this\", not \"is equal to\".",
    "let can be given a new value; const cannot. Use const unless the value must change.",
    "Names are case sensitive. A name that was never created is a ReferenceError.",
  ],
};
