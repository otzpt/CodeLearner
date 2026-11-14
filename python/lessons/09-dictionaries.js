module.exports = {
  id: "dictionaries",
  title: "Dictionaries: values with labels",
  titlePt: "Dicionários: valores com nome",
  screens: [
    [
      "A list keeps values in order by position. A dictionary keeps values by name, called a key. Each key and value pair is written key: value, inside { }:",
      { code: 'student = {"name": "Ana", "age": 17, "city": "Porto"}\nprint(student["name"])\nprint(student["age"])' },
      "Read a value with its key in [ ]. It is clearer than a position number, because the key says what the value is.",
    ],
    [
      "You can change a value or add a new key the same way. Asking for a key that is not there is an error, so .get() is the safe way: it gives None instead:",
      { code: 'student = {"name": "Ana", "age": 17}\nstudent["age"] = 18\nstudent["course"] = "Computing"\nprint(student)\nprint(student.get("phone"))' },
    ],
    [
      "A list of dictionaries is the usual way to keep records. To go through one dictionary's keys and values, use for key, value in student.items(). Here, a list of them:",
      { code: 'students = [\n    {"name": "Ana", "grade": 15},\n    {"name": "Rui", "grade": 12},\n]\nfor s in students:\n    print(f"{s[\'name\']}: {s[\'grade\']}")' },
      { quiz: "In the list above, what is students[1][\"name\"]?", choices: ["Ana", "Rui"], answer: 2, why: "Position 1 is the second item, because positions start at 0." },
    ],
    [
      'Reading a missing key with [ ] stops the program. Use "key" in dictionary first, or .get():',
      { code: 'prices = {"pen": 2}\nprint("book" in prices)\nprint(prices["book"])', error: true },
      { tip: "Dictionaries store values by key. [key] fails on a missing key; .get(key) gives None." },
    ],
    [
      {
        exercise: "dictionaries",
        task: 'Create a list of three dictionaries, each with a "name" and a "price": pen 2, book 15, bag 30. Print each as name: price, then print the total of all prices.',
        expected: ["pen: 2", "book: 15", "bag: 30", "47"],
        hints: ['Each item looks like {"name": "pen", "price": 2}.', "Loop with for, print each, and add item[\"price\"] to a total."],
        solution: 'items = [\n    {"name": "pen", "price": 2},\n    {"name": "book", "price": 15},\n    {"name": "bag", "price": 30},\n]\ntotal = 0\nfor item in items:\n    print(f"{item[\'name\']}: {item[\'price\']}")\n    total += item["price"]\nprint(total)',
      },
    ],
  ],
  recapPt: [
    "Um dicionário guarda valores por chave: {\"name\": \"Ana\", \"age\": 17}.",
    "Lê ou muda com a chave: student[\"age\"] = 18.",
    "Uma chave que não existe dá KeyError com [ ]; .get() dá None.",
    ".items() percorre chaves e valores; uma lista de dicionários guarda registos.",
  ],
  recap: [
    "A dictionary stores values by key: {\"name\": \"Ana\", \"age\": 17}.",
    "Read or change with the key: student[\"age\"] = 18.",
    "A missing key is a KeyError with [ ]; .get() gives None.",
    ".items() walks keys and values; a list of dictionaries holds records.",
  ],
};
