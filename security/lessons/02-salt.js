module.exports = {
  id: "salt",
  title: "Salt and slow hashing",
  titlePt: "Sal e hash lento",
  screens: [
    [
      "Plain sha256 has two problems. Same password, same hash: an attacker who cracks one sees everyone else who used it. And it is fast: a graphics card can try billions of guesses a second.",
      "A salt fixes the first: a random value stored next to each hash and mixed into it, so equal passwords give different hashes:",
      { code: 'import hashlib, os\n\nfor user in ["ana", "rui"]:\n    salt = os.urandom(16)\n    code = hashlib.sha256(salt + b"password123").hexdigest()\n    print(user, code[:16])' },
      'os.urandom(16) is 16 random bytes. b"..." is text written directly as bytes.',
    ],
    [
      "A slow hash fixes the second. hashlib.scrypt is built to cost time and memory, so each guess costs the attacker too. The salt is stored with the result:",
      { code: 'import hashlib, os\n\ndef hash_password(password, salt):\n    return hashlib.scrypt(password.encode(), salt=salt, n=2**14, r=8, p=1)\n\nsalt = os.urandom(16)\nstored = hash_password("password123", salt)\nprint(len(stored), "bytes")\nprint(hash_password("password123", salt) == stored)' },
      "n, r and p set how much work each hash takes.",
    ],
    [
      "Compare secrets with hmac.compare_digest instead of ==. == stops at the first different byte, so how long it takes can hint at how much was right. compare_digest always takes the same time:",
      { code: 'import hmac\n\nprint(hmac.compare_digest(b"abc", b"abc"))\nprint(hmac.compare_digest(b"abc", b"abd"))' },
      { quiz: "Why add a salt?", choices: ["to make the hash shorter", "so equal passwords do not give equal hashes"], answer: 2, why: "Each user's random salt makes their hash unique." },
    ],
    [
      {
        exercise: "salt",
        task: 'Write hash_password(password, salt) with hashlib.scrypt (n=2**14, r=8, p=1) and verify(password, salt, stored) using hmac.compare_digest. With a random 16-byte salt, store the hash of "letmein", then print verify("letmein", salt, stored) and verify("letmeout", salt, stored).',
        expected: ["True", "False"],
        hints: ["hashlib.scrypt(password.encode(), salt=salt, n=2**14, r=8, p=1)", "verify returns hmac.compare_digest(hash_password(password, salt), stored)"],
        solution: 'import hashlib, hmac, os\n\ndef hash_password(password, salt):\n    return hashlib.scrypt(password.encode(), salt=salt, n=2**14, r=8, p=1)\n\ndef verify(password, salt, stored):\n    return hmac.compare_digest(hash_password(password, salt), stored)\n\nsalt = os.urandom(16)\nstored = hash_password("letmein", salt)\nprint(verify("letmein", salt, stored))\nprint(verify("letmeout", salt, stored))',
      },
    ],
  ],
  recapPt: [
    "Um sal aleatório por utilizador faz passwords iguais darem hashes diferentes.",
    "Usa um hash lento (scrypt) para cada tentativa custar caro.",
    "Guarda o sal junto do hash.",
    "Compara segredos com hmac.compare_digest, não com ==.",
  ],
  recap: [
    "A random salt per user makes equal passwords give different hashes.",
    "Use a slow hash (scrypt) so every guess is expensive.",
    "Store the salt with the hash.",
    "Compare secrets with hmac.compare_digest, not ==.",
  ],
};
