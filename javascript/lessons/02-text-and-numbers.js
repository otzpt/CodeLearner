module.exports = {
  id: "text-and-numbers",
  title: "Text and numbers",
  titlePt: "Texto e números",
  screens: [
    [
      "Programs work with values. The two you will use most are numbers and text. Text is called a string (a string of characters).",
      "Numbers can be whole or have a decimal point. The usual maths signs work: + add, - subtract, * multiply, / divide. Brackets change the order, the same as in maths class:",
      { code: "console.log(7 * 3);\nconsole.log(7 / 2);\nconsole.log(2 + 3 * 4);\nconsole.log((2 + 3) * 4);" },
    ],
    [
      "% gives the remainder of a division. 7 % 2 is 1, because 7 divided by 2 is 3 with 1 left over. A remainder of 0 means it divides exactly, which is how programs check whether a number is even:",
      { code: "console.log(7 % 2);\nconsole.log(8 % 2);" },
      "Decimal numbers are not always exact inside a computer: 0.1 + 0.2 gives 0.30000000000000004. That is not a mistake in your code. The computer stores most decimals as a very close approximation. For money, work in whole cents (1050 instead of 10.50).",
    ],
    [
      "Strings can use single or double quotes, as long as both ends match. + also joins strings together:",
      { code: 'console.log("good" + "morning");\nconsole.log(\'good \' + "morning");' },
      "The first has no space because neither string had one. + joins exactly what is there.",
    ],
    [
      "Here is the surprise. When + meets a string and a number, it does not add. It turns the number into text and joins them. Number(...) turns a string into a number when you need maths:",
      { code: 'console.log(5 + 1);\nconsole.log("5" + 1);\nconsole.log(Number("5") + 1);' },
      'The quotes make "5" a string, so the result is the string "51". Values typed by a user, or read from a file, arrive as strings, so this catches everyone at least once.',
      { quiz: 'What does console.log("3" + 4); print?', choices: ["7", "34", "an error"], answer: 2, why: '"3" is a string, so + joins instead of adding.' },
    ],
    [
      'typeof tells you what kind of value something is: typeof 42 gives "number" and typeof "42" gives "string".',
      { tip: "When a sum looks wrong, check whether one of the values is a string: typeof shows it." },
      {
        exercise: "maths",
        task: "Print the result of 12 times 5, then the remainder of 17 divided by 5, then \"12\" joined with 5 using +.",
        expected: ["60", "2", "125"],
        hints: ["Use *, % and + inside three console.log lines.", 'The last one is console.log("12" + 5);'],
        solution: 'console.log(12 * 5);\nconsole.log(17 % 5);\nconsole.log("12" + 5);',
      },
    ],
  ],
  recapPt: [
    "Os números usam + - * / e % (resto). Os parênteses definem a ordem.",
    "Decimais como 0.1 + 0.2 não são exatos; para dinheiro usa cêntimos inteiros.",
    "Strings são texto entre aspas. + junta strings.",
    "Uma string + um número junta-os: \"5\" + 1 dá \"51\". Number(\"5\") transforma texto em número.",
    "typeof mostra se um valor é um número ou uma string.",
  ],
  recap: [
    "Numbers use + - * / and % (remainder). Brackets set the order.",
    "Decimals like 0.1 + 0.2 are not exact; use whole cents for money.",
    "Strings are text in quotes. + joins strings.",
    'A string + a number joins them: "5" + 1 is "51". Number("5") turns text into a number.',
    "typeof shows whether a value is a number or a string.",
  ],
};
