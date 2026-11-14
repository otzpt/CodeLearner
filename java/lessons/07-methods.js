module.exports = {
  id: "methods",
  title: "Methods: your own instructions",
  titlePt: "Métodos: as tuas próprias instruções",
  screens: [
    [
      "A method is a block of code with a name, written inside the class next to main. For now, start each one with static. The type before the name is what it gives back with return; void means nothing:",
      { code: 'public class Main {\n    static int area(int width, int height) {\n        return width * height;\n    }\n\n    public static void main(String[] args) {\n        System.out.println(area(3, 4));\n        System.out.println(area(10, 2) + 1);\n    }\n}' },
      "Writing the method does not run it. Only area(3, 4) runs it. Lesson 8 explains what static means.",
    ],
    [
      "A method gets copies of numbers it is given, so changing a parameter does not change the caller's variable. An array or object is different: the method gets access to the same one, and can change what is inside:",
      { code: 'public class Main {\n    static void addOne(int n) {\n        n = n + 1;\n    }\n\n    static void addOneToFirst(int[] values) {\n        values[0] = values[0] + 1;\n    }\n\n    public static void main(String[] args) {\n        int x = 5;\n        int[] list = {5};\n        addOne(x);\n        addOneToFirst(list);\n        System.out.println(x + " " + list[0]);\n    }\n}' },
      { quiz: "After addOne(x) above, what is x?", choices: ["5", "6"], answer: 1, why: "addOne changed its own copy of the number." },
    ],
    [
      "Calling a method that returns a value without using the value is allowed, but the value is simply lost. Calling a void method where a value is needed is refused:",
      { code: 'public class Main {\n    static void greet() {\n        System.out.println("hi");\n    }\n\n    public static void main(String[] args) {\n        int x = greet();\n    }\n}', error: true },
      { tip: "return gives a value back; void gives nothing. Numbers are passed as copies." },
    ],
    [
      {
        exercise: "methods",
        task: "Write static boolean isEven(int n) and static int largest(int[] values). Print isEven(7), isEven(10), and largest of {4, 9, 2}, one per line.",
        expected: ["false", "true", "9"],
        hints: ["return n % 2 == 0;", "Start best at values[0]; for (int v : values) keep the bigger one."],
        solution: 'public class Main {\n    static boolean isEven(int n) {\n        return n % 2 == 0;\n    }\n\n    static int largest(int[] values) {\n        int best = values[0];\n        for (int v : values) {\n            if (v > best) {\n                best = v;\n            }\n        }\n        return best;\n    }\n\n    public static void main(String[] args) {\n        System.out.println(isEven(7));\n        System.out.println(isEven(10));\n        System.out.println(largest(new int[] {4, 9, 2}));\n    }\n}',
      },
    ],
  ],
  recapPt: [
    "static tipo nome(parâmetros) { ... } define um método na classe; nome(valores) corre-o.",
    "return devolve um valor; void não devolve nada.",
    "Números são passados como cópias; arrays e objetos são partilhados.",
  ],
  recap: [
    "static type name(params) { ... } defines a method in the class; name(values) runs it.",
    "return gives a value back; void gives nothing.",
    "Numbers are passed as copies; arrays and objects are shared.",
  ],
};
