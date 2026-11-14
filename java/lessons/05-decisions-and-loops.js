module.exports = {
  id: "decisions-and-loops",
  title: "Decisions and loops",
  titlePt: "Decisões e ciclos",
  screens: [
    [
      "if runs a block only when its condition is true; else if checks another; else runs when none was. == compares numbers, != means \"not equal\", && is \"and\", || is \"or\":",
      { code: 'public class Main {\n    public static void main(String[] args) {\n        int score = 72;\n        if (score >= 90) {\n            System.out.println("A");\n        } else if (score >= 50) {\n            System.out.println("pass");\n        } else {\n            System.out.println("fail");\n        }\n    }\n}' },
      "Java checks from the top and runs only the first block whose condition is true.",
    ],
    [
      "A condition must be true or false. = instead of == inside if is not a silent bug in Java: it is refused, because 0 is a number, not true or false:",
      { code: 'public class Main {\n    public static void main(String[] args) {\n        int x = 3;\n        if (x = 0) {\n            System.out.println("x is 0");\n        }\n    }\n}', error: true },
      "The type that holds true or false is called boolean: boolean done = false;",
    ],
    [
      "A for loop repeats a block a set number of times: start, the condition checked before each lap, and the step after each lap:",
      { code: 'public class Main {\n    public static void main(String[] args) {\n        int total = 0;\n        for (int i = 1; i <= 4; i++) {\n            total += i;\n        }\n        System.out.println(total);\n    }\n}' },
      "Mixing up < and <= runs one lap too many or too few. while (condition) repeats as long as it is true, and break leaves any loop.",
      { quiz: "How many times does for (int i = 0; i < 3; i++) run?", choices: ["2", "3", "4"], answer: 2, why: "i is 0, 1, 2." },
    ],
    [
      {
        exercise: "count",
        task: 'Print the numbers 1 to 5, one per line. Then print the sum of the numbers 1 to 100. Then print "even" or "odd" for 2026.',
        expected: ["1", "2", "3", "4", "5", "5050", "even"],
        hints: ["for (int i = 1; i <= 5; i++)", "2026 % 2 == 0 means even."],
        solution: 'public class Main {\n    public static void main(String[] args) {\n        for (int i = 1; i <= 5; i++) {\n            System.out.println(i);\n        }\n        int total = 0;\n        for (int i = 1; i <= 100; i++) {\n            total += i;\n        }\n        System.out.println(total);\n        if (2026 % 2 == 0) {\n            System.out.println("even");\n        } else {\n            System.out.println("odd");\n        }\n    }\n}',
      },
    ],
  ],
  recapPt: [
    "if / else if / else corre só o primeiro bloco verdadeiro.",
    "Uma condição tem de ser boolean (true ou false); = num if é recusado.",
    "for (int i = 1; i <= 3; i++) repete um número fixo de vezes.",
    "while repete enquanto a condição for verdadeira; break sai do ciclo.",
  ],
  recap: [
    "if / else if / else runs only the first true block.",
    "A condition must be a boolean (true or false); = inside if is refused.",
    "for (int i = 1; i <= 3; i++) repeats a set number of times.",
    "while repeats while the condition is true; break leaves the loop.",
  ],
};
