module.exports = {
  id: "strings",
  title: "Working with text",
  screens: [
    [
      "Backticks ` make a template string. Anything inside ${ } is filled in:",
      { code: 'const name = "Ana";\nconsole.log(`Hi, ${name}! 2 + 2 is ${2 + 2}`);' },
    ],
    [
      "Only backticks do this. Normal quotes print it as written:",
      { code: 'const name = "Ana";\nconsole.log("Hi, ${name}");' },
    ],
    [
      "Strings have built-in tools, reached with a dot:",
      { code: 'const word = "Hello";\nconsole.log(word.length);\nconsole.log(word.toUpperCase());\nconsole.log(word.includes("ell"));' },
    ],
    [
      "Positions start at 0, so the last one is length - 1:",
      { code: 'const word = "Hello";\nconsole.log(word[0]);\nconsole.log(word[word.length - 1]);' },
      { quiz: 'In "cat", what is at position 1?', choices: ["c", "a", "t"], answer: 2, why: "c is 0, a is 1, t is 2." },
    ],
    [
      {
        exercise: "greeting",
        task: 'With const name = "Bruno", print: Hello, Bruno! Your name has 5 letters. Then print the name in capitals.',
        expected: ["Hello, Bruno! Your name has 5 letters.", "BRUNO"],
        hints: ["${name.length} inside backticks.", "name.toUpperCase()"],
        solution: 'const name = "Bruno";\nconsole.log(`Hello, ${name}! Your name has ${name.length} letters.`);\nconsole.log(name.toUpperCase());',
      },
    ],
  ],
  recap: [
    "`Hi, ${name}` fills in values. Normal quotes do not.",
    ".length, .toUpperCase(), .includes(...)",
    "Positions start at 0; the last is length - 1.",
  ],
};
