module.exports = {
  id: "objects",
  title: "Objects: values with labels",
  screens: [
    [
      "An object stores values by name instead of position:",
      { code: 'const student = { name: "Ana", age: 17 };\nconsole.log(student.name);\nstudent.age = 18;\nconsole.log(student);' },
      "A missing name gives undefined, like a missing array position.",
    ],
    [
      "A list of objects is the usual way to keep records:",
      { code: 'const students = [\n  { name: "Ana", grade: 15 },\n  { name: "Rui", grade: 12 },\n];\nfor (const s of students) {\n  console.log(`${s.name}: ${s.grade}`);\n}' },
      { quiz: "In that list, what is students[1].name?", choices: ["Ana", "Rui"], answer: 2, why: "Position 1 is the second item." },
    ],
    [
      "= gives an object a second name. It does not copy it:",
      { code: "const a = { x: 1 };\nconst c = a;\nc.x = 99;\nconsole.log(a.x);" },
      "And === asks \"same object?\", not \"same contents?\":",
      { code: "console.log({ x: 1 } === { x: 1 });" },
    ],
    [
      {
        exercise: "objects",
        task: 'Make a list of { name, price } objects: pen 2, book 15, bag 30. Print each as "name: price", then the total.',
        expected: ["pen: 2", "book: 15", "bag: 30", "47"],
        hints: ['{ name: "pen", price: 2 }', "for...of, print `${item.name}: ${item.price}`, add to a total."],
        solution: 'const items = [\n  { name: "pen", price: 2 },\n  { name: "book", price: 15 },\n  { name: "bag", price: 30 },\n];\nlet total = 0;\nfor (const item of items) {\n  console.log(`${item.name}: ${item.price}`);\n  total = total + item.price;\n}\nconsole.log(total);',
      },
    ],
  ],
  recap: [
    "{ name: \"Ana\", age: 17 } stores values by name; read with a dot.",
    "A missing name gives undefined.",
    "= does not copy; === asks whether it is the same object.",
  ],
};
