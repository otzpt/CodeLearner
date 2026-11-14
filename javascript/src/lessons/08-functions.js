module.exports = {
  id: "functions",
  title: "Functions: your own instructions",
  titlePt: "Funções: as tuas próprias instruções",
  screens: [
    [
      "A function is a block of code with a name. You write it once and run it whenever you want by writing its name and ( ):",
      { code: 'function greet() {\n  console.log("Hello!");\n}\n\ngreet();\ngreet();' },
      "Writing the function does not run it. Only greet() runs it, and it runs once for every time you write it.",
    ],
    [
      "A function can take values to work with, called parameters. They go in the ( ), separated by commas:",
      { code: 'function greet(name, mood) {\n  console.log(`Hello, ${name}! You look ${mood}.`);\n}\n\ngreet("Ana", "happy");\ngreet("Rui", "tired");' },
      "Inside the function, name and mood hold whatever was passed in that time, in the same order.",
    ],
    [
      "return sends a value back to where the function was called, so you can store it or use it in more code:",
      { code: "function double(n) {\n  return n * 2;\n}\n\nconst result = double(4);\nconsole.log(result);\nconsole.log(double(10) + 1);" },
      "console.log only shows a value on the screen. return hands it back to the program. A function without return gives back undefined, so storing its result gets you nothing useful.",
      { quiz: "You want to use a function's result in a sum later. What should the function do?", choices: ["console.log the result", "return the result"], answer: 2, why: "Only return gives the value back to your code." },
    ],
    [
      "There is a shorter way to write small functions, called an arrow function. These two are the same:",
      { code: "function triple(n) {\n  return n * 3;\n}\nconst tripleArrow = (n) => n * 3;\n\nconsole.log(triple(5), tripleArrow(5));" },
      "The arrow form is common for tiny functions passed to other functions, like the rule for sort() in lesson 7: (a, b) => a - b returns a negative number when a should come first.",
      { tip: "A function only runs when called with (). Use return when the caller needs the result." },
    ],
    [
      {
        exercise: "function",
        task: "Write a function area(width, height) that returns width times height. Print area(3, 4) and area(10, 2). Then write isEven(n) that returns true for even numbers, and print isEven(7).",
        expected: ["12", "20", "false"],
        hints: ["function area(width, height) { return width * height; }", "isEven returns n % 2 === 0."],
        solution: "function area(width, height) {\n  return width * height;\n}\nfunction isEven(n) {\n  return n % 2 === 0;\n}\nconsole.log(area(3, 4));\nconsole.log(area(10, 2));\nconsole.log(isEven(7));",
      },
    ],
  ],
  recapPt: [
    "function nome(parâmetros) { ... } define; nome(valores) corre-a.",
    "Os parâmetros são os valores que a função recebe de cada vez que é chamada.",
    "return devolve um valor; sem ele, a função devolve undefined.",
    "(n) => n * 3 é uma função seta curta.",
  ],
  recap: [
    "function name(params) { ... } defines; name(values) runs it.",
    "Parameters are the values the function receives each time it is called.",
    "return gives a value back; without it, the function gives undefined.",
    "(n) => n * 3 is a short arrow function.",
  ],
};
