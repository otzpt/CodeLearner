module.exports = {
  id: "guessing-game",
  title: "Project: a guessing game",
  titlePt: "Projeto: um jogo de adivinhar",
  screens: [
    [
      "Time to put everything together in one real program: a number guessing game.",
      "The program thinks of a secret number. The player guesses; the program says \"Too low\" or \"Too high\" until the guess is right, then says how many guesses it took.",
      "Plan it first:\n1. Keep the secret number and a guess counter.\n2. Repeat until the guess is right: ask, count, say low or high.\n3. Say Correct and the number of guesses.",
    ],
    [
      "You cannot know in advance how many guesses it takes, so loop with while (true) and leave with break. Stop as well when there is nothing more to read, or the loop would never end:",
      { code: 'public class Main {\n    public static void main(String[] args) {\n        int n = 0;\n        while (true) {\n            n++;\n            if (n == 3) {\n                break;\n            }\n        }\n        System.out.println(n);\n    }\n}' },
    ],
    [
      {
        exercise: "game",
        task: 'Write the game with the secret number 42, so the checker knows the answer. Ask with "Guess: ". Print Too low, Too high, or at the end Correct! It took N guesses.',
        expected: ["Guess: {50}", "Too high", "Guess: {25}", "Too low", "Guess: {42}", "Correct! It took 3 guesses."],
        hints: [
          "Scanner in = new Scanner(System.in); int secret = 42, guesses = 0; while (true) { ... }",
          'System.out.print("Guess: "); if (!in.hasNextInt()) break; int guess = in.nextInt(); guesses++;',
          "if (guess < secret) Too low, else if (guess > secret) Too high, else Correct and break.",
        ],
        solution: 'import java.util.Scanner;\n\npublic class Main {\n    public static void main(String[] args) {\n        Scanner in = new Scanner(System.in);\n        int secret = 42;\n        int guesses = 0;\n        while (true) {\n            System.out.print("Guess: ");\n            if (!in.hasNextInt()) {\n                break;\n            }\n            int guess = in.nextInt();\n            guesses++;\n            if (guess < secret) {\n                System.out.println("Too low");\n            } else if (guess > secret) {\n                System.out.println("Too high");\n            } else {\n                System.out.println("Correct! It took " + guesses + " guesses.");\n                break;\n            }\n        }\n    }\n}',
      },
    ],
    [
      "Once it passes, make it a real game with a random secret. java.util.Random's nextInt(100) gives 0 to 99, so + 1 makes it 1 to 100:",
      { code: 'import java.util.Random;\n\npublic class Main {\n    public static void main(String[] args) {\n        int secret = new Random().nextInt(100) + 1;\n        System.out.println(secret >= 1 && secret <= 100);\n    }\n}' },
      "Run it and play it yourself. The checker cannot test this version, because it would not know the secret.",
    ],
  ],
  recapPt: [
    "Planeia primeiro em palavras simples, depois passa cada passo a código.",
    "while (true) com break repete até o código decidir parar.",
    "in.hasNextInt() verifica se ainda há um número para ler.",
    "new Random().nextInt(100) + 1 dá 1 a 100.",
  ],
  recap: [
    "Plan in plain words first, then turn each step into code.",
    "while (true) with break repeats until the code decides to stop.",
    "in.hasNextInt() checks there is still a number to read.",
    "new Random().nextInt(100) + 1 gives 1 to 100.",
  ],
};
