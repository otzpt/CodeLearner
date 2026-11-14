module.exports = {
  id: "strings",
  title: "Text: strings",
  titlePt: "Texto: strings",
  screens: [
    [
      "C has no separate type for text. A string is an array of char, one character each, with a hidden extra character '\\0' at the end that marks where the text stops:",
      { code: '#include <stdio.h>\n\nint main(void)\n{\n    char name[] = "Ana";\n    printf("%s\\n", name);\n    printf("%c\\n", name[0]);\n    printf("%zu bytes\\n", sizeof name);\n    return 0;\n}' },
      "%s prints a string, %c one character. \"Ana\" takes 4 bytes: A, n, a and the '\\0'.",
    ],
    [
      "string.h has the tools for strings. strlen counts the characters before the '\\0':",
      { code: '#include <stdio.h>\n#include <string.h>\n\nint main(void)\n{\n    char word[] = "Hello";\n    printf("%zu\\n", strlen(word));\n    printf("%c\\n", word[strlen(word) - 1]);\n    return 0;\n}' },
      "%zu is the slot for sizes, which is what strlen and sizeof give back.",
    ],
    [
      "== does not compare the text of two strings: it compares where they are in memory. strcmp compares the text, and gives 0 when it is the same:",
      { code: '#include <stdio.h>\n#include <string.h>\n\nint main(void)\n{\n    char a[] = "cat";\n    char b[] = "cat";\n    printf("%d\\n", a == b);\n    printf("%d\\n", strcmp(a, b) == 0);\n    return 0;\n}', warning: true },
      { quiz: "How do you check that two strings have the same text?", choices: ["a == b", "strcmp(a, b) == 0"], answer: 2, why: "== compares places in memory; strcmp compares the characters." },
    ],
    [
      "To read a word typed by the user, scanf with %s reads up to the first space. Give it the size limit, one less than the array, so a long word cannot overflow it. An array name needs no &:",
      { code: '#include <stdio.h>\n\nint main(void)\n{\n    char name[20];\n    printf("Name? ");\n    scanf("%19s", name);\n    printf("Hello, %s!\\n", name);\n    return 0;\n}', output: ["Name? {Ana}", "Hello, Ana!"] },
      { tip: "A string is char[] ending in '\\0'. Compare with strcmp, never ==." },
    ],
    [
      {
        exercise: "strings",
        task: 'Ask "Word? " and read one word (up to 49 characters). Print how many letters it has, then print "palindrome" if it reads the same backwards, or "not a palindrome".',
        expected: ["Word? {level}", "5 letters", "palindrome"],
        hints: ["char word[50]; scanf(\"%49s\", word); then strlen(word).", "Compare word[i] with word[length - 1 - i] for each i in the first half."],
        solution: '#include <stdio.h>\n#include <string.h>\n\nint main(void)\n{\n    char word[50];\n    printf("Word? ");\n    scanf("%49s", word);\n    int length = strlen(word);\n    printf("%d letters\\n", length);\n    int same = 1;\n    for (int i = 0; i < length / 2; i++) {\n        if (word[i] != word[length - 1 - i]) {\n            same = 0;\n        }\n    }\n    if (same) {\n        printf("palindrome\\n");\n    } else {\n        printf("not a palindrome\\n");\n    }\n    return 0;\n}',
      },
    ],
  ],
  recapPt: [
    "Uma string é um array de char que acaba em '\\0'.",
    "%s mostra uma string, %c um carácter; strlen conta os caracteres.",
    "Compara texto com strcmp(a, b) == 0, nunca com ==.",
    "scanf(\"%19s\", name) lê uma palavra com limite de tamanho; um array não leva &.",
  ],
  recap: [
    "A string is a char array ending in '\\0'.",
    "%s prints a string, %c one character; strlen counts the characters.",
    "Compare text with strcmp(a, b) == 0, never ==.",
    "scanf(\"%19s\", name) reads one word with a size limit; an array needs no &.",
  ],
};
