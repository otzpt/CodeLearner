module.exports = {
  id: "first-program",
  title: "Your first program",
  screens: [
    [
      "A program is a text file of instructions. Node runs them one by one, top to bottom:",
      { code: "node hello.js", run: false },
      "Every example in this course really runs. \"Output\" is what Node printed.",
    ],
    [
      "console.log prints one line:",
      { code: 'console.log("Hello!");\nconsole.log(42);\nconsole.log(2 + 3);' },
      "Text goes in quotes. Numbers do not, and sums are worked out.",
    ],
    [
      "In quotes, it is only text:",
      { code: 'console.log("2 + 3");' },
      "A line starting with // is a comment. Node skips it.",
      { quiz: 'What does console.log("10 + 5"); print?', choices: ["15", "10 + 5", "an error"], answer: 2, why: "In quotes, it is text, printed as written." },
    ],
    [
      "Capitals matter. console.Log is not console.log:",
      { code: 'console.Log("hi");', error: true },
      "An error stops the program and names the line. Reading errors is a normal part of programming.",
    ],
    [
      {
        exercise: "hello",
        task: "Print the name Ana on one line and the number 2026 on the next.",
        expected: ["Ana", "2026"],
        hints: ["Two console.log lines.", 'Text in quotes: console.log("Ana");', "Numbers without: console.log(2026);"],
        solution: 'console.log("Ana");\nconsole.log(2026);',
      },
    ],
  ],
  recap: [
    "node file.js runs a file, top to bottom.",
    "console.log(...) prints one line.",
    "Text in quotes; numbers without. \"2 + 3\" is just text.",
    "// starts a comment.",
    "An error names what went wrong and the line.",
  ],
};
