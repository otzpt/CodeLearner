module.exports = {
  id: "objects",
  title: "Objects: values with labels",
  titlePt: "Objetos: valores com nome",
  screens: [
    [
      "An array keeps values in order by position. An object keeps values by name. Each name and value pair is called a property:",
      { code: 'const student = {\n  name: "Ana",\n  age: 17,\n  city: "Porto",\n};\nconsole.log(student.name);\nconsole.log(student.age);' },
      "Read a property with a dot: student.name. It is clearer than a position number, because the name says what the value is.",
    ],
    [
      "You can change a property or add a new one the same way. Asking for a property that does not exist gives undefined, the same as a missing array position:",
      { code: 'const student = { name: "Ana", age: 17 };\nstudent.age = 18;\nstudent.course = "Computing";\nconsole.log(student);\nconsole.log(student.phone);' },
    ],
    [
      "Arrays and objects are often used together, as a list of objects:",
      { code: 'const students = [\n  { name: "Ana", grade: 15 },\n  { name: "Rui", grade: 12 },\n  { name: "Bea", grade: 18 },\n];\nfor (const s of students) {\n  console.log(`${s.name}: ${s.grade}`);\n}' },
      { quiz: "In the example above, what is students[1].name?", choices: ["Ana", "Rui", "Bea"], answer: 2, why: "Position 1 is the second item, because positions start at 0." },
    ],
    [
      "Two objects with the same contents are still two separate objects. === asks \"is this the same object?\", not \"do they look the same?\". And c = a does not copy: it gives the same object a second name, so a change through c shows up in a too:",
      { code: "const a = { x: 1 };\nconst b = { x: 1 };\nconst c = a;\nconsole.log(a === b, a === c);\nc.x = 99;\nconsole.log(a.x);" },
      { tip: "Objects store values by name. = gives an object a second name; it does not copy it." },
    ],
    [
      {
        exercise: "objects",
        task: 'Create an array of three objects, each with a name and a price: "pen" 2, "book" 15, "bag" 30. Print each as name: price, then print the total of all prices.',
        expected: ["pen: 2", "book: 15", "bag: 30", "47"],
        hints: ['Each item looks like { name: "pen", price: 2 }.', "Loop with for...of, print `${item.name}: ${item.price}`, and add item.price to a total."],
        solution: 'const items = [\n  { name: "pen", price: 2 },\n  { name: "book", price: 15 },\n  { name: "bag", price: 30 },\n];\nlet total = 0;\nfor (const item of items) {\n  console.log(`${item.name}: ${item.price}`);\n  total = total + item.price;\n}\nconsole.log(total);',
      },
    ],
  ],
  recapPt: [
    "Um objeto guarda valores com nome: { name: \"Ana\", age: 17 }.",
    "Lê-os ou muda-os com um ponto: student.age = 18.",
    "Uma propriedade que não existe dá undefined.",
    "Uma lista de objetos é a forma habitual de guardar registos.",
    "=== em objetos pergunta se são o mesmo objeto. = não copia.",
  ],
  recap: [
    "An object holds named values: { name: \"Ana\", age: 17 }.",
    "Read or change them with a dot: student.age = 18.",
    "A missing property gives undefined.",
    "A list of objects is the usual way to hold records.",
    "=== on objects asks whether they are the same object. = does not copy.",
  ],
};
