module.exports = {
  id: "inheritance",
  title: "Inheritance and interfaces",
  titlePt: "Herança e interfaces",
  screens: [
    [
      "One class can build on another with : Parent. It gets everything the parent has. To replace a method, the parent marks it virtual and the child writes override. base(...) runs the parent's constructor:",
      { code: 'Animal a = new Dog("Rex");\nConsole.WriteLine(a.Speak());\n\nclass Animal\n{\n    public string Name;\n    public Animal(string name) { Name = name; }\n    public virtual string Speak() => $"{Name} makes a sound";\n}\n\nclass Dog : Animal\n{\n    public Dog(string name) : base(name) { }\n    public override string Speak() => $"{Name} barks";\n}' },
      "a is declared as an Animal but holds a Dog, and the Dog's Speak runs. The real object decides.",
    ],
    [
      "An interface lists methods a class promises to have, without writing them. Interface names start with I by custom. Different classes can then be used the same way:",
      { code: "IShape s = new Square(3);\nConsole.WriteLine(s.Area());\n\ninterface IShape\n{\n    double Area();\n}\n\nclass Square : IShape\n{\n    double side;\n    public Square(double side) { this.side = side; }\n    public double Area() => side * side;\n}" },
    ],
    [
      "A class that promises an interface but leaves a method out is refused:",
      { code: "interface IShape\n{\n    double Area();\n}\n\nclass Circle : IShape\n{\n}", error: true },
      { quiz: "Animal a = new Dog(\"Rex\"); a.Speak(); with Speak virtual in Animal and override in Dog. Which runs?", choices: ["Animal's", "Dog's"], answer: 2, why: "The object is a Dog, and Speak is virtual." },
    ],
    [
      {
        exercise: "inheritance",
        task: "Write interface IShape with double Area(). Write Square (a side) and Circle (a radius, area 3.14 * r * r) implementing it. Put a Square of side 2 and a Circle of radius 1 in an IShape[] array and print each area.",
        expected: ["4", "3.14"],
        hints: ["class Circle : IShape { double r; public Circle(double r) { this.r = r; } public double Area() => 3.14 * r * r; }", "IShape[] shapes = { new Square(2), new Circle(1) }; then foreach."],
        solution: "IShape[] shapes = { new Square(2), new Circle(1) };\nforeach (IShape s in shapes)\n{\n    Console.WriteLine(s.Area());\n}\n\ninterface IShape\n{\n    double Area();\n}\n\nclass Square : IShape\n{\n    double side;\n    public Square(double side) { this.side = side; }\n    public double Area() => side * side;\n}\n\nclass Circle : IShape\n{\n    double r;\n    public Circle(double r) { this.r = r; }\n    public double Area() => 3.14 * r * r;\n}",
      },
    ],
  ],
  recapPt: [
    "class Dog : Animal recebe tudo o que Animal tem; base(...) corre o construtor da mãe.",
    "virtual na mãe e override na filha deixam substituir um método.",
    "O objeto real decide qual versão corre.",
    "Uma interface lista métodos; uma classe que a promete tem de os escrever todos.",
  ],
  recap: [
    "class Dog : Animal gets everything Animal has; base(...) runs the parent's constructor.",
    "virtual in the parent and override in the child replace a method.",
    "The real object decides which version runs.",
    "An interface lists methods; a class that promises it must write them all.",
  ],
};
