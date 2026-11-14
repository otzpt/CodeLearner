module.exports = {
  id: "files",
  title: "Reading and writing files",
  titlePt: "Ler e escrever ficheiros",
  screens: [
    [
      "Variables disappear when a program ends. To keep something, write it to a file. fopen opens a file and gives back a FILE pointer, or NULL if it could not. \"w\" means write: it makes the file, or empties it if it exists. fprintf writes like printf, and fclose closes it:",
      { code: '#include <stdio.h>\n\nint main(void)\n{\n    FILE *f = fopen("notes.txt", "w");\n    if (f == NULL) {\n        printf("could not open\\n");\n        return 1;\n    }\n    fprintf(f, "first line\\n");\n    fprintf(f, "score %d\\n", 42);\n    fclose(f);\n    printf("saved\\n");\n    return 0;\n}' },
    ],
    [
      "\"r\" opens a file for reading. fgets reads one line at a time into a char array, and gives back NULL at the end of the file:",
      { code: '#include <stdio.h>\n\nint main(void)\n{\n    FILE *f = fopen("list.txt", "w");\n    if (f == NULL) return 1;\n    fprintf(f, "milk\\nbread\\n");\n    fclose(f);\n\n    char line[100];\n    f = fopen("list.txt", "r");\n    if (f == NULL) return 1;\n    while (fgets(line, sizeof line, f) != NULL) {\n        printf("- %s", line);\n    }\n    fclose(f);\n    return 0;\n}' },
      "fgets keeps the \\n at the end of each line, which is why the printf has none.",
    ],
    [
      "Opening a file that does not exist for reading gives NULL. Always check it before using the pointer:",
      { code: '#include <stdio.h>\n\nint main(void)\n{\n    FILE *f = fopen("missing.txt", "r");\n    if (f == NULL) {\n        printf("missing.txt could not be opened\\n");\n        return 1;\n    }\n    fclose(f);\n    return 0;\n}', error: true },
      "return 1 from main tells whoever ran the program that it failed.",
      { quiz: 'What does fopen("data.txt", "w") do to a file that already has text?', choices: ["adds to the end", "empties it first", "refuses to open it"], answer: 2, why: '"w" starts the file empty. "a" (append) would add to the end.' },
    ],
    [
      {
        exercise: "files",
        task: 'Write the lines "red", "green" and "blue" to colours.txt. Then read the file back and print each line with its number: 1 red, 2 green, 3 blue.',
        expected: ["1 red", "2 green", "3 blue"],
        hints: ['fopen("colours.txt", "w"), three fprintf calls each ending in \\n, fclose.', 'Read with fgets in a while loop, keep a counter, and printf("%d %s", number, line).'],
        solution: '#include <stdio.h>\n\nint main(void)\n{\n    FILE *f = fopen("colours.txt", "w");\n    if (f == NULL) return 1;\n    fprintf(f, "red\\ngreen\\nblue\\n");\n    fclose(f);\n\n    char line[100];\n    f = fopen("colours.txt", "r");\n    if (f == NULL) return 1;\n    int number = 1;\n    while (fgets(line, sizeof line, f) != NULL) {\n        printf("%d %s", number, line);\n        number++;\n    }\n    fclose(f);\n    return 0;\n}',
      },
    ],
  ],
  recapPt: [
    "fopen(nome, \"w\") escreve, \"r\" lê, \"a\" acrescenta; devolve NULL se falhar.",
    "fprintf(f, ...) escreve como o printf; fclose(f) fecha.",
    "fgets(line, sizeof line, f) lê uma linha e mantém o \\n; dá NULL no fim.",
    "Verifica sempre o NULL antes de usar o ficheiro.",
  ],
  recap: [
    "fopen(name, \"w\") writes, \"r\" reads, \"a\" appends; it gives NULL on failure.",
    "fprintf(f, ...) writes like printf; fclose(f) closes.",
    "fgets(line, sizeof line, f) reads one line and keeps its \\n; NULL at the end.",
    "Always check for NULL before using the file.",
  ],
};
