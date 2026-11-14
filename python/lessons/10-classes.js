module.exports = {
  id: "classes",
  title: "Classes: a template for objects",
  titlePt: "Classes: um molde para objetos",
  screens: [
    [
      "In lesson 9 you wrote each record by hand as a dictionary. With many students, that means typing the same keys again and again, and a typo in one of them goes unnoticed.",
      "A class is a template for making objects of the same shape. You describe it once, then make as many objects from it as you need:",
      { code: 'class Student:\n    def __init__(self, name, grade):\n        self.name = name\n        self.grade = grade\n\nana = Student("Ana", 15)\nrui = Student("Rui", 12)\nprint(ana.name, ana.grade)\nprint(rui.name, rui.grade)' },
      "Class names start with a capital letter by custom: Student, not student.",
    ],
    [
      'Step by step, here is what Student("Ana", 15) does:',
      "1. Python makes a fresh, empty object.\n2. __init__ runs (two underscores on each side), with self = that new object, name = \"Ana\" and grade = 15.\n3. self.name = name stores \"Ana\" in the object's name.\n4. The finished object goes into the variable ana.",
      "So self.name (the object's own value) and name (the value passed in) are two different things that happen to share a word. self is always the first parameter, but you never pass it yourself: Python does.",
      { quiz: 'In Student("Ana", 15), what is self inside __init__?', choices: ["the Student class", "the new object being made", '"Ana"'], answer: 2, why: "self is the object being built; name is \"Ana\"." },
    ],
    [
      "A class can also hold functions, called methods. They are written inside the class, take self first, and every object made from the class can use them:",
      { code: 'class Student:\n    def __init__(self, name, grade):\n        self.name = name\n        self.grade = grade\n\n    def passed(self):\n        return self.grade >= 10\n\n    def add_points(self, points):\n        self.grade += points\n\nrui = Student("Rui", 8)\nrui.add_points(3)\nprint(rui.grade, rui.passed())' },
      "In rui.add_points(3), self is rui, so self.grade is rui's grade.",
    ],
    [
      "Each object keeps its own values. Inside a method, they are always reached through self. Forgetting self. means Python looks for an ordinary variable, and there is none:",
      { code: "class Counter:\n    def __init__(self):\n        self.count = 0\n\n    def show(self):\n        print(count)\n\nCounter().show()", error: true },
      { tip: "Inside a class, the object's own values are self.something." },
    ],
    [
      "One class can build on another. Put the parent class in brackets: the new class gets everything the parent has, and can replace parts. super().__init__(...) runs the parent's __init__:",
      { code: 'class Animal:\n    def __init__(self, name):\n        self.name = name\n\n    def speak(self):\n        return f"{self.name} makes a sound"\n\nclass Dog(Animal):\n    def __init__(self, name, breed):\n        super().__init__(name)\n        self.breed = breed\n\n    def speak(self):\n        return f"{self.name} the {self.breed} barks"\n\nprint(Animal("Cat").speak())\nprint(Dog("Rex", "beagle").speak())' },
    ],
    [
      {
        exercise: "account",
        task: 'Write a class Account. __init__ takes an owner name and stores it, and starts self.balance at 0. deposit(amount) adds to it. withdraw(amount) prints "Not enough money" if amount is more than the balance, and otherwise takes it away. report() prints OWNER has BALANCE. Then run: a = Account("Ana"), a.deposit(50), a.withdraw(20), a.withdraw(100), a.report().',
        expected: ["Not enough money", "Ana has 30"],
        hints: [
          "class Account:\n    def __init__(self, owner):\n        self.owner = owner\n        self.balance = 0",
          "withdraw needs an if: if amount > self.balance: ... else: ...",
          'report prints f"{self.owner} has {self.balance}"',
        ],
        solution: 'class Account:\n    def __init__(self, owner):\n        self.owner = owner\n        self.balance = 0\n\n    def deposit(self, amount):\n        self.balance += amount\n\n    def withdraw(self, amount):\n        if amount > self.balance:\n            print("Not enough money")\n        else:\n            self.balance -= amount\n\n    def report(self):\n        print(f"{self.owner} has {self.balance}")\n\na = Account("Ana")\na.deposit(50)\na.withdraw(20)\na.withdraw(100)\na.report()',
      },
    ],
  ],
  recapPt: [
    "Uma classe é um molde; Student(...) cria um objeto a partir dela.",
    "__init__ corre uma vez por cada objeto novo; self é esse objeto.",
    "Métodos são funções dentro da classe, com self como primeiro parâmetro.",
    "Cada objeto tem os seus próprios valores, sempre acedidos com self.",
    "class Dog(Animal): constrói sobre outra classe; super().__init__(...) corre o __init__ da mãe.",
  ],
  recap: [
    "A class is a template; Student(...) makes an object from it.",
    "__init__ runs once per new object; self is that object.",
    "Methods are functions inside the class, with self first.",
    "Each object has its own values, always reached through self.",
    "class Dog(Animal): builds on another class; super().__init__(...) runs the parent's __init__.",
  ],
};
