module.exports = {
  id: "loops",
  title: "Repeating with loops",
  titlePt: "Repetir com ciclos",
  screens: [
    [
      "A loop repeats a block of code. The for loop is used when you know how many times:",
      { code: 'for (let i = 1; i <= 3; i++) {\n  console.log("lap " + i);\n}' },
      "The three parts in ( ), separated by semicolons:",
      "1. let i = 1  creates a counter, once, at the start.\n2. i <= 3  is checked before every lap; the loop stops when it is false.\n3. i++  runs after every lap; it adds 1 to i.",
    ],
    [
      "Change the parts and you change the count. Starting at 0 with < instead of <= is the most common way to write it:",
      { code: "for (let i = 0; i < 3; i++) {\n  console.log(i);\n}" },
      "It still runs 3 times, but counts 0, 1, 2. Mixing up < and <= is the most common loop mistake: it runs one time too many or too few.",
      { quiz: "How many times does for (let i = 1; i < 5; i++) run?", choices: ["4", "5", "6"], answer: 1, why: "i is 1, 2, 3, 4. At 5, i < 5 is false and the loop stops." },
    ],
    [
      "A loop often builds up a result. Start a variable before the loop and change it inside:",
      { code: "let total = 0;\nfor (let i = 1; i <= 4; i++) {\n  total = total + i;\n}\nconsole.log(total);" },
      "total goes 0, 1, 3, 6, 10. The console.log is after the loop's closing }, so it prints only once, at the end.",
    ],
    [
      "while repeats as long as a condition is true. Use it when you do not know the count in advance:",
      { code: "let money = 10;\nwhile (money > 0) {\n  console.log(`I have ${money}`);\n  money = money - 4;\n}" },
      "If the condition never becomes false, the loop never stops. If your program seems frozen in a terminal, Ctrl+C stops it.",
      { tip: "Something inside a while loop must change the condition, or it runs forever." },
    ],
    [
      {
        exercise: "count",
        task: "Print the numbers 1 to 5, one per line. Then print the sum of the numbers 1 to 100.",
        expected: ["1", "2", "3", "4", "5", "5050"],
        hints: ["The first part is a for loop from 1 while i <= 5.", "For the sum: let total = 0 before a second loop, total = total + i inside, console.log(total) after it."],
        solution: "for (let i = 1; i <= 5; i++) {\n  console.log(i);\n}\nlet total = 0;\nfor (let i = 1; i <= 100; i++) {\n  total = total + i;\n}\nconsole.log(total);",
      },
    ],
  ],
  recapPt: [
    "for (let i = 1; i <= 3; i++) { ... } repete um número fixo de vezes.",
    "< ou <= decide se o último número entra.",
    "Constrói um resultado com uma variável criada antes do ciclo.",
    "while (condição) { ... } repete até a condição ser falsa. Ctrl+C para um programa preso.",
  ],
  recap: [
    "for (let i = 1; i <= 3; i++) { ... } repeats a set number of times.",
    "< or <= decides whether the last number is included.",
    "Build a result with a variable made before the loop.",
    "while (condition) { ... } repeats until the condition is false. Ctrl+C stops a stuck program.",
  ],
};
