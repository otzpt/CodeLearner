module.exports = {
  id: "strings",
  title: "Working with text",
  titlePt: "Trabalhar com texto",
  screens: [
    [
      "Joining text with + gets messy with many pieces, and numbers need str(). An f-string is easier: put f before the quotes, and any value inside { }:",
      { code: 'name = "Ana"\nage = 25\nprint(f"{name} is {age} years old")\nprint(f"Next year: {age + 1}")' },
      "Whatever is inside { } is worked out first, even a sum like age + 1, and numbers need no str().",
    ],
    [
      "Without the f, the braces are just text:",
      { code: 'name = "Ana"\nprint("Hello, {name}")' },
      { quiz: 'Which line prints Hi, Ana when name is "Ana"?', choices: ['print("Hi, {name}")', 'print(f"Hi, {name}")', "both"], answer: 2, why: "Only an f-string fills in { }." },
    ],
    [
      "Strings come with tools built in. len(...) counts the characters, and the others are reached with a dot after the string:",
      { code: 'word = "Hello"\nprint(len(word))\nprint(word.upper())\nprint(word.lower())\nprint("ell" in word)' },
      "upper() and lower() give a changed copy; word itself stays \"Hello\". in answers True or False.",
    ],
    [
      "Each character has a position number, and counting starts at 0, not 1. -1 means the last character. A slice [start:end] takes a piece, up to but not including end:",
      { code: 'word = "Hello"\nprint(word[0])\nprint(word[-1])\nprint(word[1:3])' },
      "So a 5-letter word has positions 0 to 4, and word[1:3] is positions 1 and 2. This counting from 0 comes back with lists in lesson 7.",
      { tip: "Positions start at 0. A slice stops just before its end number." },
    ],
    [
      {
        exercise: "greeting",
        task: 'Create name = "Bruno". Use one f-string to print: Hello, Bruno! Your name has 5 letters. Then print the name in capitals, then its first letter.',
        expected: ["Hello, Bruno! Your name has 5 letters.", "BRUNO", "B"],
        hints: ["Inside the f-string, {len(name)} gives the number of letters.", "name.upper() gives the capitals and name[0] the first letter."],
        solution: 'name = "Bruno"\nprint(f"Hello, {name}! Your name has {len(name)} letters.")\nprint(name.upper())\nprint(name[0])',
      },
    ],
  ],
  recapPt: [
    "f-strings preenchem { }: f\"Hi, {name}\". Sem o f, as chavetas são só texto.",
    "len(), .upper(), .lower() e in funcionam com qualquer string.",
    "As posições começam em 0; -1 é a última. word[1:3] vai da posição 1 até antes da 3.",
  ],
  recap: [
    "f-strings fill in { }: f\"Hi, {name}\". Without the f, braces are plain text.",
    "len(), .upper(), .lower() and in work on any string.",
    "Positions start at 0; -1 is the last. word[1:3] goes from position 1 up to before 3.",
  ],
};
