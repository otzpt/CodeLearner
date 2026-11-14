module.exports = {
  id: "guessing-game",
  title: "Project: a guessing game",
  titlePt: "Projeto: um jogo de adivinhar",
  screens: [
    [
      "Time to put everything together in one real program: a number guessing game.",
      "The program thinks of a secret number. The player guesses; the program says \"Too low\" or \"Too high\" until the guess is right, then says how many guesses it took.",
      "It uses variables (2), scanf (3), if (4) and a loop (5).",
    ],
    [
      "Plan it in plain words before writing code. Programmers do this all the time:",
      "1. Keep the secret number and a guess counter.\n2. Repeat until the guess is right:\n   - ask for a guess\n   - add 1 to the counter\n   - say Too low or Too high\n3. Say Correct and the number of guesses.",
      "A loop that only stops from inside fits step 2. while (1) repeats forever, and break leaves it. Stop too if scanf could not read a number, or a non-number would make the loop spin forever:",
      { code: '#include <stdio.h>\n\nint main(void)\n{\n    int n = 0;\n    while (1) {\n        n++;\n        if (n == 3) {\n            break;\n        }\n    }\n    printf("%d\\n", n);\n    return 0;\n}' },
    ],
    [
      {
        exercise: "game",
        task: 'Write the game with the secret number 42, so the checker knows the answer. Ask with "Guess: ". Print Too low, Too high, or at the end Correct! It took N guesses.',
        expected: ["Guess: {50}", "Too high", "Guess: {25}", "Too low", "Guess: {42}", "Correct! It took 3 guesses."],
        hints: [
          "int secret = 42, guess, guesses = 0; then while (1) { ... }",
          'Inside: printf("Guess: "); if (scanf("%d", &guess) != 1) break; guesses++;',
          "if (guess < secret) Too low, else if (guess > secret) Too high, else print Correct and break.",
        ],
        solution: '#include <stdio.h>\n\nint main(void)\n{\n    int secret = 42;\n    int guess;\n    int guesses = 0;\n    while (1) {\n        printf("Guess: ");\n        if (scanf("%d", &guess) != 1) {\n            break;\n        }\n        guesses++;\n        if (guess < secret) {\n            printf("Too low\\n");\n        } else if (guess > secret) {\n            printf("Too high\\n");\n        } else {\n            printf("Correct! It took %d guesses.\\n", guesses);\n            break;\n        }\n    }\n    return 0;\n}',
      },
    ],
    [
      "Once it passes, make it a real game with a random secret. rand() from stdlib.h gives a big random number; % 100 + 1 turns it into 1 to 100. srand(time(NULL)), with time.h, makes it different on every run:",
      { code: '#include <stdio.h>\n#include <stdlib.h>\n#include <time.h>\n\nint main(void)\n{\n    srand(time(NULL));\n    int secret = rand() % 100 + 1;\n    printf("%d\\n", secret >= 1 && secret <= 100);\n    return 0;\n}' },
      "Build it and play it yourself. The checker cannot test this version, because it would not know the secret.",
    ],
  ],
  recapPt: [
    "Planeia primeiro em palavras simples, depois passa cada passo a código.",
    "while (1) com break repete até o código decidir parar.",
    "Verifica o que o scanf devolve, para um texto não deixar o ciclo preso.",
    "srand(time(NULL)) uma vez, depois rand() % 100 + 1 dá 1 a 100.",
  ],
  recap: [
    "Plan in plain words first, then turn each step into code.",
    "while (1) with break repeats until the code decides to stop.",
    "Check what scanf gives back, so text cannot trap the loop.",
    "srand(time(NULL)) once, then rand() % 100 + 1 gives 1 to 100.",
  ],
};
