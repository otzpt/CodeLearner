module.exports = {
  id: "numbers-and-variables",
  title: "Numbers and variables",
  titlePt: "Números e variáveis",
  screens: [
    [
      "A variable is a named box for a value. In C, every box has a fixed type, written before its name. int holds whole numbers:",
      { code: '#include <stdio.h>\n\nint main(void)\n{\n    int age = 25;\n    printf("%d\\n", age);\n    printf("Next year: %d\\n", age + 1);\n    return 0;\n}' },
      "%d in printf's text is a slot for a whole number. The values after the text fill the slots, in order.",
    ],
    [
      "double holds numbers with decimals, printed with %f. %.2f prints two decimal places:",
      { code: '#include <stdio.h>\n\nint main(void)\n{\n    double price = 4.5;\n    printf("%f\\n", price);\n    printf("%.2f\\n", price * 3);\n    return 0;\n}' },
      "Decimals are not always exact inside a computer. For money, count whole cents in an int.",
    ],
    [
      "Here is the surprise. Dividing one int by another gives an int: the decimals are thrown away, not rounded. % gives the remainder:",
      { code: '#include <stdio.h>\n\nint main(void)\n{\n    printf("%d\\n", 7 / 2);\n    printf("%d\\n", 7 % 2);\n    printf("%.1f\\n", 7.0 / 2);\n    return 0;\n}' },
      "Writing 7.0 makes it a double, so the division keeps the decimals.",
      { quiz: "What is 9 / 4 when both are int?", choices: ["2.25", "2", "3"], answer: 2, why: "int division drops the decimals: 2.25 becomes 2." },
    ],
    [
      "A variable can get a new value with =, which means \"store\", not \"is equal to\". The type stays the same. The compiler warns about many slips, such as the wrong slot in printf:",
      { code: '#include <stdio.h>\n\nint main(void)\n{\n    double half = 0.5;\n    printf("%d\\n", half);\n    return 0;\n}', warning: true },
      "%d expected an int but got a double, so what it prints is garbage. Read every warning: most of them are real bugs.",
      { tip: "int for whole numbers (%d), double for decimals (%f). int / int drops the decimals." },
    ],
    [
      {
        exercise: "variables",
        task: "Create an int price = 20 and an int quantity = 3. Print price times quantity. Then print 17 divided by 5 as an int, and as a double with one decimal.",
        expected: ["60", "3", "3.4"],
        hints: ['printf("%d\\n", price * quantity);', 'For the decimal one, write 17.0 / 5 and print it with %.1f.'],
        solution: '#include <stdio.h>\n\nint main(void)\n{\n    int price = 20;\n    int quantity = 3;\n    printf("%d\\n", price * quantity);\n    printf("%d\\n", 17 / 5);\n    printf("%.1f\\n", 17.0 / 5);\n    return 0;\n}',
      },
    ],
  ],
  recapPt: [
    "Cada variável tem um tipo fixo: int age = 25; double price = 4.5;",
    "printf usa %d para int e %f (ou %.2f) para double.",
    "int / int deita fora as casas decimais; % é o resto.",
    "Lê todos os avisos do compilador: a maioria são erros a sério.",
  ],
  recap: [
    "Every variable has a fixed type: int age = 25; double price = 4.5;",
    "printf uses %d for int and %f (or %.2f) for double.",
    "int / int throws away the decimals; % is the remainder.",
    "Read every compiler warning: most are real bugs.",
  ],
};
