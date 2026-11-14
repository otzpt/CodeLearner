module.exports = {
  id: "first-program",
  title: "Your first program",
  screens: [
    [
      "A program is a list of instructions for the computer, written in a text file. The computer does them one at a time, from the top line down.",
      "JavaScript is one language for writing those instructions. Node is the program that reads a JavaScript file and carries out what it says. You run a file by typing this in a terminal:",
      { code: "// node hello.js   <- typed in the terminal, not in the file", run: false },
      "Every example in this course really runs while you read it. What you see under \"Output\" is what Node printed just now.",
    ],
    [
      "The first instruction to learn is console.log. It prints whatever you put between its brackets:",
      { code: 'console.log("Hello!");' },
      "Text has to be inside quotes. The quotes tell JavaScript where the text starts and ends; they are not printed.",
      "Each console.log prints one line. Two of them print two lines, in order:",
      { code: 'console.log("first line");\nconsole.log("second line");' },
    ],
    [
      "Numbers do not need quotes. JavaScript can also work out a sum before printing it:",
      { code: "console.log(42);\nconsole.log(2 + 3);\nconsole.log(\"2 + 3\");" },
      "The third line prints the text 2 + 3, not 5. In quotes, it is just text to print.",
      "A line that starts with // is a comment. Node skips it. Comments are notes for people reading the code:",
      { code: '// this line is ignored\nconsole.log("this one runs");' },
      { quiz: 'What does console.log("10 + 5"); print?', choices: ["15", "10 + 5", "an error"], answer: 2, why: "It is in quotes, so it is text, printed exactly as written." },
    ],
    [
      "Spelling matters, including capital letters. console.Log, with a capital L, is not the same as console.log, and Node cannot run it:",
      { code: 'console.Log("hi");', error: true },
      "When something goes wrong, Node stops and prints an error. The course explains each error in plain words, like above. Reading errors is a normal part of programming, not a sign you did something terrible.",
      { tip: "One instruction per line, text in quotes, names spelled exactly." },
    ],
    [
      {
        exercise: "hello",
        task: "Write a program that prints your name on one line and the number 2026 on the next. For the checker, use the name Ana.",
        expected: ["Ana", "2026"],
        hints: ["You need two console.log lines.", 'Text goes in quotes: console.log("Ana");', "The number does not need quotes: console.log(2026);"],
        solution: 'console.log("Ana");\nconsole.log(2026);',
      },
    ],
  ],
  recap: [
    "A program is instructions in a file, done top to bottom. Node runs it: node file.js",
    "console.log(...) prints one line.",
    "Text goes in quotes. Numbers do not. 2 + 3 is worked out; \"2 + 3\" is just text.",
    "// starts a comment that Node skips.",
    "An error stops the program and says what went wrong and on which line.",
  ],
};
