module.exports = {
  id: "variables",
  title: "Variables: giving values a name",
  titlePt: "Variáveis: dar nome a valores",
  screens: [
    [
      "A variable is a name that holds a value, so you can use it later. Think of a labelled box. You create one with a name, = and the value:",
      { code: "age = 25\nprint(age)\nprint(age + 1)" },
      '= here does not mean "equals" like in maths. It means "put this value in the box". The name age stands for 25 from then on.',
    ],
    [
      "Giving a variable a new value replaces the old one:",
      { code: "score = 10\nscore = score + 5\nprint(score)\nscore += 5\nprint(score)" },
      "score = score + 5 reads strange at first. Read it right to left: take score (10), add 5, put the result (15) back into score. score += 5 is a shorter way to write the same thing.",
    ],
    [
      "A variable can hold any kind of value, and can even switch kinds. type() shows what it holds right now:",
      { code: 'thing = 5\nprint(type(thing))\nthing = "five"\nprint(type(thing))' },
      { quiz: "After x = 3 and then x = x * 2, what is x?", choices: ["3", "6", "x * 2"], answer: 2, why: "x * 2 is worked out (6) and stored back in x." },
    ],
    [
      "Names can use letters, digits and _, but cannot start with a digit or contain spaces. When a name has several words, the custom is to join them with _: total_price, first_name.",
      "Capitals matter. total and Total are two different names. Using a name that was never given a value is an error:",
      { code: "total = 3\nprint(Total)", error: true },
      { tip: "Most beginner errors are a name spelled two different ways. Read the name in the error message letter by letter." },
    ],
    [
      {
        exercise: "variables",
        task: "Create a variable price with the value 20 and a variable quantity with the value 3. Print price times quantity. Then change price to 25 and print price times quantity again.",
        expected: ["60", "75"],
        hints: ["Two lines: price = 20 and quantity = 3.", "After the first print, write price = 25 and print again."],
        solution: "price = 20\nquantity = 3\nprint(price * quantity)\nprice = 25\nprint(price * quantity)",
      },
    ],
  ],
  recapPt: [
    "Uma variável é uma caixa com nome para um valor: age = 25",
    "= quer dizer \"guarda isto\", não \"é igual a\".",
    "Dar um valor novo substitui o antigo. score += 5 é o mesmo que score = score + 5.",
    "Maiúsculas e minúsculas contam. Um nome que nunca recebeu valor dá NameError.",
  ],
  recap: [
    "A variable is a named box for a value: age = 25",
    "= means \"store this\", not \"is equal to\".",
    "A new value replaces the old one. score += 5 is score = score + 5.",
    "Names are case sensitive. A name that was never given a value is a NameError.",
  ],
};
