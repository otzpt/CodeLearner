module.exports = {
  id: "inheritance",
  title: "Inheritance: building on a class",
  titlePt: "Herança: construir sobre uma classe",
  screens: [
    [
      "One class can build on another: it gets everything the parent class has, and can add more. Write : public Parent after the new class's name:",
      { code: '#include <iostream>\n#include <string>\n\nclass Animal {\npublic:\n    Animal(std::string n) : name(n) {}\n    std::string name;\n};\n\nclass Dog : public Animal {\npublic:\n    Dog(std::string n) : Animal(n) {}\n    void fetch() const { std::cout << name << " fetches\\n"; }\n};\n\nint main()\n{\n    Dog rex("Rex");\n    rex.fetch();\n    return 0;\n}' },
      ": Animal(n) runs the parent's constructor first. Dog has a name because Animal does.",
    ],
    [
      "To let a child class replace a method, mark it virtual in the parent and override in the child. Then calling it through a reference to the parent still runs the child's version:",
      { code: '#include <iostream>\n\nclass Animal {\npublic:\n    virtual ~Animal() = default;\n    virtual void speak() const { std::cout << "some sound\\n"; }\n};\n\nclass Dog : public Animal {\npublic:\n    void speak() const override { std::cout << "woof\\n"; }\n};\n\nvoid make_it_speak(const Animal& a)\n{\n    a.speak();\n}\n\nint main()\n{\n    Animal cat;\n    Dog rex;\n    make_it_speak(cat);\n    make_it_speak(rex);\n    return 0;\n}' },
      "The virtual ~Animal() line makes cleanup correct for child classes; a class with virtual methods should always have it.",
    ],
    [
      "override asks the compiler to check that the method really replaces one in the parent. A typo in the name or a missing const becomes an error instead of a silent new method:",
      { code: '#include <iostream>\n\nclass Animal {\npublic:\n    virtual ~Animal() = default;\n    virtual void speak() const {}\n};\n\nclass Dog : public Animal {\npublic:\n    void speek() const override {}\n};\n\nint main()\n{\n    return 0;\n}', error: true },
      { quiz: "Without virtual on the parent's speak(), which version does make_it_speak(rex) run?", choices: ["Dog's", "Animal's"], answer: 2, why: "Only virtual methods choose the version by the real object." },
    ],
    [
      {
        exercise: "inheritance",
        task: 'Write a class Shape with virtual double area() const returning 0, and a virtual destructor. Write Square (a side) and Circle (a radius, area 3.14 * r * r), each overriding area(). Put a Square of side 2 and a Circle of radius 1 in a function print_area(const Shape& s) that prints s.area().',
        expected: ["4", "3.14"],
        hints: ["class Square : public Shape { public: Square(double s) : side(s) {} double area() const override { return side * side; } private: double side; };", "void print_area(const Shape& s) { std::cout << s.area() << \"\\n\"; }"],
        solution: '#include <iostream>\n\nclass Shape {\npublic:\n    virtual ~Shape() = default;\n    virtual double area() const { return 0; }\n};\n\nclass Square : public Shape {\npublic:\n    Square(double s) : side(s) {}\n    double area() const override { return side * side; }\n\nprivate:\n    double side;\n};\n\nclass Circle : public Shape {\npublic:\n    Circle(double r) : radius(r) {}\n    double area() const override { return 3.14 * radius * radius; }\n\nprivate:\n    double radius;\n};\n\nvoid print_area(const Shape& s)\n{\n    std::cout << s.area() << "\\n";\n}\n\nint main()\n{\n    print_area(Square(2));\n    print_area(Circle(1));\n    return 0;\n}',
      },
    ],
  ],
  recapPt: [
    "class Dog : public Animal recebe tudo o que Animal tem.",
    "O construtor da mãe corre primeiro: Dog(...) : Animal(...) {}.",
    "virtual na mãe e override na filha deixam a filha substituir o método.",
    "Uma classe com métodos virtual precisa de virtual ~Classe() = default;",
  ],
  recap: [
    "class Dog : public Animal gets everything Animal has.",
    "The parent's constructor runs first: Dog(...) : Animal(...) {}.",
    "virtual in the parent and override in the child let the child replace a method.",
    "A class with virtual methods needs virtual ~Class() = default;",
  ],
};
