module.exports = {
  id: "numbers-and-variables",
  title: "Numbers and variables",
  titlePt: "Números e variáveis",
  screens: [
    [
      "A variable is a named box for a value. Every box has a fixed type, written before its name. int holds whole numbers, double holds numbers with decimals:",
      { code: "int age = 25;\ndouble price = 4.5;\nConsole.WriteLine(age + 1);\nConsole.WriteLine(price * 3);" },
      '= means "store this", not "is equal to". A variable can get a new value later, but never a different type.',
    ],
    [
      "Here is the surprise. Dividing one int by another gives an int: the decimals are thrown away, not rounded. % gives the remainder:",
      { code: "Console.WriteLine(7 / 2);\nConsole.WriteLine(7 % 2);\nConsole.WriteLine(7.0 / 2);" },
      "Writing 7.0 makes it a double, so the division keeps the decimals.",
      { quiz: "What is 9 / 4 when both are int?", choices: ["2.25", "2", "3"], answer: 2, why: "int division drops the decimals." },
    ],
    [
      "A value with decimals does not fit in an int box. C# refuses rather than silently cutting the decimals off:",
      { code: "int half = 0.5;\nConsole.WriteLine(half);", error: true },
      "const makes a value that can never change: const int Days = 7; x += 5 is short for x = x + 5, and x++ adds 1. var lets C# work out the type from the value: var count = 3; is an int.",
      { tip: "int for whole numbers, double for decimals. int / int drops the decimals." },
    ],
    [
      {
        exercise: "variables",
        task: "Create int price = 20 and int quantity = 3. Print price times quantity. Then print 17 divided by 5 as an int, and 17.0 divided by 5.",
        expected: ["60", "3", "3.4"],
        hints: ["Console.WriteLine(price * quantity);", "17 / 5 drops the decimals; 17.0 / 5 keeps them."],
        solution: "int price = 20;\nint quantity = 3;\nConsole.WriteLine(price * quantity);\nConsole.WriteLine(17 / 5);\nConsole.WriteLine(17.0 / 5);",
      },
    ],
  ],
  recapPt: [
    "Cada variável tem um tipo fixo: int age = 25; double price = 4.5;",
    "int / int deita fora as casas decimais; % é o resto.",
    "Um decimal não cabe num int: o C# recusa.",
    "const não muda; var deixa o C# escolher o tipo pelo valor.",
  ],
  recap: [
    "Every variable has a fixed type: int age = 25; double price = 4.5;",
    "int / int throws away the decimals; % is the remainder.",
    "A decimal does not fit in an int: C# refuses.",
    "const never changes; var lets C# pick the type from the value.",
  ],
};
