module.exports = {
  id: "input",
  title: "Asking the user",
  titlePt: "Perguntar ao utilizador",
  screens: [
    [
      "So far every program printed the same thing every time. To react to the person using it, a program has to read what they type.",
      "input(...) shows a question, waits until the person types a line and presses ENTER, and gives back what they typed:",
      { code: 'name = input("What is your name? ")\nprint(f"Hello, {name}!")', output: ["What is your name? {Ana}", "Hello, Ana!"] },
      "In the output above, the bold part is what was typed.",
    ],
    [
      "What input gives back is always a string, even when the person types a number. Remember from lesson 2 what + does with a string:",
      { code: 'text = input("A number: ")\nprint(text * 2)\nprint(int(text) * 2)', output: ["A number: {5}", "55", "10"] },
      'int(...) turns the typed text into a number. text * 2 repeated the string instead.',
      { quiz: "A user types 10. What does print(answer + 5) do?", choices: ["prints 15", "prints 105", "stops with an error"], answer: 3, why: "answer is the string \"10\", and a string + a number is a TypeError. Use int(answer) + 5." },
    ],
    [
      "If the text is not a number, int() stops the program with a ValueError. Lesson 12 shows how to catch that instead of crashing:",
      { code: 'age = int(input("Age? "))', output: ["Age? {twenty}"], error: true },
      { tip: "input() always gives a string. Convert with int() or float() before doing maths." },
    ],
    [
      {
        exercise: "ask",
        task: 'Ask "Your name? " and then "Your age? ". Print: Hi NAME, next year you will be AGE+1.',
        expected: ["Your name? {Rita}", "Your age? {20}", "Hi Rita, next year you will be 21."],
        hints: ['name = input("Your name? ")', "int(age) + 1 inside the f-string."],
        solution: 'name = input("Your name? ")\nage = input("Your age? ")\nprint(f"Hi {name}, next year you will be {int(age) + 1}.")',
      },
    ],
  ],
  recapPt: [
    "input(\"pergunta\") espera por uma linha e devolve-a.",
    "Devolve sempre uma string: int() ou float() antes de fazer contas.",
    "int(\"twenty\") dá ValueError.",
  ],
  recap: [
    "input(\"question\") waits for a line and gives it back.",
    "It always gives a string: int() or float() before doing maths.",
    "int(\"twenty\") is a ValueError.",
  ],
};
