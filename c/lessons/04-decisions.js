module.exports = {
  id: "decisions",
  title: "Making decisions with if",
  titlePt: "Tomar decisões com if",
  screens: [
    [
      "A comparison is true or false. In C, true is 1 and false is 0:",
      { code: '#include <stdio.h>\n\nint main(void)\n{\n    printf("%d\\n", 5 > 3);\n    printf("%d\\n", 4 == 4);\n    printf("%d\\n", 4 != 4);\n    return 0;\n}' },
      '== means "is equal to" and != means "is not equal to". A single = stores a value.',
    ],
    [
      "if runs a block only when its condition is true. The condition goes in ( ), the block in { }. else if checks another condition; else runs when none was true:",
      { code: '#include <stdio.h>\n\nint main(void)\n{\n    int score = 72;\n    if (score >= 90) {\n        printf("A\\n");\n    } else if (score >= 50) {\n        printf("pass\\n");\n    } else {\n        printf("fail\\n");\n    }\n    return 0;\n}' },
      "C checks from the top and runs only the first block whose condition is true.",
    ],
    [
      "&& means \"and\": both must be true. || means \"or\": at least one. ! means \"not\":",
      { code: '#include <stdio.h>\n\nint main(void)\n{\n    int age = 20;\n    if (age >= 18 && age < 65) {\n        printf("working age\\n");\n    }\n    printf("%d\\n", !(age > 18));\n    return 0;\n}' },
      { quiz: "What does printf(\"%d\", 3 > 1 && 2 > 5) print?", choices: ["1", "0"], answer: 2, why: "&& needs both: 2 > 5 is false, so the whole thing is 0." },
    ],
    [
      "A common slip: = instead of == inside if. It stores the value and then tests it. The compiler warns, and the program does the wrong thing:",
      { code: '#include <stdio.h>\n\nint main(void)\n{\n    int x = 3;\n    if (x = 0) {\n        printf("x is 0\\n");\n    }\n    printf("x is now %d\\n", x);\n    return 0;\n}', warning: true },
      "x = 0 stored 0 in x, and 0 counts as false, so the block was skipped. Nothing was compared.",
      { tip: "Inside if, compare with ==. A single = changes the variable." },
    ],
    [
      {
        exercise: "grade",
        task: 'Create int points = 64. Print "great" if points is 80 or more, "good" if it is 50 or more, and "try again" otherwise. Then print 1 if points is even and 0 if not.',
        expected: ["good", "1"],
        hints: ["if / else if / else, checking 80 first.", 'printf("%d\\n", points % 2 == 0);'],
        solution: '#include <stdio.h>\n\nint main(void)\n{\n    int points = 64;\n    if (points >= 80) {\n        printf("great\\n");\n    } else if (points >= 50) {\n        printf("good\\n");\n    } else {\n        printf("try again\\n");\n    }\n    printf("%d\\n", points % 2 == 0);\n    return 0;\n}',
      },
    ],
  ],
  recapPt: [
    "As comparações dão 1 (verdadeiro) ou 0 (falso): > < >= <= == !=",
    "if (...) { } else if (...) { } else { } corre só o primeiro bloco verdadeiro.",
    "&& é \"e\", || é \"ou\", ! é \"não\".",
    "= dentro de um if guarda em vez de comparar; o compilador avisa.",
  ],
  recap: [
    "Comparisons give 1 (true) or 0 (false): > < >= <= == !=",
    "if (...) { } else if (...) { } else { } runs only the first true block.",
    "&& is and, || is or, ! is not.",
    "= inside if stores instead of comparing; the compiler warns.",
  ],
};
