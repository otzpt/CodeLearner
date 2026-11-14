module.exports = {
  id: "functions",
  title: "Functions: your own instructions",
  screens: [
    [
      "A function is named code. It only runs when called with ( ):",
      { code: 'function greet(name) {\n  console.log(`Hello, ${name}!`);\n}\ngreet("Ana");\ngreet("Rui");' },
      "name is a parameter: it holds whatever was passed in that call.",
    ],
    [
      "return hands a value back to the caller:",
      { code: "function double(n) {\n  return n * 2;\n}\nconsole.log(double(4) + 1);" },
      "Without return, a function gives back undefined:",
      { code: "function showDouble(n) {\n  console.log(n * 2);\n}\nconsole.log(showDouble(4));" },
      { quiz: "You need the result in a sum later. The function should:", choices: ["console.log it", "return it"], answer: 2, why: "Only return gives the value back." },
    ],
    [
      "Small functions are often written as arrows. These two are the same:",
      { code: "function triple(n) {\n  return n * 3;\n}\nconst triple2 = (n) => n * 3;\nconsole.log(triple(5), triple2(5));" },
      "So in sort((a, b) => a - b), the arrow is a function telling sort which comes first.",
    ],
    [
      {
        exercise: "function",
        task: "Write area(width, height) returning width times height, and isEven(n) returning true for even numbers. Print area(3, 4), area(10, 2) and isEven(7).",
        expected: ["12", "20", "false"],
        hints: ["return width * height;", "return n % 2 === 0;"],
        solution: "function area(width, height) {\n  return width * height;\n}\nfunction isEven(n) {\n  return n % 2 === 0;\n}\nconsole.log(area(3, 4));\nconsole.log(area(10, 2));\nconsole.log(isEven(7));",
      },
    ],
  ],
  recap: [
    "function name(params) { ... } defines; name(values) runs it.",
    "return gives a value back; without it you get undefined.",
    "(n) => n * 3 is a short arrow function.",
  ],
};
