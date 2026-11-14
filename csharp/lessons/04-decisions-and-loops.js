module.exports = {
  id: "decisions-and-loops",
  title: "Decisions and loops",
  titlePt: "Decisões e ciclos",
  screens: [
    [
      "if runs a block only when its condition is true; else if checks another; else runs when none was. == compares, != means \"not equal\", && is \"and\", || is \"or\":",
      { code: 'int score = 72;\nif (score >= 90)\n{\n    Console.WriteLine("A");\n}\nelse if (score >= 50)\n{\n    Console.WriteLine("pass");\n}\nelse\n{\n    Console.WriteLine("fail");\n}' },
      "C# checks from the top and runs only the first block whose condition is true.",
    ],
    [
      "A condition must be true or false, the type called bool. So = instead of == inside if is refused, not a silent bug:",
      { code: 'int x = 3;\nif (x = 0)\n{\n    Console.WriteLine("x is 0");\n}', error: true },
    ],
    [
      "A for loop repeats a block a set number of times: start, the condition checked before each lap, and the step after each lap:",
      { code: "int total = 0;\nfor (int i = 1; i <= 4; i++)\n{\n    total += i;\n}\nConsole.WriteLine(total);" },
      "Mixing up < and <= runs one lap too many or too few. while (condition) repeats as long as it is true, and break leaves any loop.",
      { quiz: "How many times does for (int i = 0; i < 3; i++) run?", choices: ["2", "3", "4"], answer: 2, why: "i is 0, 1, 2." },
    ],
    [
      {
        exercise: "count",
        task: 'Print the numbers 1 to 5, one per line. Then print the sum of the numbers 1 to 100. Then print "even" or "odd" for 2026.',
        expected: ["1", "2", "3", "4", "5", "5050", "even"],
        hints: ["for (int i = 1; i <= 5; i++)", "2026 % 2 == 0 means even."],
        solution: 'for (int i = 1; i <= 5; i++)\n{\n    Console.WriteLine(i);\n}\nint total = 0;\nfor (int i = 1; i <= 100; i++)\n{\n    total += i;\n}\nConsole.WriteLine(total);\nif (2026 % 2 == 0)\n{\n    Console.WriteLine("even");\n}\nelse\n{\n    Console.WriteLine("odd");\n}',
      },
    ],
  ],
  recapPt: [
    "if / else if / else corre só o primeiro bloco verdadeiro.",
    "Uma condição tem de ser bool; = num if é recusado.",
    "for (int i = 1; i <= 3; i++) repete um número fixo de vezes.",
    "while repete enquanto a condição for verdadeira; break sai do ciclo.",
  ],
  recap: [
    "if / else if / else runs only the first true block.",
    "A condition must be a bool; = inside if is refused.",
    "for (int i = 1; i <= 3; i++) repeats a set number of times.",
    "while repeats while the condition is true; break leaves the loop.",
  ],
};
