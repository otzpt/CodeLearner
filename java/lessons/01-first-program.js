module.exports = {
  id: "first-program",
  title: "Your first program",
  titlePt: "O teu primeiro programa",
  screens: [
    [
      "A program is a list of instructions for the computer, written in a text file. Java is one language for writing them.",
      "The java command checks your file, translates it, and runs it, in one step:",
      { code: "java Hello.java", run: false },
      "Every example in this course really runs while you read it. What you see under \"Output\" is what it printed just now.",
    ],
    [
      "Here is a whole Java program. Every Java program has this shape:",
      { code: 'public class Main {\n    public static void main(String[] args) {\n        System.out.println("Hello!");\n    }\n}' },
      "Java keeps all code inside a class, here called Main. main is where the program starts; its first line is always written exactly like that. Everything between its { and } is its body.",
    ],
    [
      "System.out.println prints a line. Text goes in double quotes; numbers do not, and a sum is worked out first. print (without ln) does not end the line:",
      { code: 'public class Main {\n    public static void main(String[] args) {\n        System.out.print("two plus three is ");\n        System.out.println(2 + 3);\n        System.out.println("2 + 3");\n    }\n}' },
      "In quotes, 2 + 3 is just text. // starts a comment that Java ignores.",
      { quiz: 'What does System.out.println("10 + 5"); show?', choices: ["15", "10 + 5", "an error"], answer: 2, why: "In quotes, it is text, shown exactly as written." },
    ],
    [
      "Every statement ends with a semicolon. Without it, Java refuses before running anything:",
      { code: 'public class Main {\n    public static void main(String[] args) {\n        System.out.println("hi")\n    }\n}', error: true },
      "Capitals matter too: system.out.println, with a small s, is a different name that does not exist. Reading error messages is a normal part of programming.",
      { tip: "Every statement ends with ;. Text in double quotes. Names spelled exactly, capitals included." },
    ],
    [
      {
        exercise: "hello",
        task: "Write a program that prints your name on one line and the number 2026 on the next. For the checker, use the name Ana.",
        expected: ["Ana", "2026"],
        hints: ["The file already has the class and main. Write inside main.", 'System.out.println("Ana");'],
        solution: 'public class Main {\n    public static void main(String[] args) {\n        System.out.println("Ana");\n        System.out.println(2026);\n    }\n}',
      },
    ],
  ],
  recapPt: [
    "java Ficheiro.java verifica, traduz e corre o programa.",
    "O código vive numa classe; o programa começa em public static void main(String[] args).",
    "System.out.println(...) mostra uma linha; print não muda de linha.",
    "Cada instrução acaba em ;. As maiúsculas contam.",
  ],
  recap: [
    "java File.java checks, translates and runs the program.",
    "Code lives in a class; the program starts at public static void main(String[] args).",
    "System.out.println(...) prints a line; print does not end the line.",
    "Every statement ends with ;. Capitals count.",
  ],
};
