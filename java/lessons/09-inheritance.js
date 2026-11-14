module.exports = {
  id: "inheritance",
  title: "Inheritance and interfaces",
  titlePt: "Herança e interfaces",
  screens: [
    [
      "One class can build on another with extends: it gets every field and method the parent has, and can replace methods. @Override asks Java to check the replacement really matches one in the parent. super(...) runs the parent's constructor:",
      { code: 'public class Main {\n    public static void main(String[] args) {\n        Animal a = new Dog("Rex");\n        System.out.println(a.speak());\n    }\n}\n\nclass Animal {\n    String name;\n    Animal(String name) { this.name = name; }\n    String speak() { return name + " makes a sound"; }\n}\n\nclass Dog extends Animal {\n    Dog(String name) { super(name); }\n    @Override\n    String speak() { return name + " barks"; }\n}' },
      "a is declared as an Animal but holds a Dog, and the Dog's speak() runs. The real object decides.",
    ],
    [
      "An interface lists methods a class promises to have, without writing them. Different classes can then be used the same way:",
      { code: 'public class Main {\n    public static void main(String[] args) {\n        Shape s = new Square(3);\n        System.out.println(s.area());\n    }\n}\n\ninterface Shape {\n    double area();\n}\n\nclass Square implements Shape {\n    double side;\n    Square(double side) { this.side = side; }\n    public double area() { return side * side; }\n}' },
      "Methods from an interface are written with public in the class.",
    ],
    [
      "A class that says it implements an interface but leaves a method out is refused:",
      { code: 'public class Main {\n    public static void main(String[] args) {\n    }\n}\n\ninterface Shape {\n    double area();\n}\n\nclass Circle implements Shape {\n}', error: true },
      { quiz: "Animal a = new Dog(\"Rex\"); a.speak(); Which speak() runs?", choices: ["Animal's", "Dog's"], answer: 2, why: "The object is a Dog, so its version runs." },
    ],
    [
      {
        exercise: "inheritance",
        task: "Write interface Shape with double area(). Write Square (a side) and Circle (a radius, area 3.14 * r * r) implementing it. Put a Square of side 2 and a Circle of radius 1 in a Shape[] array and print each area.",
        expected: ["4.0", "3.14"],
        hints: ["class Circle implements Shape { double r; Circle(double r) { this.r = r; } public double area() { return 3.14 * r * r; } }", "Shape[] shapes = { new Square(2), new Circle(1) }; then for (Shape s : shapes)."],
        solution: 'public class Main {\n    public static void main(String[] args) {\n        Shape[] shapes = { new Square(2), new Circle(1) };\n        for (Shape s : shapes) {\n            System.out.println(s.area());\n        }\n    }\n}\n\ninterface Shape {\n    double area();\n}\n\nclass Square implements Shape {\n    double side;\n    Square(double side) { this.side = side; }\n    public double area() { return side * side; }\n}\n\nclass Circle implements Shape {\n    double r;\n    Circle(double r) { this.r = r; }\n    public double area() { return 3.14 * r * r; }\n}',
      },
    ],
  ],
  recapPt: [
    "class Dog extends Animal recebe tudo o que Animal tem; super(...) corre o construtor da mãe.",
    "@Override verifica que o método substitui mesmo um da mãe.",
    "O objeto real decide qual versão do método corre.",
    "Uma interface lista métodos; implements promete escrevê-los todos.",
  ],
  recap: [
    "class Dog extends Animal gets everything Animal has; super(...) runs the parent's constructor.",
    "@Override checks the method really replaces one in the parent.",
    "The real object decides which version of a method runs.",
    "An interface lists methods; implements promises to write them all.",
  ],
};
