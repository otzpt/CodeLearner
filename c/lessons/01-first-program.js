module.exports = {
  id: "first-program",
  title: "Your first program",
  titlePt: "O teu primeiro programa",
  screens: [
    [
      "A program is a list of instructions for the computer, written in a text file. C is one language for writing them.",
      "The computer cannot read C directly. A program called a compiler first translates your file into a program the computer can run. Then you run that. In a terminal, it is two steps:",
      { code: "cc -Wall -Wextra -o hello hello.c\n./hello", run: false },
      "-Wall -Wextra ask the compiler to warn you about likely mistakes. Every example in this course is compiled and run while you read it. What you see under \"Output\" is what it really printed.",
    ],
    [
      "Here is a whole C program. Every C program has this shape:",
      { code: '#include <stdio.h>\n\nint main(void)\n{\n    printf("Hello!\\n");\n    return 0;\n}' },
      "#include <stdio.h> brings in the printing tools. main is where the program starts. Everything between { and } is its body. return 0 says \"finished fine\".",
    ],
    [
      "printf prints text. Text goes in double quotes, and every instruction ends with a semicolon. \\n means \"new line\": without it, the next print continues on the same line:",
      { code: '#include <stdio.h>\n\nint main(void)\n{\n    printf("one ");\n    printf("two\\n");\n    printf("three\\n");\n    return 0;\n}' },
      { quiz: "What does \\n do inside printf's text?", choices: ["prints a backslash and an n", "starts a new line", "ends the program"], answer: 2, why: "\\n is the new-line character." },
    ],
    [
      "A missing semicolon stops the compiler. The program never runs:",
      { code: '#include <stdio.h>\n\nint main(void)\n{\n    printf("hi\\n")\n    return 0;\n}', error: true },
      "The compiler names the line where it noticed, which is often the line after the real mistake. Reading these messages is a normal part of programming. // starts a comment that the compiler ignores.",
      { tip: "Every instruction ends with ;. Text in double quotes. \\n for a new line." },
    ],
    [
      {
        exercise: "hello",
        task: "Write a program that prints your name on one line and the number 2026 on the next. For the checker, use the name Ana.",
        expected: ["Ana", "2026"],
        hints: ["The file already has #include and main. Write two printf lines inside main.", 'printf("Ana\\n"); prints the name and a new line.'],
        solution: '#include <stdio.h>\n\nint main(void)\n{\n    printf("Ana\\n");\n    printf("2026\\n");\n    return 0;\n}',
      },
    ],
  ],
  recapPt: [
    "Um compilador traduz o ficheiro .c num programa; depois corres esse programa.",
    "Todo o programa em C tem #include <stdio.h> e int main(void) { ... return 0; }.",
    "printf(\"texto\\n\"); mostra texto; \\n muda de linha.",
    "Cada instrução acaba em ;. Sem ela, o programa nem compila.",
  ],
  recap: [
    "A compiler turns your .c file into a program; then you run that program.",
    "Every C program has #include <stdio.h> and int main(void) { ... return 0; }.",
    "printf(\"text\\n\"); prints text; \\n starts a new line.",
    "Every instruction ends with ;. Without it, the program does not even build.",
  ],
};
