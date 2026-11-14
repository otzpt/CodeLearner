module.exports = {
  id: "lists",
  title: "Lists of things (arrays)",
  screens: [
    [
      "An array is a list. Positions start at 0:",
      { code: 'const fruits = ["apple", "banana", "cherry"];\nconsole.log(fruits.length);\nconsole.log(fruits[0]);\nconsole.log(fruits[3]);' },
      "Position 3 is empty, so it gives undefined, not an error. Easy to miss.",
    ],
    [
      "push adds to the end. A position can be replaced:",
      { code: 'const list = ["milk"];\nlist.push("bread");\nlist[0] = "oat milk";\nconsole.log(list);' },
      "This works on a const: const stops reassigning the name, not changing the contents.",
    ],
    [
      "for...of gives each value. for...in gives the positions, as strings:",
      { code: 'const fruits = ["apple", "banana"];\nfor (const f of fruits) console.log(f);\nfor (const i in fruits) console.log(i);' },
      { quiz: 'for (const x in ["a", "b"]) console.log(x);  What is printed first?', choices: ["a", "0", "1"], answer: 2, why: "in gives positions, starting at 0." },
    ],
    [
      "sort() compares as text, even numbers:",
      { code: "console.log([10, 9, 2].sort());\nconsole.log([10, 9, 2].sort((a, b) => a - b));" },
      "The second form sorts numbers. Lesson 8 explains the (a, b) => part.",
    ],
    [
      {
        exercise: "shopping",
        task: 'Make an array of "rice", "beans", "oil" and push "salt". Print the count, then each item as "- item".',
        expected: ["4", "- rice", "- beans", "- oil", "- salt"],
        hints: [".push and .length.", 'for (const item of list) console.log("- " + item);'],
        solution: 'const list = ["rice", "beans", "oil"];\nlist.push("salt");\nconsole.log(list.length);\nfor (const item of list) {\n  console.log("- " + item);\n}',
      },
    ],
  ],
  recap: [
    "[1, 2, 3] is an array; .length counts it.",
    "Positions start at 0; an empty one is undefined.",
    "for...of gives values, for...in gives positions.",
    "sort() sorts as text; use sort((a, b) => a - b) for numbers.",
  ],
};
