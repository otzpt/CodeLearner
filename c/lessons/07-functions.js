module.exports = {
  id: "functions",
  title: "Functions: your own instructions",
  titlePt: "Funções: as tuas próprias instruções",
  screens: [
    [
      "A function is a block of code with a name. main is one. You write your own above main, and run it by writing its name and ( ). void means it gives nothing back:",
      { code: '#include <stdio.h>\n\nvoid greet(void)\n{\n    printf("Hello!\\n");\n}\n\nint main(void)\n{\n    greet();\n    greet();\n    return 0;\n}' },
      "Writing the function does not run it. Only greet() runs it, once for every time you write it.",
    ],
    [
      "A function can take values, called parameters, each with its type. return sends a value back, and the type before the name says which type:",
      { code: '#include <stdio.h>\n\nint area(int width, int height)\n{\n    return width * height;\n}\n\nint main(void)\n{\n    int a = area(3, 4);\n    printf("%d\\n", a);\n    printf("%d\\n", area(10, 2) + 1);\n    return 0;\n}' },
    ],
    [
      "A function gets copies of the values it is given. Changing a parameter inside does not change the caller's variable:",
      { code: '#include <stdio.h>\n\nvoid add_one(int n)\n{\n    n = n + 1;\n    printf("inside: %d\\n", n);\n}\n\nint main(void)\n{\n    int x = 5;\n    add_one(x);\n    printf("outside: %d\\n", x);\n    return 0;\n}' },
      "To let a function change the caller's variable, you give it the variable's place in memory. That is what scanf's & does, and lesson 9 shows how.",
      { quiz: "After add_one(x) above, what is x in main?", choices: ["5", "6"], answer: 1, why: "add_one changed its own copy, not x." },
    ],
    [
      "A function must be known before it is used. If you want to write it below main, put a one-line declaration (its first line plus ;) above main:",
      { code: '#include <stdio.h>\n\nint twice(int n);\n\nint main(void)\n{\n    printf("%d\\n", twice(21));\n    return 0;\n}\n\nint twice(int n)\n{\n    return n * 2;\n}' },
      { tip: "The type before the name is what it returns; void means nothing. Parameters are copies." },
    ],
    [
      {
        exercise: "functions",
        task: "Write int is_even(int n) that returns 1 for even numbers and 0 otherwise, and int largest(int a, int b) that returns the bigger one. Print is_even(7), is_even(10) and largest(3, 9), one per line.",
        expected: ["0", "1", "9"],
        hints: ["return n % 2 == 0;", "if (a > b) return a; return b;"],
        solution: '#include <stdio.h>\n\nint is_even(int n)\n{\n    return n % 2 == 0;\n}\n\nint largest(int a, int b)\n{\n    if (a > b) {\n        return a;\n    }\n    return b;\n}\n\nint main(void)\n{\n    printf("%d\\n", is_even(7));\n    printf("%d\\n", is_even(10));\n    printf("%d\\n", largest(3, 9));\n    return 0;\n}',
      },
    ],
  ],
  recapPt: [
    "tipo nome(parâmetros) { ... } define uma função; nome(valores) corre-a.",
    "return devolve um valor do tipo indicado; void quer dizer que não devolve nada.",
    "Os parâmetros são cópias: mudá-los não muda as variáveis de quem chama.",
    "Uma função tem de ser conhecida antes de ser usada: escreve-a acima ou declara-a.",
  ],
  recap: [
    "type name(params) { ... } defines a function; name(values) runs it.",
    "return gives back a value of the stated type; void means nothing.",
    "Parameters are copies: changing them does not change the caller's variables.",
    "A function must be known before it is used: write it above, or declare it.",
  ],
};
