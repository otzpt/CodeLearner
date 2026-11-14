module.exports = {
  id: "classes",
  title: "Classes: a template for objects",
  titlePt: "Classes: um molde para objetos",
  screens: [
    [
      "In lesson 9 you wrote each object by hand: { name: \"Ana\", grade: 15 }. With many students, that means typing the same shape again and again, and a typo in one of them goes unnoticed.",
      "A class is a template for making objects of the same shape. You describe it once, then make as many objects from it as you need.",
      { code: 'class Student {\n  constructor(name, grade) {\n    this.name = name;\n    this.grade = grade;\n  }\n}\n\nconst ana = new Student("Ana", 15);\nconst rui = new Student("Rui", 12);\nconsole.log(ana.name, ana.grade);\nconsole.log(rui.name, rui.grade);' },
      "Class names start with a capital letter by custom: Student, not student.",
    ],
    [
      "Step by step, here is what new Student(\"Ana\", 15) does:",
      "1. new makes a fresh, empty object.\n2. The constructor runs, with name = \"Ana\" and grade = 15.\n3. Inside the constructor, this means that fresh object. this.name = name stores \"Ana\" in its name property.\n4. new gives the finished object back, and it goes into the variable ana.",
      "So this.name (the object's property) and name (the value passed in) are two different things that happen to share a word.",
      "Forgetting new is an error. A class can only be used with it:",
      { code: 'class Student {\n  constructor(name) {\n    this.name = name;\n  }\n}\nconst ana = Student("Ana");', error: true },
    ],
    [
      "A class can also hold functions, called methods. They are written inside the class, without the word function, and every object made from the class can use them:",
      { code: 'class Student {\n  constructor(name, grade) {\n    this.name = name;\n    this.grade = grade;\n  }\n  passed() {\n    return this.grade >= 10;\n  }\n  addPoints(points) {\n    this.grade = this.grade + points;\n  }\n}\nconst rui = new Student("Rui", 8);\nrui.addPoints(3);\nconsole.log(rui.grade, rui.passed());' },
      "Inside a method, this means the object the method was called on. In rui.addPoints(3), this is rui, so this.grade is rui's grade.",
      { quiz: "In ana.passed(), what is this inside passed()?", choices: ["the Student class", "the object ana", "nothing"], answer: 2, why: "this is the object before the dot when the method is called." },
    ],
    [
      "Each object keeps its own values. Changing one does not touch the others:",
      { code: 'class Counter {\n  constructor() {\n    this.count = 0;\n  }\n  add() {\n    this.count = this.count + 1;\n  }\n}\n\nconst a = new Counter();\nconst b = new Counter();\na.add();\na.add();\nb.add();\nconsole.log(a.count, b.count);' },
    ],
    [
      "A common slip inside a method: writing count instead of this.count. Without this., JavaScript looks for an ordinary variable called count, and there is none:",
      { code: "class Counter {\n  constructor() {\n    this.count = 0;\n  }\n  show() {\n    console.log(count);\n  }\n}\nnew Counter().show();", error: true },
      { tip: "Inside a class, reach the object's own values with this." },
    ],
    [
      "Sometimes a value should only be changed through the class's own methods, like the balance of a bank account. A name starting with # makes a private field: only code inside the class can read or change it. Written as #balance = 0 at the top of the class, it also gets a starting value.",
      "Trying to reach it from outside the class does not even start:",
      { code: "class Account {\n  #balance = 0;\n}\nconst acc = new Account();\nconsole.log(acc.#balance);", error: true },
      "Inside the class, methods use it as this.#balance. The exercise at the end of this lesson does exactly that.",
    ],
    [
      "One class can build on another with extends. The new class gets everything the old one has, and can add to it or replace parts. If it has its own constructor, the first line must be super(...), which runs the parent's constructor:",
      { code: 'class Animal {\n  constructor(name) {\n    this.name = name;\n  }\n  speak() {\n    return `${this.name} makes a sound`;\n  }\n}\n\nclass Dog extends Animal {\n  constructor(name, breed) {\n    super(name);\n    this.breed = breed;\n  }\n  speak() {\n    return `${this.name} the ${this.breed} barks`;\n  }\n}\n\nconsole.log(new Animal("Cat").speak());\nconsole.log(new Dog("Rex", "beagle").speak());' },
      "Dog replaced speak, and got name from Animal through super(name).",
    ],
    [
      {
        exercise: "account",
        task: 'Write a class Account. The constructor takes an owner name and stores it. A private #balance starts at 0. deposit(amount) adds to it. withdraw(amount) prints "Not enough money" if amount is more than the balance, and otherwise takes it away. report() prints OWNER has BALANCE. Then run: const a = new Account("Ana"); a.deposit(50); a.withdraw(20); a.withdraw(100); a.report();',
        expected: ["Not enough money", "Ana has 30"],
        hints: [
          "Start with: class Account { #balance = 0; constructor(owner) { this.owner = owner; } }",
          "withdraw needs an if: if (amount > this.#balance) { ... } else { ... }",
          "report() { console.log(`${this.owner} has ${this.#balance}`); }",
        ],
        solution: 'class Account {\n  #balance = 0;\n  constructor(owner) {\n    this.owner = owner;\n  }\n  deposit(amount) {\n    this.#balance = this.#balance + amount;\n  }\n  withdraw(amount) {\n    if (amount > this.#balance) {\n      console.log("Not enough money");\n    } else {\n      this.#balance = this.#balance - amount;\n    }\n  }\n  report() {\n    console.log(`${this.owner} has ${this.#balance}`);\n  }\n}\n\nconst a = new Account("Ana");\na.deposit(50);\na.withdraw(20);\na.withdraw(100);\na.report();',
      },
    ],
  ],
  recapPt: [
    "Uma classe é um molde; new NomeDaClasse(...) cria um objeto a partir dela.",
    "O constructor corre uma vez por cada objeto novo; this é o objeto a ser criado.",
    "Métodos são funções dentro da classe; num método, this é o objeto antes do ponto.",
    "Cada objeto tem os seus próprios valores. Dentro da classe, chega-se a eles com this.",
    "#nome é privado: só o código da própria classe o pode usar.",
    "extends constrói sobre outra classe; super(...) corre o constructor da classe-mãe.",
  ],
  recap: [
    "A class is a template; new ClassName(...) makes an object from it.",
    "The constructor runs once per new object; this is the object being built.",
    "Methods are functions inside the class; in a method, this is the object before the dot.",
    "Each object has its own values. Inside the class, reach them with this.",
    "#name is private: only the class's own code can use it.",
    "extends builds on another class; super(...) runs the parent's constructor.",
  ],
};
