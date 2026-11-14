module.exports = {
  id: "files",
  title: "Reading and writing files",
  titlePt: "Ler e escrever ficheiros",
  screens: [
    [
      "Variables disappear when a program ends. To keep something, write it to a file. open(name, \"w\") opens a file for writing (making it, or emptying it if it exists), and with closes it again when the block ends:",
      { code: 'with open("notes.txt", "w") as f:\n    f.write("first line\\n")\n    f.write("second line\\n")\nprint("saved")' },
      "\\n is a new line. write does not add one for you.",
    ],
    [
      'open(name) on its own opens a file for reading. .read() gives the whole file as one string:',
      { code: 'with open("notes.txt", "w") as f:\n    f.write("apples\\npears\\n")\n\nwith open("notes.txt") as f:\n    text = f.read()\nprint(text.upper())' },
    ],
    [
      "Going through a file with for gives one line at a time. Each line still ends with its \\n, which .strip() removes:",
      { code: 'with open("list.txt", "w") as f:\n    f.write("milk\\nbread\\neggs\\n")\n\nwith open("list.txt") as f:\n    for line in f:\n        print("-", line.strip())' },
      { quiz: 'What does open("data.txt", "w") do to a file that already has text in it?', choices: ["adds to the end", "empties it first", "refuses to open it"], answer: 2, why: '"w" starts the file empty. "a" (append) would add to the end.' },
    ],
    [
      "Opening a file that does not exist for reading is an error:",
      { code: 'with open("missing.txt") as f:\n    print(f.read())', error: true },
      { tip: 'with open(...) closes the file for you. "w" empties it; "a" adds to it.' },
    ],
    [
      {
        exercise: "files",
        task: 'Write the three lines "red", "green" and "blue" to colours.txt. Then read the file back and print each line with its number: 1 red, 2 green, 3 blue.',
        expected: ["1 red", "2 green", "3 blue"],
        hints: ['with open("colours.txt", "w") as f: and three f.write calls, each ending in \\n.', "Keep a counter that starts at 1, or use for number, line in enumerate(f, 1):"],
        solution: 'with open("colours.txt", "w") as f:\n    f.write("red\\n")\n    f.write("green\\n")\n    f.write("blue\\n")\n\nwith open("colours.txt") as f:\n    number = 1\n    for line in f:\n        print(number, line.strip())\n        number += 1',
      },
    ],
  ],
  recapPt: [
    "with open(nome, \"w\") as f: escreve um ficheiro e fecha-o no fim.",
    "f.write(texto) não acrescenta \\n; f.read() dá o ficheiro todo.",
    "for line in f: dá uma linha de cada vez; .strip() tira o \\n.",
    "Ler um ficheiro que não existe dá FileNotFoundError.",
  ],
  recap: [
    "with open(name, \"w\") as f: writes a file and closes it at the end.",
    "f.write(text) adds no \\n; f.read() gives the whole file.",
    "for line in f: gives one line at a time; .strip() removes the \\n.",
    "Reading a file that does not exist is a FileNotFoundError.",
  ],
};
