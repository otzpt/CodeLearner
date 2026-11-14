module.exports = {
  id: "decisions",
  title: "Making decisions with if",
  titlePt: "Tomar decisões com if",
  screens: [
    [
      "Comparing two values gives True or False. These two values are called booleans.",
      { code: "print(5 > 3)\nprint(5 >= 5)\nprint(4 == 4)\nprint(4 != 4)" },
      '== means "is equal to" and != means "is not equal to". Two characters for ==: a single = stores a value, as you saw with variables.',
    ],
    [
      "if runs a block of code only when a condition is true. The line ends with a colon, and the block is the indented lines under it. else runs when it is false, and elif checks another condition in between:",
      { code: 'score = 72\nif score >= 90:\n    print("A")\nelif score >= 50:\n    print("pass")\nelse:\n    print("fail")\nprint("done")' },
      "Python checks from the top and runs only the first block whose condition is true. \"done\" is not indented, so it is not part of the if and always runs.",
    ],
    [
      "The indentation is not decoration: it is how Python knows which lines belong to the if. Four spaces is the custom. A block with nothing indented is an error:",
      { code: 'temperature = 30\nif temperature > 25:\nprint("Hot")', error: true },
      { quiz: "How does Python know which lines belong to an if?", choices: ["the brackets around them", "they are indented under it", "they end with a colon"], answer: 2, why: "The block is the indented lines after the line ending in a colon." },
    ],
    [
      'and means both sides must be true. or means at least one must be. not flips True and False:',
      { code: "age = 20\nhas_ticket = True\nprint(age >= 18 and has_ticket)\nprint(age < 13 or age > 65)\nprint(not has_ticket)" },
      "Numbers and strings can be compared with == too, but a number is never equal to text: 5 == \"5\" is False.",
      { tip: "if line ends with a colon; its block is indented. == compares, = stores." },
    ],
    [
      {
        exercise: "grade",
        task: 'Create points = 64. Print "great" if points is 80 or more, "good" if it is 50 or more, and "try again" otherwise. Then, on a new line, print whether points is an even number (True or False).',
        expected: ["good", "True"],
        hints: ["Use if, elif and else, checking 80 first.", "points % 2 == 0 is True for an even number."],
        solution: 'points = 64\nif points >= 80:\n    print("great")\nelif points >= 50:\n    print("good")\nelse:\n    print("try again")\nprint(points % 2 == 0)',
      },
    ],
  ],
  recapPt: [
    "As comparações dão True ou False: > < >= <= == !=",
    "if / elif / else corre só o primeiro bloco verdadeiro.",
    "A linha do if acaba em dois pontos; o bloco são as linhas indentadas por baixo.",
    "and, or e not combinam condições.",
  ],
  recap: [
    "Comparisons give True or False: > < >= <= == !=",
    "if / elif / else runs only the first true block.",
    "The if line ends with a colon; its block is the indented lines under it.",
    "and, or and not combine conditions.",
  ],
};
