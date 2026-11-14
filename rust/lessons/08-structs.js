module.exports = {
  id: "structs",
  title: "Structs: values that belong together",
  titlePt: "Structs: valores que andam juntos",
  screens: [
    [
      "A struct groups several named values, called fields, into one new type. You make one by giving every field a value, and read them with a dot:",
      { code: 'struct Student {\n    name: String,\n    grade: i32,\n}\n\nfn main() {\n    let ana = Student {\n        name: String::from("Ana"),\n        grade: 15,\n    };\n    println!("{}: {}", ana.name, ana.grade);\n}' },
    ],
    [
      "impl adds methods to a struct. A method's first parameter is &self, the struct it was called on; &mut self lets it change the struct. A function in impl without self, often called new, makes one:",
      { code: 'struct Counter {\n    count: i32,\n}\n\nimpl Counter {\n    fn new(start: i32) -> Counter {\n        Counter { count: start }\n    }\n\n    fn add(&mut self) {\n        self.count += 1;\n    }\n\n    fn value(&self) -> i32 {\n        self.count\n    }\n}\n\nfn main() {\n    let mut a = Counter::new(10);\n    a.add();\n    println!("{}", a.value());\n}' },
      "Counter::new(10) calls the function that belongs to Counter. a must be let mut, because add changes it.",
      { quiz: "In fn value(&self), what is self?", choices: ["the struct type", "the value the method was called on"], answer: 2, why: "In a.value(), self is a." },
    ],
    [
      "#[derive(Debug)] above a struct lets {:?} print the whole thing, which helps while testing:",
      { code: '#[derive(Debug)]\nstruct Point {\n    x: i32,\n    y: i32,\n}\n\nfn main() {\n    let p = Point { x: 3, y: 4 };\n    println!("{:?}", p);\n    println!("{}", p.x + p.y);\n}' },
      { tip: "struct for the fields, impl for the methods; &self reads, &mut self changes." },
    ],
    [
      {
        exercise: "account",
        task: 'Write struct Account { owner: String, balance: i32 } with fn new(owner: &str) -> Account (balance 0), deposit(&mut self, amount: i32), withdraw(&mut self, amount: i32) that prints "Not enough money" if amount is more than the balance (otherwise subtracts), and report(&self) that prints OWNER has BALANCE. Then: new("Ana"), deposit 50, withdraw 20, withdraw 100, report.',
        expected: ["Not enough money", "Ana has 30"],
        hints: ["fn new(owner: &str) -> Account { Account { owner: owner.to_string(), balance: 0 } }", "withdraw: if amount > self.balance { ... } else { self.balance -= amount; }"],
        solution: 'struct Account {\n    owner: String,\n    balance: i32,\n}\n\nimpl Account {\n    fn new(owner: &str) -> Account {\n        Account { owner: owner.to_string(), balance: 0 }\n    }\n\n    fn deposit(&mut self, amount: i32) {\n        self.balance += amount;\n    }\n\n    fn withdraw(&mut self, amount: i32) {\n        if amount > self.balance {\n            println!("Not enough money");\n        } else {\n            self.balance -= amount;\n        }\n    }\n\n    fn report(&self) {\n        println!("{} has {}", self.owner, self.balance);\n    }\n}\n\nfn main() {\n    let mut a = Account::new("Ana");\n    a.deposit(50);\n    a.withdraw(20);\n    a.withdraw(100);\n    a.report();\n}',
      },
    ],
  ],
  recapPt: [
    "struct junta campos com nome num tipo novo; lêem-se com um ponto.",
    "impl junta métodos; &self lê, &mut self muda.",
    "Uma função em impl sem self (como new) chama-se com Tipo::new(...).",
    "#[derive(Debug)] deixa mostrar tudo com {:?}.",
  ],
  recap: [
    "A struct groups named fields into a new type; read them with a dot.",
    "impl adds methods; &self reads, &mut self changes.",
    "A function in impl without self (like new) is called as Type::new(...).",
    "#[derive(Debug)] lets {:?} print the whole thing.",
  ],
};
