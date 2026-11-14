module.exports = {
  id: "classes",
  title: "Classes: a template for objects",
  screens: [
    [
      "A class is a template for objects of the same shape. Write it once, make many:",
      { code: 'class Student {\n  constructor(name) {\n    this.name = name;\n  }\n}\nconst ana = new Student("Ana");\nconsole.log(ana.name);' },
    ],
    [
      'What new Student("Ana") does:',
      "1. new makes an empty object.\n2. constructor runs with name = \"Ana\".\n3. this is that new object, so this.name = name stores \"Ana\" in it.\n4. The object goes into ana.",
      "Forgetting new is an error:",
      { code: 'class Student {}\nconst ana = Student();', error: true },
    ],
    [
      "Methods are functions inside the class. In a method, this is the object before the dot:",
      { code: 'class Student {\n  constructor(grade) {\n    this.grade = grade;\n  }\n  passed() {\n    return this.grade >= 10;\n  }\n}\nconst rui = new Student(8);\nconsole.log(rui.passed());' },
      { quiz: "In rui.passed(), what is this?", choices: ["the class", "rui", "nothing"], answer: 2, why: "this is the object before the dot." },
    ],
    [
      "Each object keeps its own values:",
      { code: "class Counter {\n  count = 0;\n  add() {\n    this.count = this.count + 1;\n  }\n}\nconst a = new Counter();\nconst b = new Counter();\na.add();\nconsole.log(a.count, b.count);" },
      "count = 0 at the top gives every new object a starting value. Inside methods, it is always this.count; plain count is an error.",
    ],
    [
      "A # name is private: only the class's own code can use it:",
      { code: "class Account {\n  #balance = 0;\n}\nconst acc = new Account();\nconsole.log(acc.#balance);", error: true },
      "Use it for values that should only change through the class's methods.",
    ],
    [
      "extends builds a class on another. super(...) runs the parent's constructor:",
      { code: 'class Animal {\n  constructor(name) {\n    this.name = name;\n  }\n}\nclass Dog extends Animal {\n  constructor(name, breed) {\n    super(name);\n    this.breed = breed;\n  }\n}\nconst rex = new Dog("Rex", "beagle");\nconsole.log(rex.name, rex.breed);' },
    ],
    [
      {
        exercise: "account",
        task: 'Write class Account: constructor(owner) stores the owner; private #balance starts at 0; deposit(amount) adds; withdraw(amount) prints "Not enough money" if amount is too big, else subtracts; report() prints "OWNER has BALANCE". Then: const a = new Account("Ana"); a.deposit(50); a.withdraw(20); a.withdraw(100); a.report();',
        expected: ["Not enough money", "Ana has 30"],
        hints: [
          "class Account { #balance = 0; constructor(owner) { this.owner = owner; } }",
          "withdraw: if (amount > this.#balance) { ... } else { ... }",
          "report() { console.log(`${this.owner} has ${this.#balance}`); }",
        ],
        solution: 'class Account {\n  #balance = 0;\n  constructor(owner) {\n    this.owner = owner;\n  }\n  deposit(amount) {\n    this.#balance = this.#balance + amount;\n  }\n  withdraw(amount) {\n    if (amount > this.#balance) {\n      console.log("Not enough money");\n    } else {\n      this.#balance = this.#balance - amount;\n    }\n  }\n  report() {\n    console.log(`${this.owner} has ${this.#balance}`);\n  }\n}\n\nconst a = new Account("Ana");\na.deposit(50);\na.withdraw(20);\na.withdraw(100);\na.report();',
      },
    ],
  ],
  recap: [
    "A class is a template; new ClassName(...) makes an object.",
    "The constructor fills in the new object; this is that object.",
    "In a method, this is the object before the dot.",
    "#name is private to the class.",
    "extends builds on a class; super(...) runs the parent's constructor.",
  ],
};
