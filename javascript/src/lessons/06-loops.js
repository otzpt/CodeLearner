module.exports = {
  id: "loops",
  title: "Repeating with loops",
  screens: [
    [
      "for repeats a block a set number of times:",
      { code: "for (let i = 1; i <= 3; i++) {\n  console.log(i);\n}" },
      "let i = 1 runs once at the start. i <= 3 is checked before each lap. i++ adds 1 after each lap.",
    ],
    [
      "< or <= decides whether the last number is included. Mixing them up is the most common loop bug:",
      { code: "for (let i = 0; i < 3; i++) {\n  console.log(i);\n}" },
      { quiz: "How many times does for (let i = 1; i < 5; i++) run?", choices: ["4", "5", "6"], answer: 1, why: "1, 2, 3, 4. At 5, i < 5 is false." },
    ],
    [
      "To build a result, start a variable before the loop:",
      { code: "let total = 0;\nfor (let i = 1; i <= 4; i++) {\n  total = total + i;\n}\nconsole.log(total);" },
    ],
    [
      "while repeats as long as its condition is true:",
      { code: "let money = 10;\nwhile (money > 0) {\n  console.log(money);\n  money = money - 4;\n}" },
      "If the condition never turns false, it never stops. Ctrl+C stops a stuck program.",
    ],
    [
      {
        exercise: "count",
        task: "Print 1 to 5, one per line. Then print the sum of 1 to 100.",
        expected: ["1", "2", "3", "4", "5", "5050"],
        hints: ["for loop with i <= 5.", "let total = 0 before a second loop; print it after."],
        solution: "for (let i = 1; i <= 5; i++) {\n  console.log(i);\n}\nlet total = 0;\nfor (let i = 1; i <= 100; i++) {\n  total = total + i;\n}\nconsole.log(total);",
      },
    ],
  ],
  recap: [
    "for (let i = 1; i <= 3; i++) repeats a set number of times.",
    "< or <= decides whether the last number counts.",
    "while (condition) repeats until it is false.",
  ],
};
