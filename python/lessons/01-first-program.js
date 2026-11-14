module.exports = {
  id: "first-program",
  title: "Your first program",
  titlePt: "O teu primeiro programa",
  screens: [
    [
      "A program is a list of instructions for the computer, written in a text file. The computer does them one at a time, from the top line down.",
      "Python is one language for writing those instructions, and also the program that reads a Python file and carries it out. You run a file by typing this in a terminal:",
      { code: "python3 hello.py", run: false },
      "Every example in this course really runs while you read it. What you see under \"Output\" is what Python printed just now.",
    ],
    [
      "The first instruction to learn is print. It shows whatever you put between its brackets, one line each time:",
      { code: 'print("Hello!")\nprint("A second line")' },
      "Text has to be inside quotes. The quotes tell Python where the text starts and ends; they are not printed. The lines come out in the order they are written.",
    ],
    [
      "Numbers do not need quotes, and Python works out a sum before printing it. A line starting with # is a comment: Python skips it, it is a note for people.",
      { code: '# numbers and sums\nprint(42)\nprint(2 + 3)\nprint("2 + 3")' },
      "The last line prints the text 2 + 3, not 5. In quotes, it is just text to print.",
      { quiz: 'What does print("10 + 5") show?', choices: ["15", "10 + 5", "an error"], answer: 2, why: "It is in quotes, so it is text, printed exactly as written." },
    ],
    [
      "Spelling matters, including capital letters. Print, with a capital P, is not print, and Python does not know it:",
      { code: 'Print("hi")', error: true },
      "When something goes wrong, Python stops and prints an error. The course explains each error in plain words, like above. Reading errors is a normal part of programming, not a sign you did something terrible.",
      { tip: "One instruction per line, text in quotes, names spelled exactly." },
    ],
    [
      {
        exercise: "hello",
        task: "Write a program that prints your name on one line and the number 2026 on the next. For the checker, use the name Ana.",
        expected: ["Ana", "2026"],
        hints: ["You need two print lines.", 'Text goes in quotes: print("Ana")', "The number does not need quotes: print(2026)"],
        solution: 'print("Ana")\nprint(2026)',
      },
    ],
  ],
  recapPt: [
    "Um programa são instruções num ficheiro, feitas de cima para baixo. O Python corre-o: python3 ficheiro.py",
    "print(...) mostra uma linha.",
    "O texto vai entre aspas. Os números não. 2 + 3 é calculado; \"2 + 3\" é só texto.",
    "# começa um comentário que o Python ignora.",
    "Um erro para o programa e diz o que correu mal e em que linha.",
  ],
  recap: [
    "A program is instructions in a file, done top to bottom. Python runs it: python3 file.py",
    "print(...) shows one line.",
    "Text goes in quotes. Numbers do not. 2 + 3 is worked out; \"2 + 3\" is just text.",
    "# starts a comment that Python skips.",
    "An error stops the program and says what went wrong and on which line.",
  ],
};
