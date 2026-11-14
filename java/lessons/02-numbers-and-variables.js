module.exports = {
  id: "numbers-and-variables",
  title: "Numbers and variables",
  titlePt: "Números e variáveis",
  screens: [
    [
      "A variable is a named box for a value. In Java, every box has a fixed type, written before its name. int holds whole numbers, double holds numbers with decimals:",
      { code: 'public class Main {\n    public static void main(String[] args) {\n        int age = 25;\n        double price = 4.5;\n        System.out.println(age + 1);\n        System.out.println(price * 3);\n    }\n}' },
      '= means "store this", not "is equal to". A variable can get a new value later, but never a different type.',
    ],
    [
      "Here is the surprise. Dividing one int by another gives an int: the decimals are thrown away, not rounded. % gives the remainder:",
      { code: 'public class Main {\n    public static void main(String[] args) {\n        System.out.println(7 / 2);\n        System.out.println(7 % 2);\n        System.out.println(7.0 / 2);\n    }\n}' },
      "Writing 7.0 makes it a double, so the division keeps the decimals.",
      { quiz: "What is 9 / 4 when both are int?", choices: ["2.25", "2", "3"], answer: 2, why: "int division drops the decimals." },
    ],
    [
      "A value with decimals does not fit in an int box. Java refuses rather than silently cutting the decimals off:",
      { code: 'public class Main {\n    public static void main(String[] args) {\n        int half = 0.5;\n        System.out.println(half);\n    }\n}', error: true },
      "final makes a variable that can never get a new value: final int DAYS = 7; x += 5 is short for x = x + 5, and x++ adds 1.",
      { tip: "int for whole numbers, double for decimals. int / int drops the decimals." },
    ],
    [
      {
        exercise: "variables",
        task: "Create int price = 20 and int quantity = 3. Print price times quantity. Then print 17 divided by 5 as an int, and 17.0 divided by 5.",
        expected: ["60", "3", "3.4"],
        hints: ["System.out.println(price * quantity);", "17 / 5 drops the decimals; 17.0 / 5 keeps them."],
        solution: 'public class Main {\n    public static void main(String[] args) {\n        int price = 20;\n        int quantity = 3;\n        System.out.println(price * quantity);\n        System.out.println(17 / 5);\n        System.out.println(17.0 / 5);\n    }\n}',
      },
    ],
  ],
  recapPt: [
    "Cada variável tem um tipo fixo: int age = 25; double price = 4.5;",
    "int / int deita fora as casas decimais; % é o resto.",
    "Um decimal não cabe num int: o Java recusa.",
    "final não deixa mudar o valor; x += 5 é x = x + 5.",
  ],
  recap: [
    "Every variable has a fixed type: int age = 25; double price = 4.5;",
    "int / int throws away the decimals; % is the remainder.",
    "A decimal does not fit in an int: Java refuses.",
    "final stops the value changing; x += 5 is x = x + 5.",
  ],
};
