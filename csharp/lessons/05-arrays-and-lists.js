module.exports = {
  id: "arrays-and-lists",
  title: "Arrays and lists",
  titlePt: "Arrays e listas",
  screens: [
    [
      "An array holds a fixed number of values of one type. Positions start at 0, and .Length is how many there are:",
      { code: "int[] scores = { 15, 12, 18 };\nConsole.WriteLine(scores[0]);\nConsole.WriteLine(scores[scores.Length - 1]);\nscores[1] = 14;\nConsole.WriteLine(scores.Length);" },
    ],
    [
      "C# checks every position. Going past the end stops the program with a clear error:",
      { code: "int[] scores = { 15, 12, 18 };\nConsole.WriteLine(scores[3]);", error: true },
      { quiz: "An array has 3 values. What is the last valid position?", choices: ["3", "2", "4"], answer: 2, why: "Positions go from 0 to Length - 1." },
    ],
    [
      "An array cannot grow. A List can: Add puts a value at the end, .Count counts. The type goes in < >, and foreach goes through every value:",
      { code: 'List<string> fruits = new();\nfruits.Add("apple");\nfruits.Add("banana");\nConsole.WriteLine($"{fruits[0]} {fruits.Count}");\nforeach (string fruit in fruits)\n{\n    Console.WriteLine($"I like {fruit}");\n}' },
      "new() makes an empty list of the type on the left. Read foreach as \"for each fruit in fruits\".",
      { tip: "Positions start at 0. Arrays are fixed; a List grows with Add." },
    ],
    [
      {
        exercise: "lists",
        task: 'Make a List<string> with "rice", "beans", "oil" and add "salt". Print how many items there are, then every item on its own line starting with "- ".',
        expected: ["4", "- rice", "- beans", "- oil", "- salt"],
        hints: ['List<string> items = new() { "rice", "beans", "oil" }; then items.Add("salt");', 'foreach (string item in items) Console.WriteLine("- " + item);'],
        solution: 'List<string> items = new() { "rice", "beans", "oil" };\nitems.Add("salt");\nConsole.WriteLine(items.Count);\nforeach (string item in items)\n{\n    Console.WriteLine("- " + item);\n}',
      },
    ],
  ],
  recapPt: [
    "int[] a = { 1, 2, 3 }; tem as posições 0 a a.Length - 1.",
    "O C# verifica as posições e para com IndexOutOfRangeException.",
    "List<string> cresce com Add; .Count conta.",
    "foreach (string x in lista) percorre todos os valores.",
  ],
  recap: [
    "int[] a = { 1, 2, 3 }; has positions 0 to a.Length - 1.",
    "C# checks positions and stops with IndexOutOfRangeException.",
    "List<string> grows with Add; .Count counts.",
    "foreach (string x in list) visits every value.",
  ],
};
