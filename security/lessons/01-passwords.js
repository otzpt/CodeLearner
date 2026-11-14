module.exports = {
  id: "passwords",
  title: "Never store a password as it was typed",
  titlePt: "Nunca guardes uma password como foi escrita",
  screens: [
    [
      "Security is about what happens when someone uses your program in a way you did not plan. This course shows common mistakes by running them, then the fix. The examples are in Python; the Python course covers the basics if any line looks unfamiliar.",
      "Start with passwords. If a program saves passwords as typed and its database leaks, every account is open, and many people reuse passwords elsewhere.",
    ],
    [
      "A hash function turns any text into a fixed-length code. The same input always gives the same code, but there is no way to turn the code back into the input:",
      { code: 'import hashlib\n\nfor word in ["hunter2", "hunter2", "hunter3"]:\n    code = hashlib.sha256(word.encode()).hexdigest()\n    print(word, code[:16])' },
      ".encode() turns text into bytes, which is what hash functions take. [:16] shows only the start of each code. One changed letter gives a completely different code.",
    ],
    [
      "So a program can store the hash, and check a login by hashing what was typed and comparing:",
      { code: 'import hashlib\n\ndef h(text):\n    return hashlib.sha256(text.encode()).hexdigest()\n\nstored = h("correct horse")\nfor attempt in ["correct horse", "wrong guess"]:\n    print(attempt, "->", h(attempt) == stored)' },
      "The program never needs the real password again. Lesson 2 shows why plain sha256 is still not enough.",
      { quiz: "Can you get the password back from its hash?", choices: ["yes, by reversing it", "no, you can only hash guesses and compare"], answer: 2, why: "A hash only goes one way." },
    ],
    [
      {
        exercise: "hash",
        task: 'Write check(password, stored) that returns True when the sha256 hash of password equals stored. Set stored to the hash of "s3cret", then print check("s3cret", stored) and check("S3cret", stored).',
        expected: ["True", "False"],
        hints: ["hashlib.sha256(password.encode()).hexdigest()", "Capital S changes the hash completely."],
        solution: 'import hashlib\n\ndef check(password, stored):\n    return hashlib.sha256(password.encode()).hexdigest() == stored\n\nstored = hashlib.sha256("s3cret".encode()).hexdigest()\nprint(check("s3cret", stored))\nprint(check("S3cret", stored))',
      },
    ],
  ],
  recapPt: [
    "Nunca guardes passwords como foram escritas.",
    "Um hash transforma texto num código fixo e não se pode desfazer.",
    "Para verificar um login, faz o hash do que foi escrito e compara.",
  ],
  recap: [
    "Never store passwords as typed.",
    "A hash turns text into a fixed code and cannot be undone.",
    "To check a login, hash what was typed and compare.",
  ],
};
