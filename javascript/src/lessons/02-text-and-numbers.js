module.exports = {
  id: "text-and-numbers",
  title: "Text and numbers",
  screens: [
    [
      "The usual maths works. Brackets set the order:",
      { code: "console.log(7 * 3);\nconsole.log(7 / 2);\nconsole.log((2 + 3) * 4);" },
    ],
    [
      "% is the remainder of a division. 0 means it divides exactly, so 8 is even:",
      { code: "console.log(7 % 2);\nconsole.log(8 % 2);" },
      "Decimals are not always exact. For money, count whole cents:",
      { code: "console.log(0.1 + 0.2);" },
    ],
    [
      "Text is called a string. + joins strings exactly as they are:",
      { code: 'console.log("good" + "morning");\nconsole.log("good " + "morning");' },
    ],
    [
      "The surprise: + with a string and a number joins instead of adding:",
      { code: 'console.log(5 + 1);\nconsole.log("5" + 1);\nconsole.log(Number("5") + 1);' },
      'Typed input arrives as strings, so this catches everyone once. Number(...) turns text into a number.',
      { quiz: 'What does console.log("3" + 4); print?', choices: ["7", "34", "an error"], answer: 2, why: '"3" is a string, so + joins.' },
    ],
    [
      "typeof shows which kind a value is:",
      { code: 'console.log(typeof 42);\nconsole.log(typeof "42");' },
      {
        exercise: "maths",
        task: 'Print 12 times 5, then the remainder of 17 divided by 5, then "12" + 5.',
        expected: ["60", "2", "125"],
        hints: ["Three console.log lines, using *, % and +."],
        solution: 'console.log(12 * 5);\nconsole.log(17 % 5);\nconsole.log("12" + 5);',
      },
    ],
  ],
  recap: [
    "+ - * / and % (remainder). Brackets set the order.",
    "Decimals are not exact; use whole cents for money.",
    '"5" + 1 is "51". Number("5") turns text into a number.',
    "typeof tells number from string.",
  ],
};
