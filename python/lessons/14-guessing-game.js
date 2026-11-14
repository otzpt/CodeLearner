module.exports = {
  id: "guessing-game",
  title: "Project: a guessing game",
  titlePt: "Projeto: um jogo de adivinhar",
  screens: [
    [
      "Time to put everything together in one real program: a number guessing game.",
      "The program thinks of a secret number. The player guesses; the program says \"Too low\" or \"Too high\" until the guess is right, then says how many guesses it took.",
      "It uses almost every lesson: variables (3), if (5), a while loop (6) and input with int (11).",
    ],
    [
      "Plan it in plain words before writing code. Programmers do this all the time:",
      "1. Keep the secret number and a guess counter.\n2. Repeat until the guess is right:\n   - ask for a guess and turn it into a number\n   - add 1 to the counter\n   - say Too low or Too high\n3. Say Correct and the number of guesses.",
      "A while loop fits step 2, because you cannot know in advance how many guesses it will take. A common trick is while True, a loop that only ends when the code inside says break:",
      { code: "n = 0\nwhile True:\n    n += 1\n    if n == 3:\n        break\nprint(n)" },
    ],
    [
      {
        exercise: "game",
        task: 'Write the game with the secret number 42, so the checker knows the answer. Ask with "Guess: ". Print Too low, Too high, or at the end Correct! It took N guesses.',
        expected: ["Guess: {50}", "Too high", "Guess: {25}", "Too low", "Guess: {42}", "Correct! It took 3 guesses."],
        starter: "secret = 42\n# your code here",
        hints: [
          "guesses = 0, then while True:",
          'Inside the loop: guess = int(input("Guess: ")) and guesses += 1',
          "if guess < secret: Too low, elif guess > secret: Too high, else: print Correct and break.",
        ],
        solution: 'secret = 42\nguesses = 0\nwhile True:\n    guess = int(input("Guess: "))\n    guesses += 1\n    if guess < secret:\n        print("Too low")\n    elif guess > secret:\n        print("Too high")\n    else:\n        print(f"Correct! It took {guesses} guesses.")\n        break',
      },
    ],
    [
      "Once it passes, make it a real game: replace 42 with a random number from 1 to 100. random comes with Python; import makes it available:",
      { code: "import random\nsecret = random.randint(1, 100)\nprint(1 <= secret <= 100)" },
      "random.randint(1, 100) includes both ends. Run it with python3 and play it yourself. The checker cannot test this version, because it would not know the secret.",
    ],
  ],
  recapPt: [
    "Planeia primeiro em palavras simples, depois passa cada passo a código.",
    "while True: com break repete até o código decidir parar.",
    "import random e random.randint(1, 100) dão um número de 1 a 100.",
    "Escreveste um programa interativo completo.",
  ],
  recap: [
    "Plan in plain words first, then turn each step into code.",
    "while True: with break repeats until the code decides to stop.",
    "import random and random.randint(1, 100) give a number from 1 to 100.",
    "You wrote a complete interactive program.",
  ],
};
