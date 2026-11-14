module.exports = {
  id: "arrays-and-lists",
  title: "Arrays and lists",
  titlePt: "Arrays e listas",
  screens: [
    [
      "An array holds a fixed number of values of one type. Positions start at 0, and .length is how many there are:",
      { code: 'public class Main {\n    public static void main(String[] args) {\n        int[] scores = {15, 12, 18};\n        System.out.println(scores[0]);\n        System.out.println(scores[scores.length - 1]);\n        scores[1] = 14;\n        System.out.println(scores.length);\n    }\n}' },
    ],
    [
      "Java checks every position. Going past the end stops the program with a clear error instead of reading garbage:",
      { code: 'public class Main {\n    public static void main(String[] args) {\n        int[] scores = {15, 12, 18};\n        System.out.println(scores[3]);\n    }\n}', error: true },
      { quiz: "An array has 3 values. What is the last valid position?", choices: ["3", "2", "4"], answer: 2, why: "Positions go from 0 to length - 1." },
    ],
    [
      "An array cannot grow. An ArrayList can: add puts a value at the end, get(i) reads one, size() counts. The type goes in < >, and for (... : list) goes through every value:",
      { code: 'import java.util.ArrayList;\n\npublic class Main {\n    public static void main(String[] args) {\n        ArrayList<String> fruits = new ArrayList<>();\n        fruits.add("apple");\n        fruits.add("banana");\n        System.out.println(fruits.get(0) + " " + fruits.size());\n        for (String fruit : fruits) {\n            System.out.println("I like " + fruit);\n        }\n    }\n}' },
      "Read the for as \"for each fruit in fruits\". For numbers, the type in < > is Integer, not int.",
      { tip: "Positions start at 0. Arrays are fixed; an ArrayList grows with add." },
    ],
    [
      {
        exercise: "lists",
        task: 'Make an ArrayList<String> with "rice", "beans", "oil" and add "salt". Print how many items there are, then every item on its own line starting with "- ".',
        expected: ["4", "- rice", "- beans", "- oil", "- salt"],
        hints: ["import java.util.ArrayList; then new ArrayList<>() and four add calls.", 'for (String item : items) System.out.println("- " + item);'],
        solution: 'import java.util.ArrayList;\n\npublic class Main {\n    public static void main(String[] args) {\n        ArrayList<String> items = new ArrayList<>();\n        items.add("rice");\n        items.add("beans");\n        items.add("oil");\n        items.add("salt");\n        System.out.println(items.size());\n        for (String item : items) {\n            System.out.println("- " + item);\n        }\n    }\n}',
      },
    ],
  ],
  recapPt: [
    "int[] a = {1, 2, 3}; tem as posições 0 a a.length - 1.",
    "O Java verifica as posições e para com ArrayIndexOutOfBoundsException.",
    "ArrayList<String> cresce com add; get(i) lê; size() conta.",
    "for (String x : lista) percorre todos os valores.",
  ],
  recap: [
    "int[] a = {1, 2, 3}; has positions 0 to a.length - 1.",
    "Java checks positions and stops with ArrayIndexOutOfBoundsException.",
    "ArrayList<String> grows with add; get(i) reads; size() counts.",
    "for (String x : list) visits every value.",
  ],
};
