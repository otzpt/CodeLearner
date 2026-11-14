const HELPER = require("./10-input.js").HELPER;

module.exports = {
  id: "guessing-game",
  title: "Project: a guessing game",
  screens: [
    [
      "Time to put everything together in one real program: a number guessing game.",
      "The program thinks of a secret number. The player guesses; the program says \"Too low\" or \"Too high\" until the guess is right, then says how many guesses it took.",
      "It uses almost every lesson: variables (3), if (5), a while loop (6), a function (8), ask and Number (10).",
    ],
    [
      "Plan it in plain words before writing code. Programmers do this all the time:",
      "1. Keep the secret number and a guess counter.\n2. Repeat until the guess is right:\n   - ask for a guess and turn it into a number\n   - add 1 to the counter\n   - say Too low or Too high\n3. Say Correct and the number of guesses.",
      "A while loop fits step 2, because you cannot know in advance how many guesses it will take. A common trick is while (true), a loop that only ends when the code inside says break:",
      { code: "let n = 0;\nwhile (true) {\n  n = n + 1;\n  if (n === 3) {\n    break;\n  }\n}\nconsole.log(n);" },
    ],
    [
      {
        exercise: "game",
        task: "Write the game with the secret number 42, so the checker knows the answer. Ask with \"Guess: \". Print Too low, Too high, or at the end Correct! It took N guesses.",
        input: ["50", "25", "42"],
        expected: ["Guess: 50", "Too high", "Guess: 25", "Too low", "Guess: 42", "Correct! It took 3 guesses."],
        starter: `${HELPER}\n\nasync function main() {\n  const secret = 42;\n  // your code here\n\n  rl.close();\n}\n\nmain();`,
        hints: [
          "let guesses = 0; then while (true) { ... }",
          "Inside the loop: const guess = Number(await ask(\"Guess: \")); guesses = guesses + 1;",
          "if (guess < secret) Too low, else if (guess > secret) Too high, else print Correct and break.",
        ],
        solution: `${HELPER}\n\nasync function main() {\n  const secret = 42;\n  let guesses = 0;\n  while (true) {\n    const guess = Number(await ask("Guess: "));\n    guesses = guesses + 1;\n    if (guess < secret) {\n      console.log("Too low");\n    } else if (guess > secret) {\n      console.log("Too high");\n    } else {\n      console.log(\`Correct! It took \${guesses} guesses.\`);\n      break;\n    }\n  }\n  rl.close();\n}\n\nmain();`,
      },
    ],
    [
      "Once it passes, make it a real game: replace 42 with a random number from 1 to 100.",
      { code: "const secret = Math.floor(Math.random() * 100) + 1;\nconsole.log(secret >= 1 && secret <= 100);" },
      "Math.random() gives a decimal from 0 up to (not including) 1. Times 100 makes it 0 to 99.99..., Math.floor cuts off the decimals, and + 1 makes it 1 to 100.",
      "Run it with node and play it yourself. The checker cannot test this version, because it would not know the secret.",
    ],
  ],
  recap: [
    "Plan in plain words first, then turn each step into code.",
    "while (true) with break repeats until the code decides to stop.",
    "Math.floor(Math.random() * 100) + 1 is a random whole number from 1 to 100.",
    "You wrote a complete interactive program. Everything after this builds on the same pieces.",
  ],
};
