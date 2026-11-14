module.exports = {
  id: "lists",
  title: "Lists of things (arrays)",
  screens: [
    [
      "An array holds many values in order, in one variable. Write the values between [ ], separated by commas:",
      { code: 'const fruits = ["apple", "banana", "cherry"];\nconsole.log(fruits);\nconsole.log(fruits.length);' },
      "Each value has a position, starting at 0, the same as the characters of a string:",
      { code: 'const fruits = ["apple", "banana", "cherry"];\nconsole.log(fruits[0]);\nconsole.log(fruits[2]);\nconsole.log(fruits[3]);' },
      "There is nothing at position 3, so you get undefined, which means \"no value here\". It is not an error, which makes it easy to miss.",
    ],
    [
      "push adds a value to the end. You can also replace a value by its position:",
      { code: 'const list = ["milk"];\nlist.push("bread");\nlist.push("eggs");\nlist[0] = "oat milk";\nconsole.log(list);' },
      "This works even though list is a const. const only stops the name list from pointing at a different array; the array's contents can still change.",
    ],
    [
      "for...of goes through an array one value at a time:",
      { code: 'const fruits = ["apple", "banana", "cherry"];\nfor (const fruit of fruits) {\n  console.log(`I like ${fruit}`);\n}' },
      "There is also for...in, which looks almost the same but gives the positions instead, as strings, starting at \"0\":",
      { code: 'const fruits = ["apple", "banana", "cherry"];\nfor (const i in fruits) {\n  console.log(i);\n}' },
      { quiz: 'for (const x in ["a", "b"]) console.log(x);  What is printed first?', choices: ["a", "0", "1"], answer: 2, why: "in gives the positions, and positions start at 0. of would give \"a\"." },
    ],
    [
      "sort() puts an array in order, but it compares everything as text, even numbers:",
      { code: "const numbers = [10, 9, 2];\nnumbers.sort();\nconsole.log(numbers);" },
      'As text, "10" comes before "9" because "1" comes before "9". To sort numbers, give sort a small rule; lesson 8 explains how this works:',
      { code: "const numbers = [10, 9, 2];\nnumbers.sort((a, b) => a - b);\nconsole.log(numbers);" },
      { tip: "Positions start at 0. for...of gives values, for...in gives positions. sort() sorts as text." },
    ],
    [
      {
        exercise: "shopping",
        task: 'Create an array with "rice", "beans", "oil". Add "salt" to the end. Print how many items there are, then print every item on its own line starting with "- ".',
        expected: ["4", "- rice", "- beans", "- oil", "- salt"],
        hints: ["push adds salt. .length gives the count.", 'for (const item of list) { console.log("- " + item); }'],
        solution: 'const list = ["rice", "beans", "oil"];\nlist.push("salt");\nconsole.log(list.length);\nfor (const item of list) {\n  console.log("- " + item);\n}',
      },
    ],
  ],
  recap: [
    "An array is a list: [1, 2, 3]. .length is how many items.",
    "Positions start at 0. A position with nothing in it gives undefined.",
    ".push(value) adds to the end; list[0] = ... replaces.",
    "for...of gives the values; for...in gives the positions as strings.",
    "sort() sorts as text; numbers need sort((a, b) => a - b).",
  ],
};
