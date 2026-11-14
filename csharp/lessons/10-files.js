module.exports = {
  id: "files",
  title: "Reading and writing files",
  titlePt: "Ler e escrever ficheiros",
  screens: [
    [
      "File.WriteAllText writes a whole file, making it or replacing it. File.ReadAllText reads it all back:",
      { code: 'File.WriteAllText("notes.txt", "first line\\nsecond line\\n");\nConsole.Write(File.ReadAllText("notes.txt"));' },
      "\\n is a new line inside a string.",
    ],
    [
      "File.ReadAllLines gives an array with one string per line, without the \\n. File.AppendAllText adds to the end instead of replacing:",
      { code: 'File.WriteAllText("list.txt", "milk\\n");\nFile.AppendAllText("list.txt", "bread\\n");\nforeach (string line in File.ReadAllLines("list.txt"))\n{\n    Console.WriteLine("- " + line);\n}' },
    ],
    [
      "Reading a file that does not exist throws FileNotFoundException. File.Exists checks first:",
      { code: 'Console.WriteLine(File.Exists("missing.txt"));\nConsole.WriteLine(File.ReadAllText("missing.txt"));', error: true },
      { quiz: "What does File.WriteAllText do to a file that already has text?", choices: ["adds to the end", "replaces it"], answer: 2, why: "WriteAllText replaces; AppendAllText adds." },
    ],
    [
      {
        exercise: "files",
        task: 'Write the lines "red", "green" and "blue" to colours.txt. Then read the file back and print each line with its number: 1 red, 2 green, 3 blue.',
        expected: ["1 red", "2 green", "3 blue"],
        hints: ['File.WriteAllText("colours.txt", "red\\ngreen\\nblue\\n");', "File.ReadAllLines, then a counter in a foreach."],
        solution: 'File.WriteAllText("colours.txt", "red\\ngreen\\nblue\\n");\nint number = 1;\nforeach (string line in File.ReadAllLines("colours.txt"))\n{\n    Console.WriteLine($"{number} {line}");\n    number++;\n}',
      },
    ],
  ],
  recapPt: [
    "File.WriteAllText(nome, texto) escreve um ficheiro inteiro; AppendAllText acrescenta.",
    "File.ReadAllText lê tudo; File.ReadAllLines dá uma linha por posição.",
    "Ler um ficheiro que não existe lança FileNotFoundException; File.Exists verifica antes.",
  ],
  recap: [
    "File.WriteAllText(name, text) writes a whole file; AppendAllText adds to it.",
    "File.ReadAllText reads it all; File.ReadAllLines gives one line per position.",
    "Reading a missing file throws FileNotFoundException; File.Exists checks first.",
  ],
};
