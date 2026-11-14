module.exports = {
  id: "input",
  title: "Reading what the user types",
  titlePt: "Ler o que o utilizador escreve",
  screens: [
    [
      "scanf reads what the user types. It takes a slot like printf's, and the variable to fill, with & in front of its name:",
      { code: '#include <stdio.h>\n\nint main(void)\n{\n    int age;\n    printf("Your age? ");\n    scanf("%d", &age);\n    printf("Next year you will be %d\\n", age + 1);\n    return 0;\n}', output: ["Your age? {20}", "Next year you will be 21"] },
      "The bold part is what was typed. & means \"the place where this variable lives\": scanf needs to know where to put the number. Lesson 9 explains it fully.",
    ],
    [
      "For a double, scanf uses %lf (the l stands for long), while printf uses %f. Several values can be read at once:",
      { code: '#include <stdio.h>\n\nint main(void)\n{\n    double a, b;\n    printf("Two numbers: ");\n    scanf("%lf %lf", &a, &b);\n    printf("%.2f\\n", a / b);\n    return 0;\n}', output: ["Two numbers: {7 2}", "3.50"] },
    ],
    [
      "scanf gives back how many values it managed to read. If the user types something that is not a number, it reads 0 values and the variable is left as it was:",
      { code: '#include <stdio.h>\n\nint main(void)\n{\n    int n = 0;\n    printf("Number: ");\n    int read = scanf("%d", &n);\n    printf("read %d value(s), n is %d\\n", read, n);\n    return 0;\n}', output: ["Number: {hello}", "read 0 value(s), n is 0"] },
      { quiz: "Why does scanf(\"%d\", &age) need the & ?", choices: ["it makes age a number", "it tells scanf where age lives, so it can fill it", "it is decoration"], answer: 2, why: "scanf must write into the variable, so it needs its place in memory." },
    ],
    [
      { tip: "scanf(\"%d\", &x) for int, scanf(\"%lf\", &x) for double. Do not forget the &." },
      {
        exercise: "ask",
        task: 'Ask "Width? " and "Height? " (whole numbers) and print: Area: W*H',
        expected: ["Width? {4}", "Height? {5}", "Area: 20"],
        hints: ["Two int variables, two printf + scanf pairs.", 'printf("Area: %d\\n", width * height);'],
        solution: '#include <stdio.h>\n\nint main(void)\n{\n    int width, height;\n    printf("Width? ");\n    scanf("%d", &width);\n    printf("Height? ");\n    scanf("%d", &height);\n    printf("Area: %d\\n", width * height);\n    return 0;\n}',
      },
    ],
  ],
  recapPt: [
    "scanf(\"%d\", &x); lê um int; scanf(\"%lf\", &x); lê um double.",
    "O & diz ao scanf onde a variável vive.",
    "scanf devolve quantos valores conseguiu ler; 0 quer dizer que o texto não era um número.",
  ],
  recap: [
    "scanf(\"%d\", &x); reads an int; scanf(\"%lf\", &x); reads a double.",
    "The & tells scanf where the variable lives.",
    "scanf gives back how many values it read; 0 means the text was not a number.",
  ],
};
