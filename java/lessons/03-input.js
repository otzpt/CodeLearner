module.exports = {
  id: "input",
  title: "Reading what the user types",
  titlePt: "Ler o que o utilizador escreve",
  screens: [
    [
      "A Scanner reads from the keyboard. import java.util.Scanner; at the top makes it available. nextInt() reads a whole number, nextLine() a whole line of text:",
      { code: 'import java.util.Scanner;\n\npublic class Main {\n    public static void main(String[] args) {\n        Scanner in = new Scanner(System.in);\n        System.out.print("Your age? ");\n        int age = in.nextInt();\n        System.out.println("Next year you will be " + (age + 1));\n    }\n}', output: ["Your age? {20}", "Next year you will be 21"] },
      "The bold part is what was typed. + joins text and a number into text; the brackets around age + 1 make the sum happen first.",
    ],
    [
      "Without those brackets, + works left to right: text + 20 is the text \"...20\", and then + 1 adds the character 1:",
      { code: 'public class Main {\n    public static void main(String[] args) {\n        int age = 20;\n        System.out.println("Next year: " + age + 1);\n        System.out.println("Next year: " + (age + 1));\n    }\n}' },
      { quiz: 'What does System.out.println("x" + 2 + 3); print?', choices: ["x5", "x23"], answer: 2, why: '"x" + 2 is "x2", then + 3 joins again: "x23".' },
    ],
    [
      "If the user types something that is not a number, nextInt() stops the program:",
      { code: 'import java.util.Scanner;\n\npublic class Main {\n    public static void main(String[] args) {\n        Scanner in = new Scanner(System.in);\n        System.out.print("Number: ");\n        int n = in.nextInt();\n        System.out.println(n);\n    }\n}', output: ["Number: {hello}"], error: true },
      "in.hasNextInt() asks first, without reading: true if the next thing typed is a whole number. Lesson 10 shows how to catch errors instead.",
      { tip: "Scanner in = new Scanner(System.in); then nextInt(), nextDouble() or nextLine()." },
    ],
    [
      {
        exercise: "ask",
        task: 'Ask "Width? " and "Height? " (whole numbers) and print: Area: W*H',
        expected: ["Width? {4}", "Height? {5}", "Area: 20"],
        hints: ["Make one Scanner, then print a prompt and call nextInt() for each.", 'System.out.println("Area: " + (width * height));'],
        solution: 'import java.util.Scanner;\n\npublic class Main {\n    public static void main(String[] args) {\n        Scanner in = new Scanner(System.in);\n        System.out.print("Width? ");\n        int width = in.nextInt();\n        System.out.print("Height? ");\n        int height = in.nextInt();\n        System.out.println("Area: " + (width * height));\n    }\n}',
      },
    ],
  ],
  recapPt: [
    "Scanner in = new Scanner(System.in); lê do teclado (import java.util.Scanner;).",
    "nextInt() lê um número inteiro, nextLine() uma linha de texto.",
    "\"texto\" + número junta tudo como texto: usa parênteses para a soma acontecer primeiro.",
    "Texto que não é número faz o nextInt() parar o programa.",
  ],
  recap: [
    "Scanner in = new Scanner(System.in); reads the keyboard (import java.util.Scanner;).",
    "nextInt() reads a whole number, nextLine() a line of text.",
    "\"text\" + number joins as text: use brackets to make a sum happen first.",
    "Text that is not a number makes nextInt() stop the program.",
  ],
};
