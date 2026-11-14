module.exports = {
  id: "decisions-and-loops",
  title: "Decisions and loops",
  titlePt: "Decisões e ciclos",
  screens: [
    [
      "if runs a block only when its condition is true. else if checks another condition; else runs when none was true. == compares, != means \"not equal\", && is \"and\", || is \"or\":",
      { code: '#include <iostream>\n\nint main()\n{\n    int score = 72;\n    if (score >= 90) {\n        std::cout << "A\\n";\n    } else if (score >= 50 && score != 51) {\n        std::cout << "pass\\n";\n    } else {\n        std::cout << "fail\\n";\n    }\n    return 0;\n}' },
      "C++ checks from the top and runs only the first block whose condition is true.",
    ],
    [
      "A common slip: = instead of == inside if. It stores the value and then tests it. The compiler warns:",
      { code: '#include <iostream>\n\nint main()\n{\n    int x = 3;\n    if (x = 0) {\n        std::cout << "x is 0\\n";\n    }\n    std::cout << "x is now " << x << "\\n";\n    return 0;\n}', warning: true },
      "x = 0 stored 0 in x, and 0 counts as false, so the block was skipped. Nothing was compared.",
    ],
    [
      "A for loop repeats a block a set number of times. Its three parts: start, the condition checked before each lap, and the step after each lap:",
      { code: '#include <iostream>\n\nint main()\n{\n    int total = 0;\n    for (int i = 1; i <= 4; i++) {\n        total += i;\n    }\n    std::cout << total << "\\n";\n    return 0;\n}' },
      "Mixing up < and <= runs one lap too many or too few. while (condition) repeats as long as it is true, and break leaves any loop at once.",
      { quiz: "How many times does for (int i = 0; i < 3; i++) run?", choices: ["2", "3", "4"], answer: 2, why: "i is 0, 1, 2." },
    ],
    [
      {
        exercise: "count",
        task: 'Print the numbers 1 to 5, one per line. Then print the sum of the numbers 1 to 100. Then print "even" or "odd" for 2026.',
        expected: ["1", "2", "3", "4", "5", "5050", "even"],
        hints: ["for (int i = 1; i <= 5; i++)", "2026 % 2 == 0 means even."],
        solution: '#include <iostream>\n\nint main()\n{\n    for (int i = 1; i <= 5; i++) {\n        std::cout << i << "\\n";\n    }\n    int total = 0;\n    for (int i = 1; i <= 100; i++) {\n        total += i;\n    }\n    std::cout << total << "\\n";\n    if (2026 % 2 == 0) {\n        std::cout << "even\\n";\n    } else {\n        std::cout << "odd\\n";\n    }\n    return 0;\n}',
      },
    ],
  ],
  recapPt: [
    "if / else if / else corre só o primeiro bloco verdadeiro.",
    "== compara, = guarda: = dentro de um if é um erro comum.",
    "for (int i = 1; i <= 3; i++) repete um número fixo de vezes; < ou <= decide se o último entra.",
    "while repete enquanto a condição for verdadeira; break sai do ciclo.",
  ],
  recap: [
    "if / else if / else runs only the first true block.",
    "== compares, = stores: = inside if is a common slip.",
    "for (int i = 1; i <= 3; i++) repeats a set number of times; < or <= decides the last.",
    "while repeats while the condition is true; break leaves the loop.",
  ],
};
