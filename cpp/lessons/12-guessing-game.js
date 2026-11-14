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
      "You cannot know in advance how many guesses it takes, so loop with while (true) and leave with break. Stop as well if reading a number fails, or the loop would never end:",
      { code: '#include <iostream>\n\nint main()\n{\n    int n = 0;\n    while (true) {\n        n++;\n        if (n == 3) {\n            break;\n        }\n    }\n    std::cout << n << "\\n";\n    return 0;\n}' },
    ],
    [
      {
        exercise: "game",
        task: 'Write the game with the secret number 42, so the checker knows the answer. Ask with "Guess: ". Print Too low, Too high, or at the end Correct! It took N guesses.',
        expected: ["Guess: {50}", "Too high", "Guess: {25}", "Too low", "Guess: {42}", "Correct! It took 3 guesses."],
        hints: [
          "int secret = 42, guess, guesses = 0; then while (true) { ... }",
          'Inside: std::cout << "Guess: "; if (!(std::cin >> guess)) break; guesses++;',
          "if (guess < secret) Too low, else if (guess > secret) Too high, else Correct and break.",
        ],
        solution: '#include <iostream>\n\nint main()\n{\n    int secret = 42;\n    int guess;\n    int guesses = 0;\n    while (true) {\n        std::cout << "Guess: ";\n        if (!(std::cin >> guess)) {\n            break;\n        }\n        guesses++;\n        if (guess < secret) {\n            std::cout << "Too low\\n";\n        } else if (guess > secret) {\n            std::cout << "Too high\\n";\n        } else {\n            std::cout << "Correct! It took " << guesses << " guesses.\\n";\n            break;\n        }\n    }\n    return 0;\n}',
      },
    ],
    [
      "Once it passes, make it a real game. <random> makes random numbers: a std::random_device seeds a generator, and a distribution picks from 1 to 100:",
      { code: '#include <iostream>\n#include <random>\n\nint main()\n{\n    std::random_device seed;\n    std::mt19937 generator(seed());\n    std::uniform_int_distribution<int> one_to_100(1, 100);\n    int secret = one_to_100(generator);\n    std::cout << (secret >= 1 && secret <= 100) << "\\n";\n    return 0;\n}' },
      "Build it and play it yourself. The checker cannot test this version, because it would not know the secret.",
    ],
  ],
  recapPt: [
    "Planeia primeiro em palavras simples, depois passa cada passo a código.",
    "while (true) com break repete até o código decidir parar.",
    "Testa a leitura: if (!(std::cin >> guess)) break;",
    "<random>: std::mt19937 e std::uniform_int_distribution dão números aleatórios.",
  ],
  recap: [
    "Plan in plain words first, then turn each step into code.",
    "while (true) with break repeats until the code decides to stop.",
    "Test the read: if (!(std::cin >> guess)) break;",
    "<random>: std::mt19937 and std::uniform_int_distribution give random numbers.",
  ],
};
