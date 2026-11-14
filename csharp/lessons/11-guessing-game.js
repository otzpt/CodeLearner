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
      "You cannot know in advance how many guesses it takes, so loop with while (true) and leave with break. ReadLine gives null when there is nothing more to read; stop then too, or the loop would never end:",
      { code: "int n = 0;\nwhile (true)\n{\n    n++;\n    if (n == 3)\n    {\n        break;\n    }\n}\nConsole.WriteLine(n);" },
    ],
    [
      {
        exercise: "game",
        task: 'Write the game with the secret number 42, so the checker knows the answer. Ask with "Guess: ". Print Too low, Too high, or at the end Correct! It took N guesses.',
        expected: ["Guess: {50}", "Too high", "Guess: {25}", "Too low", "Guess: {42}", "Correct! It took 3 guesses."],
        starter: "int secret = 42;\n// your code here",
        hints: [
          "int guesses = 0; while (true) { ... }",
          'Console.Write("Guess: "); string? text = Console.ReadLine(); if (text == null) break; int guess = int.Parse(text); guesses++;',
          "if (guess < secret) Too low, else if (guess > secret) Too high, else Correct and break.",
        ],
        solution: 'int secret = 42;\nint guesses = 0;\nwhile (true)\n{\n    Console.Write("Guess: ");\n    string? text = Console.ReadLine();\n    if (text == null)\n    {\n        break;\n    }\n    int guess = int.Parse(text);\n    guesses++;\n    if (guess < secret)\n    {\n        Console.WriteLine("Too low");\n    }\n    else if (guess > secret)\n    {\n        Console.WriteLine("Too high");\n    }\n    else\n    {\n        Console.WriteLine($"Correct! It took {guesses} guesses.");\n        break;\n    }\n}',
      },
    ],
    [
      "Once it passes, make it a real game with a random secret. Random.Shared.Next(1, 101) gives 1 to 100: the second number is not included:",
      { code: "int secret = Random.Shared.Next(1, 101);\nConsole.WriteLine(secret >= 1 && secret <= 100);" },
      "Run it and play it yourself. The checker cannot test this version, because it would not know the secret.",
    ],
  ],
  recapPt: [
    "Planeia primeiro em palavras simples, depois passa cada passo a código.",
    "while (true) com break repete até o código decidir parar.",
    "Console.ReadLine() dá null quando não há mais nada para ler.",
    "Random.Shared.Next(1, 101) dá 1 a 100.",
  ],
  recap: [
    "Plan in plain words first, then turn each step into code.",
    "while (true) with break repeats until the code decides to stop.",
    "Console.ReadLine() gives null when there is nothing more to read.",
    "Random.Shared.Next(1, 101) gives 1 to 100.",
  ],
};
