module.exports = {
  id: "classes",
  title: "Classes and objects",
  titlePt: "Classes e objetos",
  screens: [
    [
      "A class is a template for objects of the same shape: some values (fields) and methods that work on them. new makes an object from it. A second class can sit in the same file, without public:",
      { code: 'class Student {\n    String name;\n    int grade;\n\n    boolean passed() {\n        return grade >= 10;\n    }\n}\n\npublic class Main {\n    public static void main(String[] args) {\n        Student rui = new Student();\n        rui.name = "Rui";\n        rui.grade = 8;\n        System.out.println(rui.name + " " + rui.passed());\n    }\n}' },
      "passed() has no static: it belongs to one object and uses that object's grade. static methods, like main, belong to the class and have no object.",
    ],
    [
      "A constructor sets up each new object. It has the class's own name and no return type. this.name means \"this object's name\", as opposed to the parameter name:",
      { code: 'class Counter {\n    private int count;\n\n    Counter(int start) {\n        this.count = start;\n    }\n\n    void add() { count++; }\n    int value() { return count; }\n}\n\npublic class Main {\n    public static void main(String[] args) {\n        Counter a = new Counter(10);\n        Counter b = new Counter(0);\n        a.add();\n        b.add();\n        System.out.println(a.value() + " " + b.value());\n    }\n}' },
      "Each object keeps its own count.",
    ],
    [
      "private fields can only be used by the class's own methods. Code outside must go through its methods, so the object protects its values:",
      { code: 'class Account {\n    private int money = 0;\n}\n\npublic class Main {\n    public static void main(String[] args) {\n        Account acc = new Account();\n        System.out.println(acc.money);\n    }\n}', error: true },
      { quiz: "In a constructor Student(String name) { this.name = name; }, what is this.name?", choices: ["the parameter", "the new object's field"], answer: 2, why: "this.name is the object's field; name alone is the parameter." },
    ],
    [
      {
        exercise: "account",
        task: 'Write a class Account: a constructor taking the owner name, a private balance starting at 0, deposit(amount), withdraw(amount) that prints "Not enough money" if amount is more than the balance (otherwise subtracts), and report() that prints OWNER has BALANCE. In main: Account a = new Account("Ana"); a.deposit(50); a.withdraw(20); a.withdraw(100); a.report();',
        expected: ["Not enough money", "Ana has 30"],
        hints: ["class Account { private String owner; private int balance = 0; Account(String owner) { this.owner = owner; } ... }", "withdraw: if (amount > balance) { ... } else { balance -= amount; }"],
        solution: 'class Account {\n    private String owner;\n    private int balance = 0;\n\n    Account(String owner) {\n        this.owner = owner;\n    }\n\n    void deposit(int amount) {\n        balance += amount;\n    }\n\n    void withdraw(int amount) {\n        if (amount > balance) {\n            System.out.println("Not enough money");\n        } else {\n            balance -= amount;\n        }\n    }\n\n    void report() {\n        System.out.println(owner + " has " + balance);\n    }\n}\n\npublic class Main {\n    public static void main(String[] args) {\n        Account a = new Account("Ana");\n        a.deposit(50);\n        a.withdraw(20);\n        a.withdraw(100);\n        a.report();\n    }\n}',
      },
    ],
  ],
  recapPt: [
    "Uma classe junta campos e métodos; new Classe(...) cria um objeto.",
    "O construtor tem o nome da classe; this.campo é o campo deste objeto.",
    "private: só os métodos da classe usam.",
    "Métodos sem static pertencem a um objeto; static pertence à classe.",
  ],
  recap: [
    "A class bundles fields and methods; new Class(...) makes an object.",
    "The constructor has the class's name; this.field is this object's field.",
    "private: only the class's methods may use it.",
    "Methods without static belong to an object; static belongs to the class.",
  ],
};
