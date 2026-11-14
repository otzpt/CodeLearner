const HELPER = require("./11-input.js").HELPER;

module.exports = {
  id: "guessing-game",
  title: "Project: a guessing game",
  screens: [
    [
      "A game: the player guesses a secret number and hears \"Too low\" or \"Too high\" until right.",
      "Plan it first:\n1. Keep the secret and a guess counter.\n2. Repeat: ask, count, say low or high.\n3. When right, say how many guesses.",
    ],
    [
      "You cannot know how many guesses it takes, so use while (true) and stop with break:",
      { code: "let n = 0;\nwhile (true) {\n  n = n + 1;\n  if (n === 3) break;\n}\nconsole.log(n);" },
    ],
    [
      {
        exercise: "game",
        task: 'Use the secret 42 so the checker knows it. Ask "Guess: ". Print Too low or Too high, and at the end Correct! It took N guesses.',
        input: ["50", "25", "42"],
        expected: ["Guess: 50", "Too high", "Guess: 25", "Too low", "Guess: 42", "Correct! It took 3 guesses."],
        starter: `${HELPER}\n\nasync function main() {\n  const secret = 42;\n  // your code here\n\n  rl.close();\n}\n\nmain();`,
        hints: [
          "let guesses = 0; while (true) { ... }",
          'const guess = Number(await ask("Guess: ")); guesses = guesses + 1;',
          "if (guess < secret) Too low, else if (guess > secret) Too high, else Correct and break.",
        ],
        solution: `${HELPER}\n\nasync function main() {\n  const secret = 42;\n  let guesses = 0;\n  while (true) {\n    const guess = Number(await ask("Guess: "));\n    guesses = guesses + 1;\n    if (guess < secret) {\n      console.log("Too low");\n    } else if (guess > secret) {\n      console.log("Too high");\n    } else {\n      console.log(\`Correct! It took \${guesses} guesses.\`);\n      break;\n    }\n  }\n  rl.close();\n}\n\nmain();`,
      },
    ],
    [
      "Once it passes, swap 42 for a random number from 1 to 100 and play it with node:",
      { code: "const secret = Math.floor(Math.random() * 100) + 1;\nconsole.log(secret >= 1 && secret <= 100);" },
      "Math.random() is 0 up to 1. Times 100, cut the decimals with Math.floor, add 1.",
    ],
  ],
  recap: [
    "Plan in plain words, then code each step.",
    "while (true) with break repeats until the code says stop.",
    "Math.floor(Math.random() * 100) + 1 gives 1 to 100.",
  ],
};
