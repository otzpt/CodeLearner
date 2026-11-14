module.exports = {
  id: "arrays",
  title: "Arrays: many values in a row",
  titlePt: "Arrays: muitos valores seguidos",
  screens: [
    [
      "An array holds a fixed number of values of one type, side by side. The size goes in [ ] when it is made. Each value has a position, and counting starts at 0:",
      { code: '#include <stdio.h>\n\nint main(void)\n{\n    int scores[3] = {15, 12, 18};\n    printf("%d\\n", scores[0]);\n    printf("%d\\n", scores[2]);\n    scores[1] = 14;\n    printf("%d\\n", scores[1]);\n    return 0;\n}' },
      "So an array of 3 has positions 0, 1 and 2. There is no position 3.",
    ],
    [
      "A for loop goes through every position. Keep the size in one place, so the loop and the array cannot disagree:",
      { code: '#include <stdio.h>\n\n#define COUNT 4\n\nint main(void)\n{\n    int prices[COUNT] = {2, 15, 30, 7};\n    int total = 0;\n    for (int i = 0; i < COUNT; i++) {\n        total += prices[i];\n    }\n    printf("total %d\\n", total);\n    return 0;\n}' },
      "#define COUNT 4 makes COUNT stand for 4 everywhere in the file. i < COUNT stops at the last real position, 3.",
    ],
    [
      "C does not check positions for you. Reading scores[3] of a 3-value array reads whatever memory comes next, and writing there can damage other variables or crash the program. It may even seem to work, which is worse. The compiler can sometimes warn, but not always.",
      "This course will not run such code: what it does is not defined, so the result tells you nothing. Lesson 13 shows a tool that catches it.",
      { quiz: "An array is made with int a[5]. What is the last valid position?", choices: ["5", "4", "6"], answer: 2, why: "Positions go from 0 to size - 1." },
    ],
    [
      "sizeof gives the size in bytes. Dividing the whole array's size by one value's size gives how many values it holds:",
      { code: '#include <stdio.h>\n\nint main(void)\n{\n    int days[] = {31, 28, 31, 30};\n    int count = sizeof days / sizeof days[0];\n    printf("%d months\\n", count);\n    return 0;\n}' },
      "Leaving the [ ] empty lets the compiler count the values. This sizeof trick only works where the array was made, not inside a function it was passed to.",
      { tip: "Positions run from 0 to size - 1, and C does not stop you going past the end." },
    ],
    [
      {
        exercise: "arrays",
        task: "Make an int array with the values 4, 8, 15, 16, 23, 42. Print the first and last value on one line, separated by a space, then print the largest value and the total.",
        expected: ["4 42", "largest 42", "total 108"],
        hints: ["The last position is count - 1.", "Start largest at the first value; in a loop, replace it when a bigger one appears."],
        solution: '#include <stdio.h>\n\nint main(void)\n{\n    int numbers[] = {4, 8, 15, 16, 23, 42};\n    int count = sizeof numbers / sizeof numbers[0];\n    printf("%d %d\\n", numbers[0], numbers[count - 1]);\n    int largest = numbers[0];\n    int total = 0;\n    for (int i = 0; i < count; i++) {\n        if (numbers[i] > largest) {\n            largest = numbers[i];\n        }\n        total += numbers[i];\n    }\n    printf("largest %d\\n", largest);\n    printf("total %d\\n", total);\n    return 0;\n}',
      },
    ],
  ],
  recapPt: [
    "int a[3] = {1, 2, 3}; tem as posições 0, 1 e 2.",
    "O C não verifica as posições: passar do fim é um erro grave e silencioso.",
    "Um ciclo for com i < tamanho percorre todas as posições.",
    "sizeof a / sizeof a[0] conta os valores, mas só onde o array foi criado.",
  ],
  recap: [
    "int a[3] = {1, 2, 3}; has positions 0, 1 and 2.",
    "C does not check positions: going past the end is a serious, silent bug.",
    "A for loop with i < size visits every position.",
    "sizeof a / sizeof a[0] counts the values, but only where the array was made.",
  ],
};
