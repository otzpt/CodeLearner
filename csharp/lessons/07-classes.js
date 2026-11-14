module.exports = {
  id: "classes",
  title: "Classes and objects",
  titlePt: "Classes e objetos",
  screens: [
    [
      "A class is a template for objects of the same shape: some values (properties) and methods that work on them. new makes an object from it. In a file like this, the class goes after the code that uses it:",
      { code: 'Student rui = new Student("Rui", 8);\nConsole.WriteLine($"{rui.Name} {rui.Passed()}");\n\nclass Student\n{\n    public string Name;\n    public int Grade;\n\n    public Student(string name, int grade)\n    {\n        Name = name;\n        Grade = grade;\n    }\n\n    public bool Passed() => Grade >= 10;\n}' },
      "The method with the class's own name and no return type is the constructor: it runs once for every new object. public means code outside the class may use it.",
    ],
    [
      "Each object keeps its own values:",
      { code: "Counter a = new Counter();\nCounter b = new Counter();\na.Add();\na.Add();\nb.Add();\nConsole.WriteLine($\"{a.Count} {b.Count}\");\n\nclass Counter\n{\n    public int Count { get; private set; }\n    public void Add() => Count++;\n}" },
      "{ get; private set; } makes Count readable by anyone but changeable only by the class itself.",
    ],
    [
      "Members are private unless marked public: only the class's own code can use them. Code outside must go through public methods, so the object protects its values:",
      { code: "Account acc = new Account();\nConsole.WriteLine(acc.money);\n\nclass Account\n{\n    int money = 0;\n}", error: true },
      { quiz: "A class member has no public or private written. Who can use it?", choices: ["anyone", "only the class's own code"], answer: 2, why: "Members are private unless marked public." },
    ],
    [
      {
        exercise: "account",
        task: 'Write a class Account: a constructor taking the owner name, a private balance starting at 0, Deposit(amount), Withdraw(amount) that prints "Not enough money" if amount is more than the balance (otherwise subtracts), and Report() that prints OWNER has BALANCE. Then: var a = new Account("Ana"); a.Deposit(50); a.Withdraw(20); a.Withdraw(100); a.Report();',
        expected: ["Not enough money", "Ana has 30"],
        hints: ["class Account { string owner; int balance = 0; public Account(string owner) { this.owner = owner; } ... }", "Withdraw: if (amount > balance) { ... } else { balance -= amount; }"],
        solution: 'var a = new Account("Ana");\na.Deposit(50);\na.Withdraw(20);\na.Withdraw(100);\na.Report();\n\nclass Account\n{\n    string owner;\n    int balance = 0;\n\n    public Account(string owner)\n    {\n        this.owner = owner;\n    }\n\n    public void Deposit(int amount) => balance += amount;\n\n    public void Withdraw(int amount)\n    {\n        if (amount > balance)\n        {\n            Console.WriteLine("Not enough money");\n        }\n        else\n        {\n            balance -= amount;\n        }\n    }\n\n    public void Report() => Console.WriteLine($"{owner} has {balance}");\n}',
      },
    ],
  ],
  recapPt: [
    "Uma classe junta valores e métodos; new Classe(...) cria um objeto.",
    "O construtor tem o nome da classe e corre para cada objeto novo.",
    "Os membros são privados a não ser que digam public.",
    "{ get; private set; } deixa ler de fora mas só mudar dentro.",
  ],
  recap: [
    "A class bundles values and methods; new Class(...) makes an object.",
    "The constructor has the class's name and runs for every new object.",
    "Members are private unless marked public.",
    "{ get; private set; } lets anyone read it but only the class change it.",
  ],
};
