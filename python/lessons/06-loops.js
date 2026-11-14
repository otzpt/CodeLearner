module.exports = {
  id: "loops",
  title: "Repeating with loops",
  titlePt: "Repetir com ciclos",
  screens: [
    [
      "A loop repeats a block of code. for with range repeats it a set number of times, with a counter that changes each lap:",
      { code: 'for i in range(1, 4):\n    print("lap", i)' },
      "range(1, 4) counts 1, 2, 3. It stops just before the end number, the same as a slice. That is the most common loop surprise.",
    ],
    [
      "range with one number starts at 0. A third number is the step:",
      { code: "for i in range(3):\n    print(i)\nfor i in range(10, 0, -3):\n    print(i)" },
      { quiz: "How many times does for i in range(1, 5) run?", choices: ["4", "5", "6"], answer: 1, why: "i is 1, 2, 3, 4. range stops before 5." },
    ],
    [
      "A loop often builds up a result. Start a variable before the loop and change it inside:",
      { code: "total = 0\nfor i in range(1, 5):\n    total += i\nprint(total)" },
      "total goes 0, 1, 3, 6, 10. The print is not indented under the for, so it runs once, at the end.",
    ],
    [
      "while repeats as long as a condition is true. Use it when you do not know the count in advance. break leaves a loop at once:",
      { code: "money = 10\nwhile money > 0:\n    print(f\"I have {money}\")\n    money -= 4" },
      "If the condition never becomes false, the loop never stops. If your program seems frozen in a terminal, Ctrl+C stops it.",
      { tip: "range(a, b) stops before b. Something in a while loop must change the condition." },
    ],
    [
      {
        exercise: "count",
        task: "Print the numbers 1 to 5, one per line. Then print the sum of the numbers 1 to 100.",
        expected: ["1", "2", "3", "4", "5", "5050"],
        hints: ["range(1, 6) gives 1 to 5.", "For the sum: total = 0 before a second loop over range(1, 101), total += i inside, print(total) after."],
        solution: "for i in range(1, 6):\n    print(i)\ntotal = 0\nfor i in range(1, 101):\n    total += i\nprint(total)",
      },
    ],
  ],
  recapPt: [
    "for i in range(1, 4): repete com i a valer 1, 2, 3.",
    "range para antes do último número; range(3) é 0, 1, 2.",
    "Constrói um resultado com uma variável criada antes do ciclo.",
    "while condição: repete até a condição ser falsa. break sai do ciclo. Ctrl+C para um programa preso.",
  ],
  recap: [
    "for i in range(1, 4): repeats with i as 1, 2, 3.",
    "range stops before its end number; range(3) is 0, 1, 2.",
    "Build a result with a variable made before the loop.",
    "while condition: repeats until it is false. break leaves the loop. Ctrl+C stops a stuck program.",
  ],
};
