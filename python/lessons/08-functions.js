module.exports = {
  id: "functions",
  title: "Functions: your own instructions",
  titlePt: "Funções: as tuas próprias instruções",
  screens: [
    [
      "A function is a block of code with a name. You define it once with def and run it whenever you want by writing its name and ( ):",
      { code: 'def greet():\n    print("Hello!")\n\ngreet()\ngreet()' },
      "Defining the function does not run it. Only greet() runs it, once for every time you write it.",
    ],
    [
      "A function can take values to work with, called parameters. They go in the ( ), separated by commas:",
      { code: 'def greet(name, mood):\n    print(f"Hello, {name}! You look {mood}.")\n\ngreet("Ana", "happy")\ngreet("Rui", "tired")' },
      "Inside the function, name and mood hold whatever was passed in that time, in the same order.",
    ],
    [
      "return sends a value back to where the function was called, so you can store it or use it in more code:",
      { code: "def double(n):\n    return n * 2\n\nresult = double(4)\nprint(result)\nprint(double(10) + 1)" },
      "print only shows a value on the screen. return hands it back to the program. A function without return gives back None, Python's word for \"nothing\".",
      { quiz: "You want to use a function's result in a sum later. What should the function do?", choices: ["print the result", "return the result"], answer: 2, why: "Only return gives the value back to your code." },
    ],
    [
      "A parameter can have a default value, used when the caller leaves it out:",
      { code: 'def power(base, exponent=2):\n    return base ** exponent\n\nprint(power(3))\nprint(power(3, 3))' },
      { tip: "A function only runs when called with (). Use return when the caller needs the result." },
    ],
    [
      {
        exercise: "function",
        task: "Write a function area(width, height) that returns width times height. Print area(3, 4) and area(10, 2). Then write is_even(n) that returns True for even numbers, and print is_even(7).",
        expected: ["12", "20", "False"],
        hints: ["def area(width, height):\n    return width * height", "is_even returns n % 2 == 0."],
        solution: "def area(width, height):\n    return width * height\n\ndef is_even(n):\n    return n % 2 == 0\n\nprint(area(3, 4))\nprint(area(10, 2))\nprint(is_even(7))",
      },
    ],
  ],
  recapPt: [
    "def nome(parâmetros): define uma função; nome(valores) corre-a.",
    "Os parâmetros são os valores que a função recebe em cada chamada.",
    "return devolve um valor; sem ele, a função devolve None.",
    "Um parâmetro pode ter um valor por omissão: def power(base, exponent=2).",
  ],
  recap: [
    "def name(params): defines a function; name(values) runs it.",
    "Parameters are the values the function receives each time it is called.",
    "return gives a value back; without it, the function gives None.",
    "A parameter can have a default: def power(base, exponent=2).",
  ],
};
