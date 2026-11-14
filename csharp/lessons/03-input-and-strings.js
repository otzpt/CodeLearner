module.exports = {
  id: "input-and-strings",
  title: "Text and reading input",
  titlePt: "Texto e ler entrada",
  screens: [
    [
      "A string holds text. A $ before the quotes lets you put values inside { }. .Length counts the characters:",
      { code: 'string name = "Ana";\nint age = 25;\nConsole.WriteLine($"{name} is {age} years old");\nConsole.WriteLine($"Next year: {age + 1}");\nConsole.WriteLine(name.Length);' },
      "Whatever is inside { } is worked out first, even a sum. Without the $, the braces are just text.",
    ],
    [
      "Console.ReadLine() waits for the user to type a line and gives it back. ?? \"\" means \"or empty text if there was nothing to read\":",
      { code: 'Console.Write("What is your name? ");\nstring name = Console.ReadLine() ?? "";\nConsole.WriteLine($"Hello, {name}!");', output: ["What is your name? {Ana}", "Hello, Ana!"] },
      "The bold part is what was typed.",
    ],
    [
      "What ReadLine gives back is always a string, even when the user types a number. int.Parse turns it into a number:",
      { code: 'Console.Write("A number: ");\nstring text = Console.ReadLine() ?? "";\nConsole.WriteLine(text + 1);\nConsole.WriteLine(int.Parse(text) + 1);', output: ["A number: {5}", "51", "6"] },
      "text + 1 joined the string and the number. If the text is not a number, int.Parse stops the program; int.TryParse checks first (lesson 10).",
      { quiz: "A user types 10 and the program prints answer + 5, where answer is the string read. What appears?", choices: ["15", "105"], answer: 2, why: "A string + a number joins them. Use int.Parse(answer) + 5." },
    ],
    [
      "Each character has a position, starting at 0. [ ] reads one, and Substring(start, length) takes a piece. == compares the text of two strings:",
      { code: 'string word = "Hello";\nConsole.WriteLine(word[0]);\nConsole.WriteLine(word[word.Length - 1]);\nConsole.WriteLine(word.Substring(1, 3));\nConsole.WriteLine(word.ToUpper() == "HELLO");' },
      { tip: "Console.ReadLine() always gives text; int.Parse makes it a number. Positions start at 0." },
    ],
    [
      {
        exercise: "ask",
        task: 'Ask "Your name? " and then "Your age? ". Print: Hi NAME, next year you will be AGE+1.',
        expected: ["Your name? {Rita}", "Your age? {20}", "Hi Rita, next year you will be 21."],
        hints: ['string name = Console.ReadLine() ?? "";', "int.Parse(age) + 1 inside { } in a $ string."],
        solution: 'Console.Write("Your name? ");\nstring name = Console.ReadLine() ?? "";\nConsole.Write("Your age? ");\nstring age = Console.ReadLine() ?? "";\nConsole.WriteLine($"Hi {name}, next year you will be {int.Parse(age) + 1}.");',
      },
    ],
  ],
  recapPt: [
    "$\"... {valor} ...\" põe valores dentro do texto; .Length conta os caracteres.",
    "Console.ReadLine() ?? \"\" lê uma linha de texto.",
    "Lê sempre texto: int.Parse(texto) transforma-o em número.",
    "As posições começam em 0; == compara o texto de duas strings.",
  ],
  recap: [
    "$\"... {value} ...\" puts values inside text; .Length counts characters.",
    "Console.ReadLine() ?? \"\" reads a line of text.",
    "It always reads text: int.Parse(text) makes it a number.",
    "Positions start at 0; == compares the text of two strings.",
  ],
};
