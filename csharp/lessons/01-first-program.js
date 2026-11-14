module.exports = {
  id: "first-program",
  title: "Your first program",
  titlePt: "O teu primeiro programa",
  screens: [
    [
      "A program is a list of instructions for the computer, written in a text file. C# is one language for writing them.",
      "The dotnet command checks your file, translates it, and runs it, in one step:",
      { code: "dotnet run hello.cs", run: false },
      "Every example in this course really runs while you read it. What you see under \"Output\" is what it printed just now.",
    ],
    [
      "Console.WriteLine prints a line. Text goes in double quotes, and every instruction ends with a semicolon. That one line is a whole C# program:",
      { code: 'Console.WriteLine("Hello!");\nConsole.WriteLine("A second line");' },
      "The program runs from the top line down.",
    ],
    [
      "Numbers need no quotes, and a sum is worked out first. Console.Write (without Line) does not end the line. // starts a comment that C# ignores:",
      { code: '// numbers and sums\nConsole.Write("two plus three is ");\nConsole.WriteLine(2 + 3);\nConsole.WriteLine("2 + 3");' },
      "In quotes, 2 + 3 is just text.",
      { quiz: 'What does Console.WriteLine("10 + 5"); show?', choices: ["15", "10 + 5", "an error"], answer: 2, why: "In quotes, it is text, shown exactly as written." },
    ],
    [
      "A missing semicolon stops C# before anything runs:",
      { code: 'Console.WriteLine("hi")', error: true },
      "Capitals matter too: console.writeline is not Console.WriteLine. Reading error messages is a normal part of programming.",
      { tip: "Every instruction ends with ;. Text in double quotes. Names spelled exactly, capitals included." },
    ],
    [
      {
        exercise: "hello",
        task: "Write a program that prints your name on one line and the number 2026 on the next. For the checker, use the name Ana.",
        expected: ["Ana", "2026"],
        hints: ["Two Console.WriteLine lines.", 'Console.WriteLine("Ana");'],
        solution: 'Console.WriteLine("Ana");\nConsole.WriteLine(2026);',
      },
    ],
  ],
  recapPt: [
    "dotnet run ficheiro.cs verifica, traduz e corre o programa.",
    "Console.WriteLine(...) mostra uma linha; Console.Write não muda de linha.",
    "Texto entre aspas; números sem aspas.",
    "Cada instrução acaba em ;. As maiúsculas contam.",
  ],
  recap: [
    "dotnet run file.cs checks, translates and runs the program.",
    "Console.WriteLine(...) prints a line; Console.Write does not end the line.",
    "Text in quotes; numbers without.",
    "Every instruction ends with ;. Capitals count.",
  ],
};
