module.exports = {
  id: "files",
  title: "Reading and writing files",
  titlePt: "Ler e escrever ficheiros",
  screens: [
    [
      "std::fs::write writes a whole file, making it or replacing it. std::fs::read_to_string reads it all back. Both give a Result, because files can fail:",
      { code: 'use std::fs;\n\nfn main() {\n    fs::write("notes.txt", "first line\\nsecond line\\n").unwrap();\n    let text = fs::read_to_string("notes.txt").unwrap();\n    print!("{}", text);\n}' },
    ],
    [
      ".lines() goes through a text one line at a time, without the \\n:",
      { code: 'use std::fs;\n\nfn main() {\n    fs::write("list.txt", "milk\\nbread\\n").unwrap();\n    let text = fs::read_to_string("list.txt").unwrap();\n    for line in text.lines() {\n        println!("- {}", line);\n    }\n}' },
    ],
    [
      "Reading a file that does not exist gives an Err. Handle it with match instead of unwrap:",
      { code: 'use std::fs;\n\nfn main() {\n    match fs::read_to_string("missing.txt") {\n        Ok(text) => print!("{}", text),\n        Err(e) => println!("could not read missing.txt: {}", e),\n    }\n}' },
      { quiz: "What does fs::write do to a file that already has text?", choices: ["adds to the end", "replaces it"], answer: 2, why: "fs::write replaces the whole file." },
    ],
    [
      {
        exercise: "files",
        task: 'Write the lines "red", "green" and "blue" to colours.txt. Then read the file back and print each line with its number: 1 red, 2 green, 3 blue.',
        expected: ["1 red", "2 green", "3 blue"],
        hints: ['fs::write("colours.txt", "red\\ngreen\\nblue\\n").unwrap();', "for (i, line) in text.lines().enumerate() gives positions from 0; print i + 1."],
        solution: 'use std::fs;\n\nfn main() {\n    fs::write("colours.txt", "red\\ngreen\\nblue\\n").unwrap();\n    let text = fs::read_to_string("colours.txt").unwrap();\n    for (i, line) in text.lines().enumerate() {\n        println!("{} {}", i + 1, line);\n    }\n}',
      },
    ],
  ],
  recapPt: [
    "fs::write(nome, texto) escreve um ficheiro inteiro.",
    "fs::read_to_string(nome) lê tudo; .lines() dá uma linha de cada vez.",
    "Os dois devolvem Result: um ficheiro que não existe dá Err.",
  ],
  recap: [
    "fs::write(name, text) writes a whole file.",
    "fs::read_to_string(name) reads it all; .lines() gives one line at a time.",
    "Both give a Result: a missing file is an Err.",
  ],
};
