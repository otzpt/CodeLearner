module.exports = {
  id: "methods",
  title: "Methods: your own instructions",
  titlePt: "Métodos: as tuas próprias instruções",
  screens: [
    [
      "A method is a block of code with a name. The type before the name is what it gives back with return; void means nothing. Each parameter has a type:",
      { code: "int Area(int width, int height)\n{\n    return width * height;\n}\n\nConsole.WriteLine(Area(3, 4));\nConsole.WriteLine(Area(10, 2) + 1);" },
      "Writing the method does not run it. Only Area(3, 4) runs it. Method names start with a capital letter by custom.",
    ],
    [
      "A method gets copies of numbers it is given, so changing a parameter does not change the caller's variable. ref passes the variable itself instead:",
      { code: "void AddOne(int n)\n{\n    n = n + 1;\n}\n\nvoid AddOneRef(ref int n)\n{\n    n = n + 1;\n}\n\nint x = 5;\nAddOne(x);\nConsole.WriteLine(x);\nAddOneRef(ref x);\nConsole.WriteLine(x);" },
      "ref is written both in the method and at the call, so it is always visible that x may change.",
      { quiz: "After AddOne(x) above, what is x?", choices: ["5", "6"], answer: 1, why: "AddOne changed its own copy." },
    ],
    [
      "A method that returns a value can be written in one line with =>:",
      { code: "bool IsEven(int n) => n % 2 == 0;\n\nConsole.WriteLine(IsEven(7));\nConsole.WriteLine(IsEven(10));" },
      { tip: "return gives a value back; void gives nothing. Numbers are copies unless passed with ref." },
    ],
    [
      {
        exercise: "methods",
        task: "Write bool IsEven(int n) and int Largest(int[] values). Print IsEven(7), IsEven(10), and Largest of { 4, 9, 2 }, one per line.",
        expected: ["False", "True", "9"],
        hints: ["bool IsEven(int n) => n % 2 == 0;", "Start best at values[0]; foreach (int v in values) keep the bigger one."],
        solution: "bool IsEven(int n) => n % 2 == 0;\n\nint Largest(int[] values)\n{\n    int best = values[0];\n    foreach (int v in values)\n    {\n        if (v > best)\n        {\n            best = v;\n        }\n    }\n    return best;\n}\n\nConsole.WriteLine(IsEven(7));\nConsole.WriteLine(IsEven(10));\nConsole.WriteLine(Largest(new[] { 4, 9, 2 }));",
      },
    ],
  ],
  recapPt: [
    "tipo Nome(parâmetros) { ... return ...; } define um método; void não devolve nada.",
    "Números são passados como cópias; ref passa a própria variável.",
    "bool IsEven(int n) => n % 2 == 0; escreve um método numa linha.",
    "true e false aparecem como True e False quando mostrados.",
  ],
  recap: [
    "type Name(params) { ... return ...; } defines a method; void returns nothing.",
    "Numbers are passed as copies; ref passes the variable itself.",
    "bool IsEven(int n) => n % 2 == 0; writes a method in one line.",
    "true and false print as True and False.",
  ],
};
