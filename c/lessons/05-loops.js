module.exports = {
  id: "loops",
  title: "Repeating with loops",
  titlePt: "Repetir com ciclos",
  screens: [
    [
      "A for loop repeats a block a set number of times:",
      { code: '#include <stdio.h>\n\nint main(void)\n{\n    for (int i = 1; i <= 3; i++) {\n        printf("lap %d\\n", i);\n    }\n    return 0;\n}' },
      "The three parts in ( ): int i = 1 runs once at the start; i <= 3 is checked before every lap and the loop stops when it is false; i++ adds 1 after every lap.",
    ],
    [
      "Starting at 0 with < is the most common way to write it. Mixing up < and <= is the most common loop mistake: one lap too many or too few:",
      { code: '#include <stdio.h>\n\nint main(void)\n{\n    for (int i = 0; i < 3; i++) {\n        printf("%d\\n", i);\n    }\n    return 0;\n}' },
      { quiz: "How many times does for (int i = 1; i < 5; i++) run?", choices: ["4", "5", "6"], answer: 1, why: "i is 1, 2, 3, 4. At 5, i < 5 is false." },
    ],
    [
      "A loop often builds up a result. Start a variable before the loop and change it inside. x += y is short for x = x + y:",
      { code: '#include <stdio.h>\n\nint main(void)\n{\n    int total = 0;\n    for (int i = 1; i <= 4; i++) {\n        total += i;\n    }\n    printf("%d\\n", total);\n    return 0;\n}' },
    ],
    [
      "while repeats as long as its condition is true. break leaves a loop at once:",
      { code: '#include <stdio.h>\n\nint main(void)\n{\n    int money = 10;\n    while (money > 0) {\n        printf("I have %d\\n", money);\n        money -= 4;\n    }\n    return 0;\n}' },
      "If the condition never becomes false, the loop never stops. Ctrl+C stops a stuck program in a terminal.",
      { tip: "< or <= decides whether the last number counts. Something in a while loop must change its condition." },
    ],
    [
      {
        exercise: "count",
        task: "Print the numbers 1 to 5, one per line. Then print the sum of the numbers 1 to 100.",
        expected: ["1", "2", "3", "4", "5", "5050"],
        hints: ["for (int i = 1; i <= 5; i++)", "For the sum: int total = 0; before a second loop, total += i; inside, print after."],
        solution: '#include <stdio.h>\n\nint main(void)\n{\n    for (int i = 1; i <= 5; i++) {\n        printf("%d\\n", i);\n    }\n    int total = 0;\n    for (int i = 1; i <= 100; i++) {\n        total += i;\n    }\n    printf("%d\\n", total);\n    return 0;\n}',
      },
    ],
  ],
  recapPt: [
    "for (int i = 1; i <= 3; i++) { } repete um número fixo de vezes.",
    "< ou <= decide se o último número entra.",
    "total += i; é o mesmo que total = total + i;",
    "while (condição) { } repete até a condição ser falsa; break sai do ciclo.",
  ],
  recap: [
    "for (int i = 1; i <= 3; i++) { } repeats a set number of times.",
    "< or <= decides whether the last number counts.",
    "total += i; is total = total + i;",
    "while (condition) { } repeats until it is false; break leaves the loop.",
  ],
};
