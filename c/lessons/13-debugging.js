module.exports = {
  id: "debugging",
  title: "When things go wrong",
  titlePt: "Quando as coisas correm mal",
  screens: [
    [
      "C problems come in three kinds. The compiler refuses the code (an error). The compiler builds it but warns (often a real bug). Or it builds fine and goes wrong while running, which is the hardest kind.",
      "A compile error names the line and what it expected. Fix the first one first: one mistake often causes several messages after it.",
      { code: '#include <stdio.h>\n\nint main(void)\n{\n    int total = 10;\n    printf("%d\\n", totl);\n    return 0;\n}', error: true },
    ],
    [
      "Going past the end of an array usually gives no message at all. Building with -fsanitize=address adds checks that stop the program at the exact line and say what happened:",
      { code: '#include <stdio.h>\n\nint main(void)\n{\n    int scores[3] = {15, 12, 18};\n    int total = 0;\n    for (int i = 0; i <= 3; i++) {\n        total += scores[i];\n    }\n    printf("%d\\n", total);\n    return 0;\n}', flags: ["-g", "-fsanitize=address"], error: true },
      "The bug is <= 3: the loop also reads scores[3]. To build like this yourself: cc -g -fsanitize=address -o program main.c",
    ],
    [
      "The same tool catches memory used after free, and memory never freed:",
      { code: '#include <stdio.h>\n#include <stdlib.h>\n\nint main(void)\n{\n    int *n = malloc(sizeof(int));\n    if (n == NULL) return 1;\n    *n = 5;\n    free(n);\n    printf("%d\\n", *n);\n    return 0;\n}', flags: ["-g", "-fsanitize=address"], error: true },
      { quiz: "A program builds with no warnings and prints a wrong number. What can find an out-of-range array read?", choices: ["nothing, it is fine", "building with -fsanitize=address", "adding more printf"], answer: 2, why: "The sanitizer checks every array and memory access while it runs." },
    ],
    [
      "When the output is wrong but nothing crashes, print the values as the program runs, to see where they stop being what you expected:",
      { code: '#include <stdio.h>\n\nint main(void)\n{\n    int total = 0;\n    for (int i = 1; i <= 3; i++) {\n        total += i * i;\n        printf("debug: i=%d total=%d\\n", i, total);\n    }\n    printf("%d\\n", total);\n    return 0;\n}' },
      { tip: "First error first. Read warnings. Build with -fsanitize=address while learning." },
    ],
    [
      {
        exercise: "debugging",
        task: "This program should print the average of 4, 8 and 16 with two decimals, 9.33, but it prints 9.00. Copy it into main in your file, run it, and fix it:\n\n    int numbers[] = {4, 8, 16};\n    int total = 0;\n    for (int i = 0; i < 3; i++) total += numbers[i];\n    printf(\"%.2f\\n\", (double)(total / 3));",
        expected: ["9.33"],
        hints: ["Print total and total / 3 on their own to see where the decimals disappear.", "total / 3 is an int divided by an int, so the decimals are gone before (double) is applied. Turn total into a double first: (double)total / 3."],
        solution: '#include <stdio.h>\n\nint main(void)\n{\n    int numbers[] = {4, 8, 16};\n    int total = 0;\n    for (int i = 0; i < 3; i++) total += numbers[i];\n    printf("%.2f\\n", (double)total / 3);\n    return 0;\n}',
      },
    ],
  ],
  recapPt: [
    "Três tipos de problemas: erros de compilação, avisos e erros ao correr.",
    "Corrige primeiro o primeiro erro.",
    "-fsanitize=address apanha leituras fora do array e memória usada depois do free.",
    "Quando o resultado está errado, mostra os valores pelo caminho com printf.",
  ],
  recap: [
    "Three kinds of problems: compile errors, warnings and runtime bugs.",
    "Fix the first error first.",
    "-fsanitize=address catches out-of-range reads and memory used after free.",
    "When output is wrong, print the values along the way with printf.",
  ],
};
