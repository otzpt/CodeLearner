module.exports = {
  id: "numbers-and-variables",
  title: "Numbers and variables",
  titlePt: "Números e variáveis",
  screens: [
    [
      "A variable is a named box for a value. In C++, every box has a fixed type, written before its name. int holds whole numbers, double holds numbers with decimals:",
      { code: '#include <iostream>\n\nint main()\n{\n    int age = 25;\n    double price = 4.5;\n    std::cout << age + 1 << "\\n";\n    std::cout << price * 3 << "\\n";\n    return 0;\n}' },
      '= means "store this", not "is equal to". A variable can get a new value later, but its type never changes.',
    ],
    [
      "Here is the surprise. Dividing one int by another gives an int: the decimals are thrown away, not rounded. % gives the remainder:",
      { code: '#include <iostream>\n\nint main()\n{\n    std::cout << 7 / 2 << "\\n";\n    std::cout << 7 % 2 << "\\n";\n    std::cout << 7.0 / 2 << "\\n";\n    return 0;\n}' },
      "Writing 7.0 makes it a double, so the division keeps the decimals.",
      { quiz: "What is 9 / 4 when both are int?", choices: ["2.25", "2", "3"], answer: 2, why: "int division drops the decimals: 2.25 becomes 2." },
    ],
    [
      "const makes a variable that can never get a new value. Trying is an error the compiler catches:",
      { code: '#include <iostream>\n\nint main()\n{\n    const int days = 7;\n    days = 8;\n    std::cout << days << "\\n";\n    return 0;\n}', error: true },
      "x += 5 is short for x = x + 5, and x++ adds 1. Decimals are not always exact inside a computer: for money, count whole cents in an int.",
      { tip: "int for whole numbers, double for decimals. int / int drops the decimals." },
    ],
    [
      {
        exercise: "variables",
        task: "Create int price = 20 and int quantity = 3. Print price times quantity. Then print 17 divided by 5 as an int, and 17.0 divided by 5.",
        expected: ["60", "3", "3.4"],
        hints: ['std::cout << price * quantity << "\\n";', "17 / 5 drops the decimals; 17.0 / 5 keeps them."],
        solution: '#include <iostream>\n\nint main()\n{\n    int price = 20;\n    int quantity = 3;\n    std::cout << price * quantity << "\\n";\n    std::cout << 17 / 5 << "\\n";\n    std::cout << 17.0 / 5 << "\\n";\n    return 0;\n}',
      },
    ],
  ],
  recapPt: [
    "Cada variável tem um tipo fixo: int age = 25; double price = 4.5;",
    "int / int deita fora as casas decimais; % é o resto.",
    "const não deixa mudar o valor.",
    "x += 5 é x = x + 5; x++ soma 1.",
  ],
  recap: [
    "Every variable has a fixed type: int age = 25; double price = 4.5;",
    "int / int throws away the decimals; % is the remainder.",
    "const stops the value from changing.",
    "x += 5 is x = x + 5; x++ adds 1.",
  ],
};
