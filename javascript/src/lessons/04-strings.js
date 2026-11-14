module.exports = {
  id: "strings",
  title: "Working with text",
  titlePt: "Trabalhar com texto",
  screens: [
    [
      "Joining text with + works, but gets messy with many pieces. A template string is easier: write it between backticks ` (usually the key left of 1), and put any value inside ${ }:",
      { code: 'const name = "Ana";\nconst age = 25;\nconsole.log(`${name} is ${age} years old`);\nconsole.log(`Next year: ${age + 1}`);' },
      "Whatever is inside ${ } is worked out first, even a sum like age + 1.",
    ],
    [
      "This only works with backticks. Inside normal quotes, ${name} is just text:",
      { code: 'const name = "Ana";\nconsole.log("Hello, ${name}");\nconsole.log(`Hello, ${name}`);' },
      { quiz: "Which line prints Hi, Ana when name is \"Ana\"?", choices: ['console.log("Hi, ${name}");', "console.log(`Hi, ${name}`);", "both"], answer: 2, why: "Only backticks fill in ${ }." },
    ],
    [
      "Strings come with tools built in. You reach them with a dot after the string:",
      { code: 'const word = "Hello";\nconsole.log(word.length);\nconsole.log(word.toUpperCase());\nconsole.log(word.toLowerCase());\nconsole.log(word.includes("ell"));' },
      ".length is how many characters there are. toUpperCase() and toLowerCase() give a changed copy; word itself stays \"Hello\". includes(...) answers true or false.",
    ],
    [
      "Each character has a position number, and counting starts at 0, not 1. The first character is at position 0:",
      { code: 'const word = "Hello";\nconsole.log(word[0]);\nconsole.log(word[1]);\nconsole.log(word[word.length - 1]);' },
      "So the last character is at length - 1. A 5-letter word has positions 0 to 4. This counting from 0 comes back with lists in lesson 7.",
      { tip: "Positions start at 0. The last one is length - 1." },
    ],
    [
      {
        exercise: "greeting",
        task: 'Create const name = "Bruno". Use one template string to print: Hello, Bruno! Your name has 5 letters. Then print the name in capitals.',
        expected: ["Hello, Bruno! Your name has 5 letters.", "BRUNO"],
        hints: ["Inside backticks, ${name.length} gives the number of letters.", "name.toUpperCase() gives the capitals."],
        solution: 'const name = "Bruno";\nconsole.log(`Hello, ${name}! Your name has ${name.length} letters.`);\nconsole.log(name.toUpperCase());',
      },
    ],
  ],
  recapPt: [
    "Template strings usam acentos graves e preenchem ${ }: `Hi, ${name}`.",
    "Com aspas normais, ${name} sai como texto.",
    ".length, .toUpperCase(), .toLowerCase(), .includes(...) existem em todas as strings.",
    "As posições começam em 0; o último carácter está em length - 1.",
  ],
  recap: [
    "Template strings use backticks and fill in ${ }: `Hi, ${name}`.",
    "Normal quotes print ${name} as plain text.",
    ".length, .toUpperCase(), .toLowerCase(), .includes(...) are built into every string.",
    "Positions start at 0; the last character is at length - 1.",
  ],
};
