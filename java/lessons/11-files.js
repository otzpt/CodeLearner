module.exports = {
  id: "files",
  title: "Reading and writing files",
  titlePt: "Ler e escrever ficheiros",
  screens: [
    [
      "java.nio.file has short tools for files. Files.writeString writes a whole file, making it or replacing it. Files.readString reads it all back. Methods that touch files can throw IOException, so main must say throws IOException:",
      { code: 'import java.io.IOException;\nimport java.nio.file.Files;\nimport java.nio.file.Path;\n\npublic class Main {\n    public static void main(String[] args) throws IOException {\n        Path file = Path.of("notes.txt");\n        Files.writeString(file, "first line\\nsecond line\\n");\n        System.out.print(Files.readString(file));\n    }\n}' },
    ],
    [
      "Files.readAllLines gives a list with one String per line, without the \\n:",
      { code: 'import java.io.IOException;\nimport java.nio.file.Files;\nimport java.nio.file.Path;\n\npublic class Main {\n    public static void main(String[] args) throws IOException {\n        Path file = Path.of("list.txt");\n        Files.writeString(file, "milk\\nbread\\n");\n        for (String line : Files.readAllLines(file)) {\n            System.out.println("- " + line);\n        }\n    }\n}' },
    ],
    [
      "Reading a file that does not exist throws NoSuchFileException, which stops the program unless it is caught:",
      { code: 'import java.io.IOException;\nimport java.nio.file.Files;\nimport java.nio.file.Path;\n\npublic class Main {\n    public static void main(String[] args) throws IOException {\n        System.out.println(Files.readString(Path.of("missing.txt")));\n    }\n}', error: true },
      { quiz: "Why does main say throws IOException?", choices: ["it makes files faster", "file methods may fail, and Java wants that said or caught", "it is decoration"], answer: 2, why: "Java insists that an IOException is either caught or declared." },
    ],
    [
      {
        exercise: "files",
        task: 'Write the lines "red", "green" and "blue" to colours.txt. Then read the file back and print each line with its number: 1 red, 2 green, 3 blue.',
        expected: ["1 red", "2 green", "3 blue"],
        hints: ['Files.writeString(Path.of("colours.txt"), "red\\ngreen\\nblue\\n");', "Files.readAllLines, then a counter in a for loop."],
        solution: 'import java.io.IOException;\nimport java.nio.file.Files;\nimport java.nio.file.Path;\n\npublic class Main {\n    public static void main(String[] args) throws IOException {\n        Path file = Path.of("colours.txt");\n        Files.writeString(file, "red\\ngreen\\nblue\\n");\n        int number = 1;\n        for (String line : Files.readAllLines(file)) {\n            System.out.println(number + " " + line);\n            number++;\n        }\n    }\n}',
      },
    ],
  ],
  recapPt: [
    "Files.writeString(Path.of(nome), texto) escreve um ficheiro inteiro.",
    "Files.readString lê tudo; Files.readAllLines dá uma lista de linhas.",
    "Métodos de ficheiros podem lançar IOException: apanha-a ou escreve throws IOException.",
  ],
  recap: [
    "Files.writeString(Path.of(name), text) writes a whole file.",
    "Files.readString reads it all; Files.readAllLines gives a list of lines.",
    "File methods may throw IOException: catch it or write throws IOException.",
  ],
};
