module.exports = {
  id: "pointers",
  title: "Pointers: where a value lives",
  titlePt: "Ponteiros: onde vive um valor",
  screens: [
    [
      "Every variable lives somewhere in memory, at an address. &x gives the address of x. A pointer is a variable that holds an address. int *p means \"p holds the address of an int\":",
      { code: '#include <stdio.h>\n\nint main(void)\n{\n    int x = 42;\n    int *p = &x;\n    printf("x is %d\\n", x);\n    printf("p points to %d\\n", *p);\n    return 0;\n}' },
      "*p means \"the value at the address in p\". So *p is x.",
    ],
    [
      "Changing *p changes x itself, because they are the same place in memory:",
      { code: '#include <stdio.h>\n\nint main(void)\n{\n    int x = 42;\n    int *p = &x;\n    *p = 7;\n    printf("x is now %d\\n", x);\n    return 0;\n}' },
      { quiz: "After int x = 1; int *p = &x; *p = 5; what is x?", choices: ["1", "5", "the address of x"], answer: 2, why: "*p is x, so *p = 5 stores 5 in x." },
    ],
    [
      "This is how a function can change the caller's variable. Lesson 7 showed that parameters are copies. Pass the address instead, and the function writes through it:",
      { code: '#include <stdio.h>\n\nvoid swap(int *a, int *b)\n{\n    int saved = *a;\n    *a = *b;\n    *b = saved;\n}\n\nint main(void)\n{\n    int x = 1, y = 2;\n    swap(&x, &y);\n    printf("x=%d y=%d\\n", x, y);\n    return 0;\n}' },
      "It is also why scanf needs &age: it must know where age lives to fill it.",
    ],
    [
      "A pointer that points nowhere is written NULL. Reading through it crashes the program, because address 0 is never yours:",
      { code: '#include <stdio.h>\n\nint main(void)\n{\n    int *p = NULL;\n    printf("before\\n");\n    fflush(stdout);\n    printf("%d\\n", *p);\n    return 0;\n}', error: true },
      "fflush(stdout) makes sure \"before\" is printed before the crash. Always check a pointer that might be NULL before using it.",
      { tip: "&x is the address of x; *p is the value at the address in p." },
    ],
    [
      {
        exercise: "pointers",
        task: "Write void min_max(int a, int b, int *smaller, int *bigger) that stores the smaller of a and b in *smaller and the bigger in *bigger. In main, call it with 9 and 4 and print: smaller 4 bigger 9",
        expected: ["smaller 4 bigger 9"],
        hints: ["Inside: if (a < b) { *smaller = a; *bigger = b; } else { ... }", "In main: int s, b; min_max(9, 4, &s, &b);"],
        solution: '#include <stdio.h>\n\nvoid min_max(int a, int b, int *smaller, int *bigger)\n{\n    if (a < b) {\n        *smaller = a;\n        *bigger = b;\n    } else {\n        *smaller = b;\n        *bigger = a;\n    }\n}\n\nint main(void)\n{\n    int s, b;\n    min_max(9, 4, &s, &b);\n    printf("smaller %d bigger %d\\n", s, b);\n    return 0;\n}',
      },
    ],
  ],
  recapPt: [
    "&x é o endereço de x; int *p guarda o endereço de um int.",
    "*p é o valor nesse endereço: mudar *p muda a variável original.",
    "Passa endereços (&x) a uma função para ela mudar as tuas variáveis.",
    "NULL não aponta para nada; ler através dele faz o programa rebentar.",
  ],
  recap: [
    "&x is the address of x; int *p holds the address of an int.",
    "*p is the value at that address: changing *p changes the original variable.",
    "Pass addresses (&x) to a function to let it change your variables.",
    "NULL points nowhere; reading through it crashes the program.",
  ],
};
