module.exports = {
  id: "structs",
  title: "Structs: values that belong together",
  titlePt: "Structs: valores que andam juntos",
  screens: [
    [
      "A struct groups several values, each with its own name and type, into one new type. Its parts are called members, and you reach them with a dot:",
      { code: '#include <stdio.h>\n\nstruct student {\n    char name[20];\n    int grade;\n};\n\nint main(void)\n{\n    struct student ana = {"Ana", 15};\n    ana.grade = 17;\n    printf("%s: %d\\n", ana.name, ana.grade);\n    return 0;\n}' },
      "The struct is defined once, above main, and ends with }; . Then struct student is a type like int.",
    ],
    [
      "typedef gives a type a shorter name, so you can write Student instead of struct student. An array of structs is the usual way to keep records:",
      { code: '#include <stdio.h>\n\ntypedef struct {\n    char name[20];\n    int grade;\n} Student;\n\nint main(void)\n{\n    Student class[] = {{"Ana", 15}, {"Rui", 12}, {"Bea", 18}};\n    for (int i = 0; i < 3; i++) {\n        printf("%s: %d\\n", class[i].name, class[i].grade);\n    }\n    return 0;\n}' },
      { quiz: "In the example above, what is class[1].name?", choices: ["Ana", "Rui", "Bea"], answer: 2, why: "Position 1 is the second record; positions start at 0." },
    ],
    [
      "Passing a struct to a function copies all of it. To let the function change it, pass its address. Through a pointer, members are reached with -> instead of a dot:",
      { code: '#include <stdio.h>\n\ntypedef struct {\n    char name[20];\n    int grade;\n} Student;\n\nvoid add_points(Student *s, int points)\n{\n    s->grade += points;\n}\n\nint main(void)\n{\n    Student rui = {"Rui", 8};\n    add_points(&rui, 3);\n    printf("%s: %d\\n", rui.name, rui.grade);\n    return 0;\n}' },
      "s->grade is short for (*s).grade: the grade of the struct that s points to.",
      { tip: "A dot for a struct, -> through a pointer to a struct." },
    ],
    [
      {
        exercise: "structs",
        task: 'Make a typedef struct Item with a name (char[20]) and a price (int). Store pen 2, book 15 and bag 30 in an array. Print each as "name: price", then the total.',
        expected: ["pen: 2", "book: 15", "bag: 30", "total 47"],
        hints: ['typedef struct { char name[20]; int price; } Item;', 'Item items[] = {{"pen", 2}, {"book", 15}, {"bag", 30}};'],
        solution: '#include <stdio.h>\n\ntypedef struct {\n    char name[20];\n    int price;\n} Item;\n\nint main(void)\n{\n    Item items[] = {{"pen", 2}, {"book", 15}, {"bag", 30}};\n    int total = 0;\n    for (int i = 0; i < 3; i++) {\n        printf("%s: %d\\n", items[i].name, items[i].price);\n        total += items[i].price;\n    }\n    printf("total %d\\n", total);\n    return 0;\n}',
      },
    ],
  ],
  recapPt: [
    "struct junta vários valores com nome num tipo novo; acede-se com um ponto.",
    "typedef struct { ... } Nome; deixa escrever só Nome.",
    "Um array de structs guarda registos.",
    "Através de um ponteiro usa-se ->: s->grade é (*s).grade.",
  ],
  recap: [
    "A struct groups named values into a new type; reach them with a dot.",
    "typedef struct { ... } Name; lets you write just Name.",
    "An array of structs holds records.",
    "Through a pointer, use ->: s->grade is (*s).grade.",
  ],
};
