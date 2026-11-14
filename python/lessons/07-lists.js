module.exports = {
  id: "lists",
  title: "Lists of things",
  titlePt: "Listas de coisas",
  screens: [
    [
      "A list holds many values in order, in one variable. Write them between [ ], separated by commas. len() counts them, and positions start at 0, the same as in a string:",
      { code: 'fruits = ["apple", "banana", "cherry"]\nprint(len(fruits))\nprint(fruits[0])\nprint(fruits[-1])\nprint(fruits)' },
    ],
    [
      "Asking for a position that does not exist is an error:",
      { code: 'fruits = ["apple", "banana", "cherry"]\nprint(fruits[3])', error: true },
      "A list of 3 has positions 0, 1 and 2. Going one past the end is a very common mistake.",
    ],
    [
      "append adds a value to the end. You can also replace a value by its position:",
      { code: 'shopping = ["milk"]\nshopping.append("bread")\nshopping.append("eggs")\nshopping[0] = "oat milk"\nprint(shopping)' },
    ],
    [
      "for goes through a list one value at a time. in asks whether a value is there:",
      { code: 'fruits = ["apple", "banana"]\nfor fruit in fruits:\n    print(f"I like {fruit}")\nprint("kiwi" in fruits)' },
      { quiz: 'What does print(["a", "b", "c"][1]) show?', choices: ["a", "b", "c"], answer: 2, why: "Position 1 is the second item; counting starts at 0." },
    ],
    [
      "b = a does not copy a list. It gives the same list a second name, so a change through b shows up in a too. a.copy() makes a real copy:",
      { code: "a = [1, 2, 3]\nb = a\nb.append(4)\nprint(a)\nc = a.copy()\nc.append(5)\nprint(a)" },
      "sorted(list) gives a new sorted list; list.sort() sorts the list itself.",
      { tip: "Positions run from 0 to len - 1. = gives a list a second name; copy() copies it." },
    ],
    [
      {
        exercise: "shopping",
        task: 'Create a list with "rice", "beans", "oil". Add "salt" to the end. Print how many items there are, then print every item on its own line starting with "- ".',
        expected: ["4", "- rice", "- beans", "- oil", "- salt"],
        hints: ["append adds salt. len() gives the count.", 'for item in items: print("- " + item)'],
        solution: 'items = ["rice", "beans", "oil"]\nitems.append("salt")\nprint(len(items))\nfor item in items:\n    print("- " + item)',
      },
    ],
  ],
  recapPt: [
    "Uma lista: [1, 2, 3]. len() conta os elementos.",
    "As posições vão de 0 a len - 1; ir além do fim dá IndexError.",
    ".append(valor) junta ao fim; lista[0] = ... substitui.",
    "for x in lista: percorre os valores; in pergunta se um valor está lá.",
    "b = a não copia; a.copy() copia.",
  ],
  recap: [
    "A list: [1, 2, 3]. len() counts its items.",
    "Positions go from 0 to len - 1; going past the end is an IndexError.",
    ".append(value) adds to the end; list[0] = ... replaces.",
    "for x in list: walks the values; in asks whether a value is there.",
    "b = a does not copy; a.copy() does.",
  ],
};
