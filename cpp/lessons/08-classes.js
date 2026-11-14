module.exports = {
  id: "classes",
  title: "Classes: a template for objects",
  titlePt: "Classes: um molde para objetos",
  screens: [
    [
      "A class is a template for objects of the same shape: some values (members) and functions that work on them (methods). The class ends with }; . Then the class name is a type like int:",
      { code: '#include <iostream>\n#include <string>\n\nclass Student {\npublic:\n    std::string name;\n    int grade;\n\n    bool passed() const\n    {\n        return grade >= 10;\n    }\n};\n\nint main()\n{\n    Student rui{"Rui", 8};\n    std::cout << rui.name << " " << rui.passed() << "\\n";\n    return 0;\n}' },
      "public: means code outside the class may use what follows. const after passed() promises it does not change the object.",
    ],
    [
      "A constructor is a method with the class's own name and no return type. It runs once for every new object, to set it up:",
      { code: '#include <iostream>\n#include <string>\n\nclass Counter {\npublic:\n    Counter(int start) : count(start) {}\n\n    void add() { count++; }\n    int value() const { return count; }\n\nprivate:\n    int count;\n};\n\nint main()\n{\n    Counter a(10);\n    Counter b(0);\n    a.add();\n    a.add();\n    b.add();\n    std::cout << a.value() << " " << b.value() << "\\n";\n    return 0;\n}' },
      ": count(start) sets count before the body runs. Each object keeps its own count.",
    ],
    [
      "private: members can only be used by the class's own methods. Code outside must go through the public methods, so the object can protect its own values:",
      { code: '#include <iostream>\n\nclass Account {\npublic:\n    int balance() const { return money; }\n\nprivate:\n    int money = 0;\n};\n\nint main()\n{\n    Account acc;\n    std::cout << acc.money << "\\n";\n    return 0;\n}', error: true },
      { quiz: "Where can a private member be used?", choices: ["anywhere", "only inside the class's own methods", "only in main"], answer: 2, why: "private keeps it inside the class." },
    ],
    [
      {
        exercise: "account",
        task: 'Write a class Account with a constructor taking the owner name, a private balance starting at 0, deposit(amount), withdraw(amount) that prints "Not enough money" if amount is more than the balance (otherwise subtracts), and report() that prints OWNER has BALANCE. Then: Account a("Ana"); a.deposit(50); a.withdraw(20); a.withdraw(100); a.report();',
        expected: ["Not enough money", "Ana has 30"],
        hints: [
          "class Account { public: Account(std::string o) : owner(o) {} ... private: std::string owner; int balance = 0; };",
          "withdraw: if (amount > balance) { ... } else { balance -= amount; }",
        ],
        solution: '#include <iostream>\n#include <string>\n\nclass Account {\npublic:\n    Account(std::string o) : owner(o) {}\n\n    void deposit(int amount) { balance += amount; }\n\n    void withdraw(int amount)\n    {\n        if (amount > balance) {\n            std::cout << "Not enough money\\n";\n        } else {\n            balance -= amount;\n        }\n    }\n\n    void report() const { std::cout << owner << " has " << balance << "\\n"; }\n\nprivate:\n    std::string owner;\n    int balance = 0;\n};\n\nint main()\n{\n    Account a("Ana");\n    a.deposit(50);\n    a.withdraw(20);\n    a.withdraw(100);\n    a.report();\n    return 0;\n}',
      },
    ],
  ],
  recapPt: [
    "Uma classe junta valores (membros) e funções (métodos) num tipo novo.",
    "O construtor tem o nome da classe e corre para cada objeto novo.",
    "private: só os métodos da classe usam; public: qualquer código usa.",
    "const depois de um método promete que não muda o objeto.",
  ],
  recap: [
    "A class bundles values (members) and functions (methods) into a new type.",
    "The constructor has the class's name and runs for every new object.",
    "private: only the class's methods may use it; public: any code may.",
    "const after a method promises it does not change the object.",
  ],
};
