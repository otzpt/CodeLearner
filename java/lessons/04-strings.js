module.exports = {
  id: "strings",
  title: "Text: String",
  titlePt: "Texto: String",
  screens: [
    [
      "A String holds text. + joins strings, .length() counts the characters, and .toUpperCase() gives a copy in capitals:",
      { code: 'public class Main {\n    public static void main(String[] args) {\n        String first = "Ana";\n        String full = first + " Silva";\n        System.out.println(full);\n        System.out.println(full.length());\n        System.out.println(full.toUpperCase());\n    }\n}' },
    ],
    [
      "Each character has a position, starting at 0. charAt(i) reads one, and substring(start, end) takes a piece, up to but not including end:",
      { code: 'public class Main {\n    public static void main(String[] args) {\n        String word = "Hello";\n        System.out.println(word.charAt(0));\n        System.out.println(word.charAt(word.length() - 1));\n        System.out.println(word.substring(1, 3));\n    }\n}' },
      { quiz: 'In "cat", what is charAt(1)?', choices: ["c", "a", "t"], answer: 2, why: "c is 0, a is 1, t is 2." },
    ],
    [
      "Here is the trap. == on two strings does not compare their text: it asks whether they are the very same object in memory. .equals() compares the text:",
      { code: 'public class Main {\n    public static void main(String[] args) {\n        String a = "cat";\n        String b = new String("cat");\n        System.out.println(a == b);\n        System.out.println(a.equals(b));\n    }\n}' },
      "Text typed by the user is always a new object, so == fails exactly when it matters. Always use .equals() for text.",
      { tip: "Compare text with .equals(), never ==. Positions start at 0." },
    ],
    [
      {
        exercise: "strings",
        task: 'Create String name = "Bruno". Print: Hello, Bruno! Your name has 5 letters. Then print its first and last letter separated by a space. Then print whether name equals "bruno" (lower case).',
        expected: ["Hello, Bruno! Your name has 5 letters.", "B o", "false"],
        hints: ['"Hello, " + name + "! Your name has " + name.length() + " letters."', "name.charAt(0) + \" \" + name.charAt(name.length() - 1), and name.equals(\"bruno\")."],
        solution: 'public class Main {\n    public static void main(String[] args) {\n        String name = "Bruno";\n        System.out.println("Hello, " + name + "! Your name has " + name.length() + " letters.");\n        System.out.println(name.charAt(0) + " " + name.charAt(name.length() - 1));\n        System.out.println(name.equals("bruno"));\n    }\n}',
      },
    ],
  ],
  recapPt: [
    "String guarda texto; + junta; .length() conta.",
    "As posições começam em 0: charAt(i), substring(início, fim).",
    "Compara texto com .equals(), nunca com ==.",
  ],
  recap: [
    "A String holds text; + joins; .length() counts.",
    "Positions start at 0: charAt(i), substring(start, end).",
    "Compare text with .equals(), never ==.",
  ],
};
