module.exports = {
  id: "text-and-numbers",
  title: "Text and numbers",
  titlePt: "Texto e números",
  screens: [
    [
      "Programs work with values. The two you will use most are numbers and text. Text is called a string (a string of characters).",
      "The usual maths signs work: + add, - subtract, * multiply, / divide. ** is a power: 2 ** 3 is 2 times 2 times 2. Brackets change the order, the same as in maths class:",
      { code: "print(7 * 3)\nprint(2 ** 3)\nprint(2 + 3 * 4)\nprint((2 + 3) * 4)" },
    ],
    [
      "/ always gives a decimal number, even when it divides exactly. // divides and drops the decimals. % gives the remainder, which is how programs check whether a number is even:",
      { code: "print(7 / 2)\nprint(8 / 2)\nprint(7 // 2)\nprint(7 % 2)" },
      "Whole numbers are called int, decimal numbers are called float. Decimals are not always exact inside a computer: 0.1 + 0.2 gives 0.30000000000000004. For money, work in whole cents.",
    ],
    [
      "Strings can use single or double quotes, as long as both ends match. + joins strings, and * repeats one:",
      { code: 'print("good" + "morning")\nprint(\'good \' + "morning")\nprint("ha" * 3)' },
      "The first has no space because neither string had one. + joins exactly what is there.",
    ],
    [
      "Text and numbers do not mix with +. Python refuses rather than guess what you meant:",
      { code: 'print("5" + 1)', error: true },
      'int(...) turns a string into a whole number, float(...) into a decimal, and str(...) turns a number into text. So int("5") + 1 is 6, and "5" + str(1) is "51".',
      { quiz: 'What does print(int("3") + 4) show?', choices: ["7", "34", "an error"], answer: 1, why: 'int("3") is the number 3, so + adds.' },
    ],
    [
      'type(...) tells you what kind of value something is: type(42) is int, type(4.2) is float and type("42") is str.',
      { tip: "When a sum fails or looks wrong, check whether one of the values is a string." },
      {
        exercise: "maths",
        task: "Print 12 times 5, then 17 divided by 5 with the decimals dropped, then the remainder of 17 divided by 5.",
        expected: ["60", "3", "2"],
        hints: ["Use *, // and % inside three print lines."],
        solution: "print(12 * 5)\nprint(17 // 5)\nprint(17 % 5)",
      },
    ],
  ],
  recapPt: [
    "Os números usam + - * / ** // e %. Os parênteses definem a ordem.",
    "/ dá sempre um decimal; // corta as casas decimais; % é o resto.",
    "Strings são texto entre aspas. + junta strings, * repete.",
    "Texto + número é um erro. int(), float() e str() convertem.",
    "type() mostra o tipo de um valor: int, float ou str.",
  ],
  recap: [
    "Numbers use + - * / ** // and %. Brackets set the order.",
    "/ always gives a decimal; // drops the decimals; % is the remainder.",
    "Strings are text in quotes. + joins strings, * repeats one.",
    "Text + a number is an error. int(), float() and str() convert.",
    "type() shows a value's type: int, float or str.",
  ],
};
