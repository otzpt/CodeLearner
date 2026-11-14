module.exports = {
  id: "randomness",
  title: "Random values attackers cannot guess",
  titlePt: "Valores aleatórios que ninguém adivinha",
  screens: [
    [
      "The random module makes numbers that look random but come from a formula. Anyone who learns its starting point, the seed, gets the exact same sequence:",
      { code: 'import random\n\nrandom.seed(42)\nprint([random.randint(0, 9) for _ in range(5)])\nrandom.seed(42)\nprint([random.randint(0, 9) for _ in range(5)])' },
      "That is fine for a game. It is not fine for a password reset code, a session id or anything an attacker would like to guess.",
    ],
    [
      "The secrets module uses the operating system's unpredictable source. token_hex makes a random code, and secrets.choice picks from a list:",
      { code: 'import secrets, string\n\ntoken = secrets.token_hex(16)\nprint(len(token), "characters")\nletters = string.ascii_letters + string.digits\npassword = "".join(secrets.choice(letters) for _ in range(12))\nprint(len(password), "characters")' },
      "The values change on every run, so the example prints only their lengths.",
      { quiz: "Which is right for a password reset code?", choices: ["random.randint", "secrets.token_hex"], answer: 2, why: "secrets cannot be predicted from earlier values." },
    ],
    [
      {
        exercise: "randomness",
        task: "Write make_code() that returns a 6-digit code as a string, each digit chosen with secrets.choice from \"0123456789\". Print the length of one code, and whether it is all digits.",
        expected: ["6", "True"],
        hints: ['"".join(secrets.choice("0123456789") for _ in range(6))', "code.isdigit() is True when every character is a digit."],
        solution: 'import secrets\n\ndef make_code():\n    return "".join(secrets.choice("0123456789") for _ in range(6))\n\ncode = make_code()\nprint(len(code))\nprint(code.isdigit())',
      },
    ],
  ],
  recapPt: [
    "O módulo random é previsível: a mesma seed dá a mesma sequência.",
    "Para segredos (códigos, tokens, passwords) usa o módulo secrets.",
    "secrets.token_hex(n) e secrets.choice(lista) são imprevisíveis.",
  ],
  recap: [
    "The random module is predictable: the same seed gives the same sequence.",
    "For secrets (codes, tokens, passwords) use the secrets module.",
    "secrets.token_hex(n) and secrets.choice(list) cannot be predicted.",
  ],
};
