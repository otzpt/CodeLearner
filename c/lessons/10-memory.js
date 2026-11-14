module.exports = {
  id: "memory",
  title: "Memory you ask for: malloc and free",
  titlePt: "Memória que pedes: malloc e free",
  screens: [
    [
      "An array's size is fixed when you write the program. When you only learn the size while it runs, ask for memory with malloc, from stdlib.h. It gives back a pointer to that many bytes, or NULL if there is none:",
      { code: '#include <stdio.h>\n#include <stdlib.h>\n\nint main(void)\n{\n    int count = 5;\n    int *numbers = malloc(count * sizeof(int));\n    if (numbers == NULL) {\n        return 1;\n    }\n    for (int i = 0; i < count; i++) {\n        numbers[i] = i * i;\n    }\n    printf("%d\\n", numbers[4]);\n    free(numbers);\n    return 0;\n}' },
      "count * sizeof(int) is room for count ints. The pointer is then used exactly like an array.",
    ],
    [
      "Memory from malloc stays yours until you give it back with free. Every malloc needs exactly one free when you are done:",
      "- Forget free, and the memory is lost until the program ends (a memory leak).\n- free twice, or use the memory after free, and the program can crash or silently corrupt data.",
      "After free, the pointer still holds the old address but must not be used. Setting it to NULL makes a mistake easier to spot.",
      { quiz: "How many free calls does one malloc need?", choices: ["none", "exactly one", "one per value stored"], answer: 2, why: "One free gives back the whole block malloc handed out." },
    ],
    [
      "realloc makes a block bigger (or smaller), keeping what was in it. It may move the block, so use the pointer it gives back:",
      { code: '#include <stdio.h>\n#include <stdlib.h>\n\nint main(void)\n{\n    int *list = malloc(2 * sizeof(int));\n    if (list == NULL) return 1;\n    list[0] = 10;\n    list[1] = 20;\n    int *bigger = realloc(list, 3 * sizeof(int));\n    if (bigger == NULL) { free(list); return 1; }\n    list = bigger;\n    list[2] = 30;\n    printf("%d %d %d\\n", list[0], list[1], list[2]);\n    free(list);\n    return 0;\n}' },
      { tip: "Check malloc against NULL. One free per malloc. Never use memory after freeing it." },
    ],
    [
      {
        exercise: "memory",
        task: 'Ask "How many? " and read a count n. Use malloc for n ints, fill position i with (i + 1) * 10, print them all on one line separated by spaces, then free the memory.',
        expected: ["How many? {4}", "10 20 30 40"],
        hints: ["int *values = malloc(n * sizeof(int)); and check it is not NULL.", 'Print each with printf("%d", ...), a space between them (not after the last), then "\\n".'],
        solution: '#include <stdio.h>\n#include <stdlib.h>\n\nint main(void)\n{\n    int n;\n    printf("How many? ");\n    scanf("%d", &n);\n    int *values = malloc(n * sizeof(int));\n    if (values == NULL) {\n        return 1;\n    }\n    for (int i = 0; i < n; i++) {\n        values[i] = (i + 1) * 10;\n    }\n    for (int i = 0; i < n; i++) {\n        if (i > 0) {\n            printf(" ");\n        }\n        printf("%d", values[i]);\n    }\n    printf("\\n");\n    free(values);\n    return 0;\n}',
      },
    ],
  ],
  recapPt: [
    "malloc(n * sizeof(int)) pede espaço para n ints; verifica se deu NULL.",
    "O ponteiro usa-se como um array.",
    "Cada malloc precisa de exatamente um free; sem ele há uma fuga de memória.",
    "Nunca uses memória depois do free. realloc muda o tamanho e pode mudar o endereço.",
  ],
  recap: [
    "malloc(n * sizeof(int)) asks for room for n ints; check for NULL.",
    "The pointer is used like an array.",
    "Every malloc needs exactly one free; without it, memory leaks.",
    "Never use memory after free. realloc resizes and may move the block.",
  ],
};
